<?php
execute_add_class('comment', 'Comment');
include('CommentModel.class.inc');
include_once('comment.tool.inc');

/// Module, which uses comment, must have these columns (in other cases alternative column name must be set) :
/// - topic
/// - writer
//  - comment_permission
//  - comment_time
/// and these configurations :
/// - module_name_appearance [comment_avatar, signature]
/// - module_name_comment_write_label
/// - module_name_comment_page_length
class Comment extends Control{
	public $parentTopicColumn = 'topic';
	public $parentWriterColumn = 'writer';
	public $parentURIColumn = 'uri';
	public $parentHasURIColumn = true;
	public $parentPermissionColumn = 'comment_permission';
	public $parentIsAjax = false;
	public $parentCommentTimeColumn = 'comment_time';
	
	private $mode = null;
	private $parent = null;
	private $permission = null;
	
	private $parentWriter = null;
	private $parentData;
	
	private $template;
	private $insertMode;
	private $parentAdminGroup;
	private $subscribeMode = null;
	private $subscribeMessage = null;
	
	function Comment($isInit=true){
		$this->tableName = 'comment';
		$this->moduleName = 'comment';
		$this->moduleTitle = tt('Comment');
		$this->template = 'comment.tpl';
		$this->insertMode = 'comment_insert';
		if($isInit) $this->parentlessInit();
	}
	
	public function index(){
		$this->id = (int) gg('id');
		if($this->id){
			$this->selectByID(array('mode', 'parent'));
			$uri = $this->dataByID['mode'].'/'.$this->dataByID['parent'].'#comment_'.$this->id;
			document_set_redirect($uri);
			execute_set_db();
		}else{
			error_not_found();
		}
	}
	
	public function parentlessInit(){
		$this->setParent(null);
		$this->setMode(null);
		$this->setPermission(null);
		$this->setAccess();
		Control::init(new CommentModel($this->mode, $this->parent, $this->parentWriter));
	}
	
	public function init(){
		$this->setAccess();
		Control::init(new CommentModel($this->mode, $this->parent, $this->parentWriter));
	}
	
	public function setMode($mode){
		if($mode) $this->mode = $mode;
		elseif(pp('mode')) $this->mode = addslashes(pp('mode'));
		elseif(gg('module')) $this->mode = addslashes(gg('module'));
		$this->parentAdminGroup = $this->mode.'_comment_admin';
	}
	
	public function setParent($parent){
		if($parent) $this->parent = $parent;
		elseif(pp('parent')) $this->parent = (int) pp('parent');
		elseif(gg('parent')) $this->parent = (int) gg('parent');
	}
	
	public function setSubscribeMode($mode){
		$this->subscribeMode = $mode;
	}
	
	public function setParentWriter($parentWriter){
		$this->parentWriter = $parentWriter;
	}
	
	public function setParentPermissionColumn($parentPermissionColumn){
		$this->parentPermissionColumn = $parentPermissionColumn;
	}
	
	public function setPermission($permission){
		if(strlen($permission)){
			$this->permission = $permission;
		}elseif(!empty($this->mode) and !empty($this->mode)){
			$clause = 'WHERE id='.((int) $this->parent);
			$permission = db_select_single_column($this->mode, $this->parentPermissionColumn, $clause, false);
			$this->permission = $permission[0];
		}
	}
	
	public function render(){
		$render = $this->renderEntry();
		$render.= $this->write();
		return $render;
	}
	
	public function write(){
		if($this->isWritable()){
			$this->formMeta->id = $this->moduleName;
			$this->formMeta->title = cc($this->mode.'_comment_write_label');
			$this->formMeta->isCaptcha = true;
			$this->formMeta->mode  = $this->insertMode;
			$this->formMeta->mode .= '/page/'.$this->pager->getNextPage();
			$this->formMeta->isSetTitle = false;
			$this->formMeta->isAjax = $this->parentIsAjax;
			$this->setParentColumn();
			return $this->renderInsertForm(false);
		}else{
			return '';
		}
	}
	
	public function insert(){
		if($this->isWritable()){
			$this->getParentColumn();
			$this->formMeta->isCaptcha = true;
			if(Control::insert()){
				count_increase('contribution', USER_ID);
				$column = array('id', $this->parentTopicColumn, $this->parentWriterColumn);
				$this->parentData = db_select($this->mode, $column, 'WHERE id='.$this->parent);
				$this->parentData = $this->parentData[$this->parent];
				$this->insertTracker();
				$this->insertNotify();
				db_update($this->mode, array('comment_time' => NOW), $this->parent);
				document_set_redirect($this->mode.'/'.$this->parent.'/page/'.pid());
			}
		}
	}
	
