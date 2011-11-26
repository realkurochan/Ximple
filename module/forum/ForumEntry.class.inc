<?php
execute_add_extended_class('forum', 'entry', 'ForumEntry');
include_once(MODULE_PATH.'content/Content.class.inc');
include_once('forum.tool.inc');
include_once('forum.module.inc');
include_once('ForumEntryModel.class.inc');

class ForumEntry extends Content{
	function ForumEntry(){
		$this->tableName = 'forum_entry';
		$this->moduleName = 'forum';
		$this->moduleExtension = 'entry';
		$this->moduleTitle = tt('Forum Entry');
		$this->addAction('forum_entry_insert_comment', 'insertComment');
		$this->addAccessMethod('guest', 'write');
		$this->addAccessMethod('guest', 'insert');
		$this->addAccessMethod('guest', 'insertComment');
		$this->init(new ForumEntryModel());
		$this->writeLabel = tt('Write new Forum Entry');
		$this->formMeta->isCaptcha = true;
		document_set_path(tt('Forum'), 'forum');
		menu_activate('forum');
	}
	
	public function index(){
		$this->id = gg('id');
		$this->selectByID($this->column);
		$this->getCommentNumber();
		if(!authority_is_guest()){
			count_set('forum_entry'.USER_ID, $this->id, time());
		}
		$readableStatus = ($this->dataByID['publishing_status'] <= 1);
		$readableStatus = ($readableStatus or ($this->dataByID['writer']['id'] == USER_ID));
		if(!$readableStatus or !forum_check_readable($this->dataByID['forum'])){
			error_denied();
		}
		
		count_increase($this->fullModuleName, $this->id);
		head_add_css('forum.css');
		forum_set_path($this->dataByID['forum']);
		document_set_path($this->dataByID['topic']);
		
		$this->getAppearanceConfiguration();
		$writerID = $this->dataByID['writer']['id'];
		$this->dataByID['editable'] = $this->isEditable($writerID);
		
		$this->initComment();
		$this->insertTracker('read');
		$this->comment->setTemplate('forum_comment.tpl');
		$this->comment->setInsertMode('forum_entry_insert_comment');
		
		$operation = $this->getExtendedOperation();
		$this->dataByID['addition_operation'] = $operation[$this->id];
		
		$render = render($this->dataByID, 'forum_entry.tpl', 'forumentry');
		$render.= $this->comment->render();
		
		return $render;
	}
	
	public function write(){
		if($this->isWritable()){
			if(gg('forum')) forum_set_path(gg('forum'));
			return Content::write();
		}else{
			error_denied();
		}
	}
	
	public function insert(){
		Control::insert();
		count_increase('contribution', USER_ID);
		$this->selectByID($this->column);
		$this->dataByID['category'] = forum_get_category_id($this->dataByID['forum']);
		$this->insertTracker('write');
		if($this->isFeed() and $this->isReadable(false)){
			$feedData = $this->formatFeedData();
			feed_insert($this->moduleName, $feedData);
		}
		document_set_redirect('forum_entry/'.$this->id);
		count_increase('forum_entry_no', $this->dataByID['forum']);
		$this->subscribe();
		$this->checkPublishable();
	}
	
	public function insertComment(){
		$this->initComment();
		$this->comment->insert();
		$data = array(
			'last_commentator' => pp('writer'),
			'last_commentator_id' => USER_ID,
			'update_time' => NOW,
		);
		db_update('forum_entry', $data, pp('parent'));
		$this->id = pp('parent');
		$this->selectByID($this->column);
		count_increase('forum_comment_no', $this->dataByID['forum']);
		$this->subscribeComment();
	}
	
	public function edit(){
		$render = Content::edit();
		forum_set_path($this->dataByID['forum']);
		return $render;
	}
	
	public function update(){
		Content::update();
		$this->checkPublishable();
	}
	
	public function listPage(){
		$this->prepareList();
		head_add_css('list.css');
		$renderArray = array(
			'data' => $this->getListData(),
			'forum' => forum_get_info(gg('forum')),
			'writable' => $this->isWritable(),
			'pager' => $this->pager->render('forum_entry_list/forum/'.gg('forum')),
		);
		return render($renderArray, 'forum_list.tpl', 'forum_list');
	}
	
	public function contributorListPage(){
		$this->modelConnector->setAttribute('writer_name', 'isList', false);
		$title = sprintf(tt('List of %s for Contributor'), $this->moduleTitle);
		$this->listMeta->title =  $title;
		document_set_path($title);
		$p = tt('In the following list, your own %s are shown. You can use this page to access them.');
		$this->listMeta->description = sprintf($p, $this->moduleTitle);
		$this->listMeta->mode = $this->moduleName.'_contributor_list';
		$this->getListColumn();
		return $this->renderListPage('WHERE (writer='.USER_ID.')');
	}
	
	public function editorListPage(){
		$title = sprintf(tt('List of %s for Editor'), $this->moduleTitle);
		$this->listMeta->title =  $title;
		document_set_path($title);
		$p = tt('You as Editor can use this page to access %s in Section.');
		$this->listMeta->description = sprintf($p, $this->moduleTitle);
		$this->listMeta->mode = $this->moduleName.'_editor_list';
		$this->getListColumn();
		return $this->renderListPage('WHERE (section='.SECTION.')');
	}
	
	protected function setConfigForm(){
		$publish = $this->getNewConfigForm(tt('publishing and comment'));
		$publish->append('publishing_status');
		$publish->append('comment_permission');
	}
	
	protected function postSelect($column){
		Control::postSelect($column);
		foreach($this->data as $key => $value){
			$this->data[$key]['uri'] = 	$value['topic'];
			if(isset($value['writer']) and $value['writer']['id'] == -1){
				$this->data[$key]['writer']['login_name'] = $value['writer_name'];
			}
		}
	}
	
