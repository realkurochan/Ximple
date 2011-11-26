<?php
execute_add_class('directory', 'PageDirectory');
include_once('PageDirectoryModel.class.inc');
include_once('directory.tool.inc');

/// @TODO
/// - Cut and paste
/// - Explore
/// - Multiple linguages
/// - Navigation widget

class PageDirectory extends Control{
	private $parent;
	private $page;
	private $configuration;
	private $directoryItem;
	
	function PageDirectory(){
		$this->tableName = 'directory';
		$this->moduleName = 'directory';
		$this->moduleTitle = tt('Directory');
		$this->setID();
		$this->setParent();
		$this->setAccess();
		$this->dropRedirect = 'directory_list';
		$this->init(new PageDirectoryModel($this->parent));
	}
	
	public function index(){
		include_once(INCLUDE_PATH.'tool/menu.tool.inc');
		head_add_css('directory.css');
		$this->selectByID($this->column);
		$this->translateDataByID();
		$this->parent = $this->dataByID['parent'];
		$this->configuration = unserialize($this->dataByID['configuration']);
		$this->configuration = translate_content($this->configuration, 'directory_config', $this->id);
		$render = $this->renderIconBar();
		$render.= $this->render();
		if(strlen($this->dataByID['uri'])) $uri = '/'.$this->dataByID['uri'].'.html';
		else $uri = '';
		menu_activate('directory/'.mid().$uri);
		directory_set_path($this->parent);
		document_set_path($this->dataByID['name']);
		document_set_title($this->dataByID['name']);
		return $render;
	}
	
	public function write(){
		if($this->isWritable()){
			$this->setConfigForm();
			$this->writeLabel = tt('Create Sub-Directory');
			directory_set_path($this->parent);
			return Control::write();
		}
	}
	
	public function edit(){
		$this->setConfigForm();
		directory_set_path(mid());
		return Control::edit();
	}
	
	public function insert(){
		if($this->isWritable()){
			Control::insert();
			$this->configuration = directory_default_configuration();
			$this->createIndexPage(pp('name'), pp('description'), pp('icon'));
			document_set_redirect('directory/'.$this->id);
			$this->insertIntoParentList();
		}
	}
	
	public function update(){
		Control::update();
		document_set_redirect('directory/'.$this->id);
	}
	
	public function drop(){
		if($this->isDropable()){
			Control::drop();
		}else{
			notify_add_error(tt('Directory id no empty and can not be deleted.'));
			execute_set_db();
			document_redirect($this->dropRedirection);
		}
	}
	
	public function listPage(){
		$title = tt('List of Directory for Contributor');
		document_set_path($title);
		document_set_title($title);
		$p = tt('You as Editor can use this page to access the entries of %s in Section.');
		$this->listMeta->description = sprintf($p, $this->moduleTitle);
		$this->listMeta->mode = 'directory_list';
		return $this->renderListPage('WHERE (owner='.USER_ID.')');
	}
	
	public function editorListPage(){
		$this->modelConnector->setAttribute('owner', 'isList', true);
		$title = tt('List of Directory for Administrator');
		document_set_path($title);
		document_set_title($title);
		$p = tt('In the following list, your own entries of %s are shown. You can use this page to access them.');
		$this->listMeta->description = sprintf($p, $this->moduleTitle);
		$this->listMeta->mode = 'directory_editor_list';
		return $this->renderListPage('WHERE (section='.SECTION.')');
	}
	
	public function initModule(){
		Control::initModule('PageDirectory.class.inc');
		$this->initPage();
		$this->page->initModule('Page.class.inc');
		$category = directory_get_page_category_id();
		$this->id = directory_create_root($category);
		$this->configuration = directory_default_configuration();
		if($this->id) $this->createIndexPage('ROOT Directory');
		return $this->page;
	}
	
	protected function renderListPage($clause){
		$this->listMeta->title = tt('List of Directory');
		$this->listMeta->defaultOrderDesc = 'id';
		$this->selectListPage(20, $clause);
		$this->renderOwner();
		$this->setListEditIcon();
		$this->setListDropIcon();
		return Control::renderListPage();
	}
	