	public function update(){
		Control::update();
		document_set_redirect($this->mode.'/'.$this->parent.'/page/'.pid().'#comment_'.$this->id);
	}
	
	public function drop(){
		Control::drop();
		document_set_redirect($this->mode.'/'.$this->parent);
	}
	
	public function listPage(){
		$clause = 'WHERE (writer_id='.USER_ID.')';
		$label = tt('My Comments');
		$description = '';
		return $this->renderListPage($clause, $label, $description);
	}
	
	public function contributorListPage(){
		$clause = 'WHERE (parent_writer='.USER_ID.')';
		$label = tt('Comments on my Contributions');
		$description = '';
		return $this->renderListPage($clause, $label, $description);
	}
	
	public function sectionListPage(){
		$clause = 'WHERE (section='.SECTION.')';
		$label = sprintf(tt('Comments in %s'), cc('section_name'));
		$description = '';
		return $this->renderListPage($clause, $label, $description);
	}
	
	public function setTemplate($template){
		$this->template = $template;
	}
	
	public function setInsertMode($insertMode){
		$this->insertMode = $insertMode;
	}
		
	private function setAccess(){
		$this->ownerColumn = 'writer_id';
		if($this->isWritable()) $this->addAccessMethod('guest', 'insert');
		$this->setAdmin('comment_admin');
		
		$this->addAction('comment_list', 'listPage');
		$this->addAction('comment_section_list', 'sectionListPage');
		$this->addAction('comment_contributor_list', 'contributorListPage');
		
		$this->addAccessMethod('user', 'listPage');
		$this->addAccessMethod('user', 'sectionListPage');
		$this->addAccessMethod('user', 'contributorListPage');
		
		$this->addAccessMethod($this->parentAdminGroup, 'edit');
		$this->addAccessMethod($this->parentAdminGroup, 'update');
		$this->addAccessMethod($this->parentAdminGroup, 'drop');
	}
	
	protected function renderListPage($clause, $label, $description){
		$title = sprintf(tt('List of %s'), $label);
		document_set_path($title);
		document_set_title($title);
		$this->listMeta->title = $title;
		$this->listMeta->mode = $this->moduleName;
		$this->listMeta->defaultOrderDesc = 'id';
		$this->listMeta->description = $description;
		$this->selectListPage(20, $clause, array('mode'));
		$this->renderParentTopic();
		return Control::renderListPage();
	}
	
	private function renderEntry(){
		if($this->isReadable()){
			head_add_css('module/comment/theme/comment.css');
			$clause = 'WHERE parent='.$this->parent." AND mode='".$this->mode."' ORDER BY id";
			$this->selectPage($this->column, cc($this->mode.'_comment_page_length'), $clause);
			$this->checkDataEditable();
			$this->setGuestName();
			$this->checkNew();
			$pager = $this->pager->render(mm().'/'.mid());
			$render = $pager;
			$renderArray = array(
				'data' => $this->data,
				'show_avatar' => $this->checkAvatar(),
				'show_signature' => $this->checkSignature(),
				'addition_operation' => $this->getExtendedOperation(),
			);
			$render.= render($renderArray, $this->template, 'comment');
			$render.= $pager;
			return $render;
		}else{
			return '';
		}
	}
	
	private function getExtendedOperation(){
		$operation = array();
		foreach($this->data as $id => $data){
			$data['module'] = 'comment';
			if(is_array($data['writer_id'])) $data['owner'] = $data['writer_id']['id'];
			else $data['owner'] = $data['writer_id'];
			$operation[$id] = extension_execute_append_result('comment_operation', $data, array());
		}
		return $operation;
	}
	
	private function isReadable(){
		return ($this->permission != XIMPLE_COMMENT_DISABLE);
	}
	
	private function isWritable(){
		if($this->permission == XIMPLE_COMMENT_ALL) return true;
		elseif($this->permission == XIMPLE_COMMENT_AUTHENTICATED and !authority_is_guest()) return true;
		else return false;
	}
	
	private function checkDataEditable(){
		$isAdmin = $this->isCallable('edit');
		foreach($this->data as $key => $value){
			$this->data[$key]['editable'] = ($isAdmin or $value['writer_id'] == USER_ID);
		}
	}
	
	private function setGuestName(){
		foreach($this->data as $key => $value){
			if($value['writer_id']['id'] == -1){
				$this->data[$key]['writer_id']['login_name'] = $value['writer'];
			}
		}
	}
	
	private function checkNew(){
		if(!authority_is_guest()){
			$idArray = array_keys($this->data);
			$count = count_get_array('comment'.USER_ID, $idArray);
			foreach($count as $id => $number){
				$this->data[$id]['is_new'] = ($number == 0);
			}
			count_increase('comment'.USER_ID, $idArray);
		}else{
			foreach($this->data as $key => $value){
				$this->data[$key]['is_new'] = false;
			}
		}
	}
	
