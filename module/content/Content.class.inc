<?php
include_once('content.tool.inc');
include_once('ContentModel.class.inc');
include_once('locale/'.LANGUAGE.'.locale.inc');

abstract class Content extends Control{
	protected $comment;
	protected $categoryID;
	
	public function write(){
		$this->setConfigForm();
		$this->extendFormDescription('content_write_description');
		if(strlen($this->writeLabel) == 0){
			$this->writeLabel = sprintf(tt('Write new %s entry'), $this->moduleTitle);
		}
		return Control::write();
	}
	
	public function edit(){
		$this->setConfigForm();
		$this->extendFormDescription('content_edit_description');
		return Control::edit();
	}
	
	public function insert(){
		Control::insert();
		extension_execute('content_insert', $this->data);
		$this->selectByID($this->column, true);
		$this->insertTracker('write');
		if($this->isFeed() and $this->isReadable(false)){
			$feedData = $this->formatFeedData();
			feed_insert($this->fullModuleName, $feedData);
		}
		document_set_redirect($this->fullModuleName.'/'.$this->id);
		count_increase('contribution', USER_ID);
		$this->subscribe();
	}
	
	public function update(){
		Control::update();
		extension_execute('content_update', $this->data);
		$this->insertTracker('update');
		if($this->isReadable(false)){
			$feedData = $this->formatFeedData();
			feed_update($this->fullModuleName, $feedData);
		}
		document_set_redirect($this->fullModuleName.'/'.$this->id);
		$this->subscribe();
	}
	
	public function drop(){
		include_once(MODULE_PATH.'feed/feed.tool.inc');
		$this->insertTracker('drop');
		feed_drop($this->fullModuleName, $this->id);
		db_drop_many('comment', 'parent='.$this->id);
		Control::drop();
		extension_execute('content_drop', $this->id);
	}
	
	public function listPage(){
		$clause = 'WHERE (section='.SECTION.' AND publishing_status < 2 AND publish_time <= '.date('Y-m-d').')';
		$title = sprintf(tt('List of %s entries'), $this->moduleTitle);
		document_set_path($title);
		$this->listMeta->mode = $this->fullModuleName.'_list';
		return $this->renderListPage($clause);
	}
	
	public function printPage(){
		$this->id = mid();
		$this->selectByID($this->column);
		execute_set_print();
		document_set_title($this->dataByID['topic']);
		return render($this->dataByID, 'content_print.tpl', $this->fullModuleName.'_print');
	}
	
	public function contributorListPage(){
		$title = sprintf(tt('List of %s entries for Contributor'), $this->moduleTitle);
		$this->listMeta->title =  $title;
		$this->isOwnerMode = true;
		document_set_path($title);
		$p = tt('In the following list, your own entries of %s are shown. You can use this page to access them.');
		$this->listMeta->description = sprintf($p, $this->moduleTitle);
		$this->listMeta->mode = $this->fullModuleName.'_contributor_list';
		return $this->renderListPage('WHERE (writer='.USER_ID.')');
	}
	
	public function editorListPage(){
		$this->modelConnector->setAttribute('writer', 'isList', true);
		$title = sprintf(tt('List of %s entries for Editor'), $this->moduleTitle);
		$this->listMeta->title =  $title;
		document_set_path($title);
		$p = tt('You as Editor can use this page to access the entries of %s in Section.');
		$this->listMeta->description = sprintf($p, $this->moduleTitle);
		$this->listMeta->mode = $this->fullModuleName.'_editor_list';
		return $this->renderListPage('WHERE (section='.SECTION.')');
	}
	
	public function initModule($requirement){
		Control::initModule($requirement);
		content_init_module($this);
	}
	
	public function dropModule(){
		Control::dropModule();
		include_once('content.tool.inc');
		content_drop_module($this->fullModuleName, $this->moduleTitle);
	}
	
	protected function init($modelObj, $isEmbed=false, $setPath=true){
		Control::init($modelObj, $isEmbed, $setPath);
		$this->setAccess();
	}
	
	protected function renderListPage($clause){
		$this->listMeta->defaultOrderDesc = 'id';
		$this->selectListPage(20, $clause);
		$this->renderWriter();
		$this->setListWriteIcon();
		$this->setListEditIcon();
		$this->setListDropIcon();
		return Control::renderListPage();
	}
	
	protected function selectFirstPage($column, $pageLength){
		if(gg('tag')){
			$this->selectFirstPageTag($column, $pageLength);
		}elseif(gg('year')){
			$this->selectFirstPageArchive($column, $pageLength);
		}elseif(pid() == 0){
			$this->selectFirstPageFearured($column, $pageLength);
		}else{
			$this->selectFirstPageCommon($column, $pageLength);
		}
	}
	