	private function setParent(){
		if(pp('parent')) $this->parent = (int) pp('parent');
		elseif(gg('parent')) $this->parent = (int) gg('parent');
		else $this->parent = 0;
	}
	
	private function setID(){
		if(mid()){
			$this->id = mid();
		}elseif(db_exist_table($this->tableName)){
			$clause = 'WHERE parent=0 AND section='.SECTION.' ORDER BY id LIMIT 1';
			$id = db_select_single_column($this->tableName, 'id', $clause, false);
			if(isset($id[0])) $this->id = $id[0];
		}
	}
	
	private function setAccess(){
		$this->ownerColumn = 'owner';
		$this->setAdmin('directory_admin');
		
		$this->addAction('directory_list', 'listPage');
		$this->addAction('directory_editor_list', 'editorListPage');
		
		$this->addAccessMethod('directory_contributor', 'write');
		$this->addAccessMethod('directory_contributor', 'insert');
		$this->addAccessMethod('guest', 'index');
	}
	
	private function createIndexPage($directoryName, $introduction=null, $icon=null){
		if(pp('category')) $category = pp('category');
		else $category = directory_get_page_category_id();
		$pid = directory_create_index_page($this->id, $category, $directoryName, $introduction, $icon);
		$this->configuration['show_page'][] = $pid;
		$dataArray = array(
			'index_page' => $pid,
			'configuration' => serialize($this->configuration),
		);
		db_update($this->tableName, $dataArray, $this->id);
	}
	
	private function isWritable(){
		if($this->parent){
			$pid = $this->parent;
			$parentData = db_select($this->tableName, array('permission', 'owner'), 'WHERE id='.$pid);
			$group = unserialize($parentData[0]['permission']);
			$oid = $parentData[0]['owner'];
			if(authority_group('directory_admin') or authority(array($oid), $group)) return true;
			else return false;
		}else{
			return $this->isCallable('write');
		}
	}
	
	private function isDropable(){
		$clause = 'WHERE parent='.$this->id.' LIMIT 1';
		$countPage = db_select_single_column('page', 'id', $clause);
		$countSubDirectory = db_select_single_column($this->tableName, 'id', $clause);
		$countDirectory = db_select_single_column($this->tableName, 'id', 'WHERE section='.SECTION.' LIMIT 1');
		return (count($countDirectory) != 0 and count($countSubDirectory) == 0 and count($countPage) == 0);
	}
	
	private function initPage(){
		if($this->page == null){
			include_once(MODULE_PATH.'page/Page.class.inc');
			$this->page = new Page();
			$this->page->setParent($this->id);
			$this->page->setConfiguration($this->configuration);
		}
	}
	
	private function renderIconBar(){
		$icon = array();
		if($this->isWritable()){
			$this->initPage();
			$icon[] = render_icon('directory_write/parent/'.$this->parent, 'files/icon/add.png', tt('sibling'));
			$icon[] = render_icon('directory_write/parent/'.$this->id, 'files/icon/add.png', tt('sub-directory'));
			if($this->page->isCallable('write')){
				$icon[] = render_icon('page_write/parent/'.$this->id, 'files/icon/add.png', tt('page'));
			}
		}
		if($this->isCallable('edit')){
			$icon[] = render_icon('directory_config/'.$this->id, 'files/icon/config.png', tt('configuration'));
			$icon[] = render_icon('directory_edit/'.$this->id, 'files/icon/edit.png', tt('edit'));
		}
		if($this->isCallable('drop') and $this->isDropable()){
			$icon[] = render_icon('directory_drop/'.$this->id, 'files/icon/drop.png', tt('drop'));
		}
		if(count($icon)){
			head_add_css('directory_edit.css');
			head_add_js('module/directory/theme/directory_edit.js');
			return render(array('icon' => $icon), 'directory_icon_bar.tpl', 'directory_icon_bar');
		}
	}
	