	protected function isReadable(){
		if($this->dataByID['publishing_status'] < 2) return true;
		elseif($this->isEditable($this->dataByID['writer']['id'])) return true;
		else return false;
	}
	
	protected function formatFeedData(){
		include_once(MODULE_PATH.'feed/feed.tool.inc');
		$this->getCommentNumber();
		$data = $this->data[$this->id];
		$data['writer']  = $this->dataByID['writer_name'];
		$data['summary'] = $this->dataByID['content'];
		$forum = forum_get_info($this->dataByID['forum']);
		$data['freetag'] = $forum['name'];
		return $data;
	}
	
	private function prepareList(){
		$forumID = gg('forum');
		if($forumID and forum_check_readable($forumID)) forum_set_path($forumID);
		else error_denied();
	}
	
	private function getListData(){
		$featuredData = $this->getFeaturedData();
		$regularData = $this->getRegularData();
		$data = array();
		foreach($featuredData as $key => $value){
			$value['icon'] = 'stick';
			$data[] = $value;
		}
		$regularData = $this->checkNew($regularData);
		foreach($regularData as $key => $value){
			$value = $this->getIcon($value);
			$data[] = $this->getCommentator($value);
		}
		return $data;
	}
	
	private function isCommentable($permission){
		if($permission == 0) return true;
		elseif($permission == 1 and !authority_is_guest()) return true;
		else return false;
	}
	
	private function checkNew($data){
		if(!authority_is_guest()){
			$count = count_get_array('forum_entry'.USER_ID, array_keys($data));
			foreach($count as $id => $accessTime){
				$commentTime = strtotime($this->data[$id]['comment_time']);
				$data[$id]['is_new'] = ($accessTime < $commentTime);
			}
		}else{
			foreach($data as $id => $value){
				$data[$id]['is_new'] = false;
			}
		}
		return $data;
	}
	
	private function getFeaturedData(){
		$forumID = (int) gg('forum');
		$pattern = "WHERE forum=%d AND publishing_status=1 ORDER by comment_time DESC";
		return $this->getDataFromDB(sprintf($pattern, $forumID));
	}
	
	private function getRegularData(){
		$forumID = (int) gg('forum');
		if(authority_group('forum_entry_editor') or authority_group('forum_entry_admin')){
			$pattern = "WHERE forum=%d AND (publishing_status=0 OR publishing_status=4) ORDER by update_time DESC";
			$clause = sprintf($pattern, $forumID);
		}else{
			$pattern = "WHERE forum=%d AND (publishing_status=0 OR (publishing_status=4 AND writer=%d)) ORDER by update_time DESC";
			$clause = sprintf($pattern, $forumID, USER_ID);
		}
		return $this->getDataFromDB($clause);
	}
	
	private function getDataFromDB($clause){
		$column = $this->getShowPageListColumn();
		$this->selectPage($column, 20, $clause);
		$this->getCommentNumber();
		return $this->data;
	}
	
	private function getShowPageListColumn(){
		return array(
			'id',
			'topic',
			'write_time',
			'writer',
			'writer_name',
			'last_commentator',
			'last_commentator_id',
			'comment_time',
			'comment_permission'
		);
	}
	
	private function getIcon($entry){
		if($entry['is_new']) $entry['icon'] = 'new';
		else $entry['icon'] = 'default';
		
		if(isset($entry['comment_no'])){
			if($entry['comment_no'] > 25) $entry['icon'] = 'veryhot';
			elseif($entry['comment_no'] > 15) $entry['icon'] = 'hot';
		}elseif(!$this->isCommentable($entry['comment_permission'])){
			$entry['icon'] = 'lock';
		}
		return $entry;
	}
	
	private function getCommentator($entry){	
		if(strlen($entry['last_commentator']) and $entry['last_commentator_id']['id'] != -1){
			$entry['last_commentator'] = render_profile_link($entry['last_commentator_id']);
		}
		return $entry;
	}
	
	private function getAppearanceConfiguration(){
		$appearance = cc('forum_appearance');
		if(!is_array($appearance)) $appearance = array();
		$this->dataByID['show_avatar'] = in_array('avatar', $appearance);
		$this->dataByID['show_level'] = in_array('level', $appearance);
	}
	
	private function isWritable(){
		if(forum_check_authority_group()){
			return true;
		}
		if(gg('forum')){
			if(in_array(gg('forum'), forum_get_writable_id())){
				return true;
			}
		}
		return true;
	}
	
	private function checkPublishable(){
		$publishableForum = forum_get_publishable_id();
		if(!in_array(pp('forum'), $publishableForum)){
			$data = array('publishing_status' => 2);
			db_update('forum_entry', $data, $this->id);
			$message  = tt("You don't have permission to publish forum entry. ");
			$message .= tt('Your forum entry is in review.');
			notify_add_warning($message);
		}
	}
	
	private function subscribe(){
		$message = tt('%s posts <a href="%s">%s</a> in your subscribed forum.');
		$uri = SECTION_URI.Q.'forum_entry/'.$this->id;
		$message = sprintf($message, pp('writer_name'), $uri, pp('topic'));
		notify_subscribe_by_mode($message, 'forum_'.pp('forum'));
	}
	
	private function subscribeComment(){
		$message = tt('%s comments in <a href="%s">%s</a>, which is in your subscribed forum.');
		$uri = SECTION_URI.Q.'forum_entry/'.$this->id;
		$message = sprintf($message, pp('writer'), $uri, $this->dataByID['topic']);
		notify_subscribe_by_mode($message, 'forum_comment_'.$this->dataByID['forum']);
	}
}
?>