	protected function postSelect($column){
		Control::postSelect($column);
		$this->addCategoryToFreeTag();
		$this->clearDailyCache();
		foreach($this->data as $key => $value){
			$this->data[$key]['module'] = $this->fullModuleName;
		}
		$this->data = extension_execute('content_post_select', $this->data);
	}
	
	protected function getCommentNumber(){
		$idArray = array_keys($this->data);
		if(count($idArray)){
			$clause = 'WHERE (parent='.join(' OR parent=', $idArray).") AND mode='".$this->fullModuleName."'GROUP BY parent";
			$commentNumber = db_select('comment', array('parent', 'COUNT(id) as number'), $clause);
			foreach($commentNumber as $key=>$value){
				$this->data[$value['parent']]['comment_no'] = $value['number'];
			}
		}
	}
	
	protected function getDataEditable(){
		foreach($this->data as $key => $value){
			if(isset($value['writer'])){
				$this->data[$key]['editable'] = $this->isEditable($value['writer']['id']);
			}else{
				break;
			}
		}
	}
	
	protected function isReadable($checkEdit=true){
		$publishTime = strtotime($this->dataByID['publish_time']);
		if(strlen($this->dataByID['publishing_status']) > 1){
			$publishingMap = array_flip($this->getPublishingStatus());
			$publishingStatus = $publishingMap[$this->dataByID['publishing_status']];
		}else{
			$publishingStatus = $this->dataByID['publishing_status'];
		}
		if($publishingStatus < 2 and $publishTime <= time()) return true;
		elseif($checkEdit and $this->isEditable($this->dataByID['writer']['id'])) return true;
		else return false;
	}
	
	protected function isEditable($writerID){
		if(authority_group($this->moduleName.'_admin')) return true;
		if(authority_group($this->moduleName.'_editor')) return true;
		if(authority_owner($this->moduleName.'_contributor', $writerID)) return true;
		return false;
	}
	
	protected function getPageMode(){
		if(gg('tag')) return '/tag/'.gg('tag');
		if(gg('year')) return '/year/'.gg('year').'/month/'.gg('month').'/day/'.gg('day');
	}
	
	protected function setPathMutiplePage(){
		if(strlen(gg('tag'))){
			if(gg('tag_label')){
				document_set_path(gg('tag_label'), $this->fullModuleName.'/tag/'.gg('tag'));
			}else{
				document_set_path(gg('tag'), $this->fullModuleName.'/tag/'.gg('tag'));
			}
		}elseif(strlen(gg('year'))){
			$archievTime = date('F Y', strtotime('01-'.gg('month').'-20'.gg('year')));
			document_set_path(tt('Archieve'), $this->fullModuleName.'_archiev');
			document_set_path($archievTime,$this->fullModuleName.'/year/'.gg('year').'/month/'.gg('month'));
		}
	}
	
	protected function initComment(){
		if(!$this->comment){
			include_once(MODULE_PATH.'comment/Comment.class.inc');

			if(!isset($this->dataByID['comment_permission'])) $permission = null;
			else $permission = $this->dataByID['comment_permission'];

			$this->comment = new Comment(false);
			$this->comment->setMode($this->fullModuleName);
			$this->comment->setParent($this->id);
			$this->comment->setPermission($permission);
			$this->comment->setParentWriter($this->dataByID['writer']['id']);
			$this->comment->init();
		}
	}
	
	protected function getSummary(){
		mb_internal_encoding('utf8');
		return mb_substr(strip_tags($this->dataByID['content']), 0, 2000);
	}
	
	protected function insertTracker($action){
		if(!authority_is_guest()){
			include_once(MODULE_PATH.'tracker/tracker.tool.inc');
			$data = $this->dataByID;
			if(!is_array($this->dataByID['writer'])){
				$clause = 'WHERE id='.$this->dataByID['writer'];
				$writerName = db_select_single_column('users', 'login_name', $clause, false);
				if(isset($writerName[0])){
					$this->dataByID['writer_name'] = $writerName[0];
					$data['writer'] = $this->dataByID['writer'];
					$data['writer_name'] = $writerName[0];
				}
			}else{
				$data['writer'] = $this->dataByID['writer']['id'];
				$data['writer_name'] = $this->dataByID['writer']['login_name'];
			}
			$data['action'] = $action;
			$data['mode'] = $this->fullModuleName;
			tracker_insert($data);
		}
	}
	