	private function render(){
		$methodMap = array(
			'index_page' => 'renderIndexPage',
			'description' => 'renderDescription',
			'page_list' => 'renderPageList',
			'page_description' => 'renderPageDescription',
			'subdirectory_list' => 'renderSubDirectoryList',
			'subdirectory_description' => 'renderSubDirectoryDescription',
			'extension' => 'renderExtension',
		);
		$showItem = $this->configuration['show_item'];
		
		$pre = true;
		$preIndex = '';
		$postIndex = '';
		$index = '';
		foreach($showItem as $item){
			if(isset($methodMap[$item])){
				$methodName = $methodMap[$item];
				if($item == 'index_page') $index = $this->$methodName();
				elseif(strlen($index)) $postIndex .= $this->$methodName();
				else $preIndex .= $this->$methodName();
			}
		}
		if(strlen($index)){
			$render = str_replace('<!--{page_pre_index}-->', $preIndex, $index);
			$render = str_replace('<!--{page_post_index}-->', $postIndex, $render);
		}else{
			$render = '<h3 class="title_label">'.$this->dataByID['name'].'</h3>';
			$render.= $preIndex.$postIndex;
		}
		return $render;
	}
	
	private function renderIndexPage(){
		$this->initPage();
		$this->page->id = $this->dataByID['index_page'];
		$this->page->selectByID($this->page->column);
		return $this->page->render();
	}
	
	private function renderPageList(){
		$this->initPage();
		return $this->page->renderItemList();
	}
	
	private function renderPageDescription(){
		$this->initPage();
		return $this->page->renderDescription();
	}
	
	private function renderSubDirectoryList(){
		$this->getDirectoryItem();
		if(count($this->directoryItem)){
			$renderArray = array(
				'label' => $this->configuration['directory_list_label'],
				'item' =>$this->directoryItem
			);
			return render($renderArray, 'directory_item_list.tpl', 'directory_item_list');
		}
	}
	
	private function renderDescription(){
		$renderArray = array(
			'item' => $this->data,
			'is_label' => false,
		);
		return render($renderArray, 'directory_description.tpl', 'directory_description');
	}
	
	private function renderSubDirectoryDescription(){
		$this->getDirectoryItem();
		$renderArray = array(
			'item' => $this->directoryItem,
			'is_label' => true,
		);
		return render($renderArray, 'directory_description.tpl', 'directory_description');
	}
	
	private function renderExtension(){
		$mode = $this->configuration['extension_mode'];
		if(strlen($mode)){
			$renderArray = array(
				'extension' => execute_string($mode),
			);
			return render($renderArray, 'directory_extension.tpl', 'directory_extension');
		}
	}
	
	private function getDirectoryItem(){
		$directoryID = $this->configuration['show_directory'];
		if(count($directoryID) and !is_array($this->directoryItem)){
			$clause = 'WHERE parent='.$this->id.' AND (id='.join(' OR id=', $directoryID).')';
			$childDirectory = db_select($this->tableName, array('id', 'name', 'uri', 'icon', 'description'), $clause);
			$this->directoryItem = array();
			foreach($directoryID as $id){
				$this->directoryItem[$id] = $childDirectory[$id];
			}
		}
		if(!is_array($this->directoryItem)){
			$this->directoryItem = array();
		}
	}
	
	private function renderOwner(){
		foreach($this->data as $key => $value){
			if(isset($value['owner'])){
				$this->data[$key]['owner'] = render_profile_link($value['owner']);
			}else{
				break;
			}
		}
	}
	
	private function setConfigForm(){
		$permission = $this->getNewConfigForm(tt('permission'));
		$permission->append('permission');
	}
	
	private function insertIntoParentList(){
		if($this->parent){
			$parentData = db_select_single_column($this->tableName, 'configuration', 'WHERE id='.$this->parent);
			$configuration = unserialize($parentData[$this->parent]);
			$configuration['show_directory'][] = $this->id;
			db_update($this->tableName, array('configuration' => serialize($configuration)), $this->parent);
		}
	}
}
?>