	private function insertTracker(){
		if(!authority_is_guest()){
			include_once(MODULE_PATH.'tracker/tracker.tool.inc');
			$parentWriter = $this->parentData[$this->parentWriterColumn];
			$writerName = db_select_single_column('users', 'login_name', 'WHERE id='.$parentWriter, false);
			if(isset($writerName[0])) $writerName = $writerName[0];
			else $writerName = tt('guest');
			
			$trackerData = array(
				'mode' => $this->mode,
				'id' => $this->parent,
				'topic' => $this->parentData[$this->parentTopicColumn],
				'writer' => $this->parentData[$this->parentWriterColumn],
				'writer_name' => $writerName,
				'action' => 'comment',
			);
			tracker_insert($trackerData);
		}
	}
	
	private function formatTopic($topic){
		mb_internal_encoding('utf8');
		$topic = preg_replace("/\s/", " ", $topic);
		if(strlen($topic) > 30) $topic = mb_substr($topic, 0, 30).' ...';
		return $topic;
	}
	
	private function insertNotify(){
		$p = tt('%s commented on your contribution : <a href="%s">%s</a>.');
		$uri = SECTION_URI.Q.$this->mode.'/'.$this->parent.'/page/'.pid().'#last_comment';
		$message = sprintf($p, pp('writer'), $uri, $this->parentData[$this->parentTopicColumn]);
		if($this->parentData[$this->parentWriterColumn] != USER_ID){
			notify_add_info($message, $this->parentData[$this->parentWriterColumn]);
			notify_subscribe($message, $this->parentData[$this->parentWriterColumn], 'comment');
		}
		$clause = "WHERE mode='".$this->mode."' AND parent=".$this->parent." AND writer_id != -1 GROUP BY writer_id";
		$relatedUser = db_select_single_column('comment', 'writer_id', $clause, false);
		foreach($relatedUser as $key => $uid){
			if($uid != $this->parentData[$this->parentWriterColumn] and $uid != USER_ID){
				notify_add_info($message, $uid);
			}else{
				unset($relatedUser[$key]);
			}
		}
		notify_multiple_subscribe($message, $relatedUser, 'comment');
		if($this->subscribeMode){
			notify_subscribe_by_mode($this->subscribeMode, $message);
		}
	}
	
	private function renderParentTopic(){
		$mode = array();
		foreach($this->data as $key => $value){
			$mode[$value['mode']][] = $value['parent'];
		}
		
		$column = array('id', $this->parentTopicColumn);
		if($this->parentHasURIColumn) $column[] = $this->parentURIColumn;
		$parent = array();
		$column = array('id', $this->parentTopicColumn);
		if($this->parentHasURIColumn) $column[] = $this->parentURIColumn;
		foreach($mode as $table => $id_array){
			$clause = 'WHERE id='.join(' OR id=', $id_array);
			$parent[$table] = db_select($table, $column, $clause);
		}
		
		foreach($this->data as $key => $value){
			$uri = render_uri($value['mode'], $parent[$value['mode']][$value['parent']]);
			$topic = $this->formatTopic($parent[$value['mode']][$value['parent']][$this->parentTopicColumn]);
			$this->data[$key]['parent'] = '<a href="'.$uri.'">'.$topic.'</a>';
		}
	}
	
	private function checkAvatar(){
		return config_has_value($this->mode.'_appearance', 'comment_avatar');
	}
	
	private function checkSignature(){
		return config_has_value($this->mode.'_appearance', 'signature');
	}
	
	private function setParentColumn(){
		$this->modelConnector->setAttribute('parent_topic_column', 'defaultData', $this->parentTopicColumn);
		$this->modelConnector->setAttribute('parent_writer_column', 'defaultData', $this->parentWriterColumn);
		$this->modelConnector->setAttribute('parent_uri_column', 'defaultData', $this->parentURIColumn);
		$this->modelConnector->setAttribute('parent_has_uri_column', 'defaultData', $this->parentHasURIColumn);
		$this->modelConnector->setAttribute('parent_permission_column', 'defaultData', $this->parentPermissionColumn);
	}
	
	private function getParentColumn(){
		$this->parentTopicColumn = pp('parent_topic_column');
		$this->parentWriterColumn = pp('parent_writer_column');
		$this->parentURIColumn = pp('parent_uri_column');
		$this->parentHasURIColumn = (bool) pp('parent_has_uri_column');
		$this->parentPermissionColumn = pp('parent_permission_column');
	}
}
?>