	protected function setConfigForm(){
		$tags = $this->getNewConfigForm(tt('category & tags'));
		$tags->append('category');
		$tags->append('freetag');
		
		$publish = $this->getNewConfigForm(tt('publishing and comment'));
		$publish->append('publishing_status');
		$publish->append('publish_time');
		$publish->append('comment_permission');
		
		$searchEngine = $this->getNewConfigForm(tt('search engine'));
		$searchEngine->append('uri');
		$searchEngine->append('description');
	}
	
	protected function addMetaData(){
		document_add_keywords($this->dataByID['freetag']);
		document_set_description($this->dataByID['description']);
	}
	
	protected function isFeed(){
		return in_array($this->fullModuleName, cc('feed_mode')) or true;
	}
	
	protected function formatFeedData(){
		include_once(MODULE_PATH.'feed/feed.tool.inc');
		$category = db_select_single_column('category', 'category', 'WHERE id='.$this->dataByID['category'], false);
		$this->getCommentNumber();
		$data = $this->data[$this->id];
		$data['writer'] = $this->dataByID['writer_name'];
		$data['summary'] = $this->getSummary();
		if(isset($category[0])) $data['freetag'].= ', '.$category[0];
		return $data;
	}
	
	protected function isSingleEntry(){
		return mid() and MODULE == $this->moduleName;
	}
	
	protected function accessDeniedEntry(){
		document_set_redirect($this->fullModuleName);
		execute_set_db();
		notify_add_error(sprintf(tt('You are not allowed to view the selected %s.'), $this->moduleTitle));
	}
	
	protected function handlePublishingStatus(){
		if($this->dataByID['publishing_status'] < 2){
			search_set_page();
			count_increase($this->fullModuleName, $this->id);
		}
	}
	
	protected function setDocumentData(){
		$category = $this->dataByID['category']['category'];
		$reference = $this->dataByID['category']['reference'];
		document_set_path($category, $this->moduleName.'/tag/'.$reference.'/tag_label/'.$category);
		document_set_path($this->dataByID['topic']);
	}
	
	protected function getPublishingStatus(){
		return content_get_publishing_status();
	}
	
	protected function renderWriteIcon(){
		if($this->isCallable('write')){
			if($this->categoryID) $category = '/category_id/'.$this->categoryID;
			else $category = null;
			
			if(strlen($this->writeLabel) == 0){
				$this->writeLabel = sprintf(tt('Write new %s entry'), $this->moduleTitle);
			}
			$mode = $this->fullModuleName.'_write'.$category;
			return render_write_icon($mode, $this->writeLabel);
		}
	}
	
	protected function getExtendedOperation(){
		$operation = array();
		foreach($this->data as $id => $data){
			$data['module'] = $this->fullModuleName;
			if(is_array($data['writer'])) $data['owner'] = $data['writer']['id'];
			else $data['owner'] = $data['writer'];
			$operation[$id] = extension_execute_append_result('content_operation', $data, array());
		}
		return $operation;
	}
	
	private function extendFormDescription($mode){
		$description = $this->formMeta->description;
		$description = extension_execute($mode, $description);
		$this->formMeta->description = $description;
	}
	
	private function selectFirstPageTag($column, $pageLength){
		$tag = gg('tag');
		$tag = addslashes($tag);
		$tagClause = $this->getTagClause($tag);
		$categoryClause = $this->getCategoryClause($tag);
		
		if(strlen($tagClause)){
			if(strlen($categoryClause)==0) $tagClause = "AND ($tagClause)";
			else $tagClause = "AND ($tagClause OR $categoryClause)";
		}else{
			if(strlen($categoryClause)==0) $tagClause = "AND id=0";
			else $tagClause = "AND ".$categoryClause;
		}

		$clause  = "WHERE (section=".SECTION." AND publishing_status < 2 ";
		$clause .= "AND publish_time <= '".date('Y-m-d')." 23:59:59' $tagClause )";
		$clause .= "ORDER BY publish_time DESC, id DESC";
		$this->selectPage($column, $pageLength, $clause);
	}
	
	private function selectFirstPageArchive($column, $pageLength){
		$year = (int) gg('year');
		$month = (int) gg('month');
		$day = (int) gg('day');
		
		$publishDate = "AND publish_time <= '".date('Y-m-d')."'";
		if(empty($day)){
			$archive  = " (publish_time > '$year-$mounth-1' AND publish_time < '$year-$mounth--31' $publishDate)";
		}else{
			$date = "$year-$mounth-$day";
			$archive  = " (publish_time >= '$data 00:00:00' AND publish_time < '$date 23:59:59' $publishDate)";
		}
		$clause = "WHERE ($archive AND section=".SECTION." AND publishing_status < 2)";
		$clause.= "ORDER BY publish_time DESC, id DESC";
		$this->selectPage($column, $pageLength, $clause);
	}
	
