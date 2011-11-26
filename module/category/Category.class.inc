<?php
execute_add_class('category', 'Category');
include('CategoryModel.class.inc');

class Category extends Control{
	private $mode;
	private $modeName;
	private $indexMode;
	private $checkRequirement = true;
	private $translateMode = array(
		'category_translate_form',
		'category_translate'
	);
	
	function Category(){
		$this->tableName = 'category';
		$this->moduleName = 'category';
		$this->moduleTitle = tt('Category');
		$this->inserRedirect  = 'category/module/'.$this->getMode();
		$this->updateRedirect = 'category/module/'.$this->getMode();
		$this->init(new CategoryModel(gg('module')));
	}
	
	public function index(){
		$this->setListMeta();
		$this->selectListPage(5, 'WHERE (section='.SECTION." and mode='".$this->mode."')");
		return $this->renderListPage();
	}
	
	public function insert(){
		Control::insert();
		extension_execute('category_insert', $this->data);
	}
	
	public function update(){
		Control::update();
		extension_execute('category_update', $this->data);
	}
	
	public function isCallable($methodName){
		$this->getMode();
		$this->indexMode = 'category/module/'.$this->mode;
		$this->setAdmin($this->mode.'_admin');
		$this->uriMode = 'category/module/'.$this->mode;
		if(!in_array(MODE, $this->translateMode)){
			$this->getModuleName();
			$this->checkRequirement();
		}
		$this->documentSetPath();
		$this->insertRedirection = $this->indexMode;
		$this->updateRedirection = $this->indexMode;
		return Authority::isCallable($methodName);
	}
	
	public function renderTranslateForm(){
		$this->checkRequirement = false;
		return Control::renderTranslateForm();
	}
	
	public function setTranslate(){
		$this->checkRequirement = false;
		Control::setTranslate();
	}
	
	private function setListMeta(){
		$this->listMeta->title = sprintf(tt('List of %s for %s'), $this->moduleTitle, $this->modeName);
		$this->listMeta->defaultOrder = 'id';
		$this->listMeta->mode = $this->indexMode;
		$writeMode = 'category_write/module/'.$this->mode;
		$this->setListWriteIcon(sprintf(tt('Add new Category for %s'), $this->modeName), $writeMode);
		$this->setListEditIcon();
	}
	
	private function getModuleName(){
		$moduleName = cc('module_name');
		$this->modeName = $moduleName[$this->mode];
	}
	
	private function checkRequirement(){
		$mode_require = cc('module_require');
		if($this->checkRequirement and !isset($mode_require[$this->mode])){
			error_syslog('The called mode for category does not exist.');
		}
	}
	
	private function documentSetPath(){
		document_set_path($this->modeName, $this->mode);
		document_set_path($this->moduleTitle, $this->indexMode);
		document_set_title($this->moduleTitle);
	}
	
	private function getMode(){
		$this->mode = gg('module');
		if(!$this->mode) $this->mode = pp('mode');
		if(!$this->mode) $this->mode = $this->dataByID['mode'];
		return $this->mode;
	}
}
?>