	private function selectFirstPageFearured($column, $pageLength){
		$clause = "WHERE (section=".SECTION." AND publishing_status=1 AND publish_time <= '".date('Y-m-d')." 23:59:59') ";
		$clause.= "ORDER BY publish_time DESC, id DESC";
		$this->selectPage($column, $pageLength, $clause);
		$featuredData = $this->data;
		
		$this->selectFirstPageCommon($column, $pageLength);
		$publishedData = $this->data;
		
		$this->data = array();
		foreach($featuredData as $key => $value){
			$this->data[$key] = $value;
		}
		foreach($publishedData as $key => $value){
			$this->data[$key] = $value;
		}
	}
	
	private function selectFirstPageCommon($column, $pageLength){
		$clause = "WHERE (section=".SECTION." AND publishing_status=0 AND publish_time <= '".date('Y-m-d')." 23:59:59') ";
		$clause.= "ORDER BY publish_time DESC, id DESC";
		$this->selectPage($column, $pageLength, $clause);
	}
	
	private function setAccess(){
		$adminGroup = $this->moduleName.'_admin';
		$editorGroup = $this->moduleName.'_editor';
		$contributorGroup = $this->moduleName.'_contributor';
		$this->addAction($this->fullModuleName.'_list', 'listPage');
		$this->addAction($this->fullModuleName.'_editor_list', 'editorListPage');
		$this->addAction($this->fullModuleName.'_contributor_list', 'contributorListPage');
		
		$this->ownerColumn = 'writer';
		$this->setAdmin($adminGroup);
		
		$this->addAccessMethod($editorGroup, 'write');
		$this->addAccessMethod($editorGroup, 'edit');
		$this->addAccessMethod($editorGroup, 'insert');
		$this->addAccessMethod($editorGroup, 'update');
		$this->addAccessMethod($editorGroup, 'drop');
		$this->addAccessMethod($editorGroup, 'editorListPage');
		
		$this->addAccessMethod($contributorGroup, 'write');
		$this->addAccessMethod($contributorGroup, 'insert');
		$this->addAccessMethod($contributorGroup, 'contributorListPage');
		
		$this->addAccessMethod('guest', 'index');
		$this->addAccessMethod('guest', 'listPage');
		$this->addAccessMethod('guest', 'printPage');
	}
	
	
	private function renderWriter(){
		foreach($this->data as $key => $value){
			if(isset($value['writer'])){
				$this->data[$key]['writer'] = render_profile_link($value['writer']);
			}else{
				break;
			}
		}
	}
	
	private function addCategoryToFreeTag(){
		foreach($this->data as $key => $value){
			if(isset($value['category']) and isset($value['freetag'])){
				$category = $value['category']['category'];
				$this->data[$key]['freetag'] = array_merge(array($category), $value['freetag']);
			}else{
				break;
			}
		}
		if(!empty($this->dataByID)) $this->dataByID = $this->data[$this->id];
	}
	
	private function clearDailyCache(){
		$dateFile = FILES_PATH.'dbcache/'.$this->fullModuleName.'/lastclear.txt';
		$cacheDirectory = FILES_PATH.'dbcache/'.$this->fullModuleName;
		cache_clear_by_day($dateFile, $cacheDirectory);
	}
	
	private function getTagClause($tag){
		$clause = "WHERE (freetag='".$tag."' AND mode='".$this->fullModuleName."')";
		$tagData = db_select('freetag', array('content_id'), $clause);
		$contentID = array();
		foreach($tagData as $key => $value){
			$contentID[] = $value['content_id'];
		}
		if(count($tagData)) return 'id='.join(' OR id=', $contentID);
		else return '';
	}
	
	private function getCategoryClause($tag){
		$clause = "WHERE (category='".$tag."' AND mode='".$this->fullModuleName."')";
		$category = array_keys(db_select_single_column('category', 'id', $clause));
		if(isset($category[0])){
			$this->categoryID = $category[0];
			return 'category='.$category[0];
		}else{
			$this->categoryID = null;
			return '';
		}
		
	}
	
	private function subscribe(){
		$message = tt('%s posts <a href="%s">%s</a> in %s.');
		$uri = SECTION_URI.Q.$this->fullModuleName.'/'.$this->id;
		$message = sprintf($message, USER_NAME, $uri, pp('topic'), $this->moduleTitle);
		notify_subscribe_by_mode($message, $this->fullModuleName);
	}
}
?>
