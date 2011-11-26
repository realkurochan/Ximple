<?php

include_once('Translatable.class.inc');
include_once('Model.class.inc');
include_once('ModelFactory.class.inc');
include_once('ModelConnector.class.inc');
include_once('FormMetaData.class.inc');
include_once('ListPageMetaData.class.inc');

abstract class Control extends Translatable{
	private $tableCreator;
	private $listPage;
	private $modelHasCategory;
	
	protected $data;
	protected $dataByID;
	protected $listMeta;
	
	protected $ownerColumn;
	protected $dataModifier;
	protected $pager;

	protected $formRender;
		
	protected $insertRedirect;
	protected $updateRedirect;
	protected $dropRedirect;
	protected $redirectToIDPage = false;
	
	protected $writeLabel;
	protected $editLabel;
	
	protected $insertNotify;
	protected $updateNotify;
	protected $dropNotify;
	
	protected $prepareMethodMap = array();
	
	protected $isOwnerMode = false;
	
	public $id;
	public $column;
	public $listPageColumn;
	public $tableName;
	
	public function index(){
		return $this->listPage();
	}
	
	public function listPage(){
		if(!$this->modelConnector->hasSection()) $clause = '';
		else $clause = 'WHERE (section='.SECTION.')';
		$this->selectListPage(20, $clause);
		return $this->renderDefaultListPage();
	}
	
	public function write(){
		$this->formMeta->id = $this->fullModuleName;
		if($this->writeLabel) $this->formMeta->title = $this->writeLabel;
		else $this->formMeta->title = sprintf(tt('Add %s'), $this->moduleTitle);
		if(empty($this->formMeta->mode)){
			$this->formMeta->mode = $this->fullModuleName.'_insert';
		}
		$render = $this->renderInsertForm();
		document_set_path($this->formMeta->title);
		return $render;
	}
	
	public function edit(){
		$this->formMeta->id = $this->fullModuleName;
		head_add_js_var('translateModuleName', $this->fullModuleName);
		if($this->editLabel) $this->formMeta->title = $this->editLabel;
		else $this->formMeta->title = sprintf(tt('Edit %s'), $this->moduleTitle);
		if(empty($this->formMeta->mode)){
			$this->formMeta->mode = $this->fullModuleName.'_update';
		}
		$render = $this->renderUpdateForm();
		document_set_path($this->formMeta->title);
		return $render;
	}
	
	public function insert(){
		$this->initDataModifier();
		$this->dataModifier->prepareInsert();
		$insertOk = $this->dataModifier->insert($this->formMeta->isCaptcha);
		if($insertOk){
			$this->id = $this->dataModifier->getID();
			$this->setRedirect($this->insertRedirect);
			notify_add_info($this->insertNotify);
			return true;
		}else{
			return false;
		}
	}
	
	public function update(){
		$this->initDataModifier();
		$this->dataModifier->prepareUpdate($this->dataByID);
		$updateOk = $this->dataModifier->update($this->formMeta->isCaptcha);
		if($updateOk){
			$this->setRedirect($this->updateRedirect);
			notify_add_info($this->updateNotify);
		}
	}
	
	public function drop(){
		$this->initDataModifier();
		$this->dataModifier->prepareDrop();
		$this->dataModifier->drop();
		document_set_redirect($this->dropRedirect);
		notify_add_info($this->dropNotify);
	}
	
	public function prepareMethod($methodName){
		if(isset($this->prepareMethodMap[$methodName])){
			$prepare = $this->prepareMethodMap[$methodName];
			$this->$prepare();
		}
	}
	
	public function prepareModifyDB(){
		$this->id = (int)gg('id');
		if(empty($this->id)) error_syslog(tt('Data can not be prepared without any given ID.'));
		$this->selectByID($this->column, true);
		if(!count($this->dataByID)) error_syslog(tt('The selected data is empty.'));
		$this->setOwnerByID();
		$this->modelConnector->setData($this->dataByID);
	}
	
	public function initModule($requirement){
		if(!$this->tableCreator){
			include_once('TableCreator.class.inc');
			$this->tableCreator = new TableCreator($this->modelConnector, $this->tableName);
			if($this->modelHasCategory){
				require_once(INCLUDE_PATH.'tool/category.tool.inc');
				category_init($this->fullModuleName);
			}
		}
		$this->tableCreator->create();
		if(strlen($this->moduleExtension)){
			module_add($this->moduleName, $requirement, null, $this->moduleExtension);
		}else{
			module_add($this->moduleName, $requirement, $this->moduleTitle, '#primary');
		}
	}
	
	public function dropModule(){
		module_drop($this->moduleName);
	}
	
	public function select($column, $clause=null, $isRaw=false){
		$dbColumn = $this->checkDBColumn($column);
		$this->data = db_select($this->tableName, $dbColumn, $clause);
		$this->unserializeData();
		if(!$isRaw) $this->postSelect($column);
	}
	
	public function selectSingleColumn($column, $clause=null, $isRaw=false){
		$columnArray = $this->checkDBColumn(array($column));
		$this->select(array('id', $column), $clause, $isRaw);
		$data = array();
		foreach($this->data as $key => $value){
			$data[$key] = $value[$column];
		}
		return $data;
	}
	
	public function selectByID($column, $isRaw=false){
		$dbColumn = $this->checkDBColumn($column);
		$this->modelConnector->setID($this->id);
		$this->data = db_select($this->tableName, $dbColumn, 'WHERE id='.(int)$this->id);
		$this->unserializeData();
		if(!$isRaw) $this->postSelect($column);
		if(isset($this->data[$this->id])){
			$this->dataByID = $this->data[$this->id];
		}else{
			error_not_found();
		}
		$this->setOwnerByID();
	}
	
	public function selectBySection($column, $isRaw=false){
		if($this->modelConnector->hasSection()){
			$dbColumn = $this->checkDBColumn($column);
			$this->data = db_select($this->tableName, $dbColumn, 'WHERE section='.SECTION);
			$this->unserializeData();
			if(!$isRaw) $this->postSelect($column);
		}elseif(state_is_develop()){
			notify_add_error(sprintf(tt('%s does not have SECTION (selectBySection).'), $this->fullModuleName));
		}
	}
	
	public function selectPage($column, $pageLength, $clause=null, $isRaw=false, $page=MODE_PAGE){
		$dbColumn = $this->checkDBColumn($column);
		$this->listPageColumn = $dbColumn;
		$rowsNumber = db_get_rows_number($this->tableName, $clause);
		if(!$this->pager){
			include_once('PagerRender.class.inc');
			$this->pager = new PagerRender();
		}
		$this->pager->setPageLength($pageLength);
		$this->pager->setRowsNumber($rowsNumber);
		
		$clause .= ' LIMIT '.(int)$pageLength.' OFFSET '.(int)($page*$pageLength);
		$this->data = db_select($this->tableName, $dbColumn, $clause);
		$this->unserializeData();
		if(!$isRaw) $this->postSelect($column);
	}
	
	public function selectListPage($pageLength=20, $where=null, $additionColumn=array(), $isRaw=false){
		$this->getListColumn($additionColumn);
		$this->initListPage();
		$this->selectPage($this->listPageColumn, $pageLength, $this->listPage->getClause($where), $isRaw);
	}
	
	public function createTable(){
		if(!$this->tableCreator){
			include_once('TableCreator.class.inc');
			$this->tableCreator = new TableCreator($this->modelConnector, $this->tableName);
		}
		$this->tableCreator->create();
	}
	
	public function getNewConfigForm($title, $description=null){
		if(!$this->modelConnector and state_is_develop()) {
			$message  = 'You should call Control::createModel';
			$message .= 'before calling Control::initConfigForm().';
			notify_add_error($message);
		}
		include_once('ConfigFormRender.class.inc');
		$configForm = new ConfigFormRender($title, $this->modelConnector, $description);
		$this->configForm[] = $configForm;
		return $configForm;
	}
	
	public function getParentColumnName(){
		return $this->modelConnector->getParentColumnName();
	}
	
	protected function init($modelObj){
		Translatable::init($modelObj);
		$this->modelHasCategory = $modelObj->hasCategory();
		$this->setDefaultRedirect();
		$this->setDefaultAction();
		$this->setDefaultPrepareMethod();
		$this->setDefaultNotify();
		$this->initObject($modelObj);
		$this->modelConnector->triggerSetTableName($this->tableName);
		$this->modelConnector->setModuleName($this->fullModuleName);
	}
	
	protected function setOwnerColumn($ownerColumn){
		$this->ownerColumn = $ownerColumn;
	}
	
	protected function setListWriteIcon($label=null, $mode=null){
		if($this->isCallable('write')){
			$this->checkWriteLabel();
			if(empty($label)) $label = $this->writeLabel;
			if(empty($mode)) $mode = $this->fullModuleName.'_write';
			$this->listMeta->writeLabel = $label;
			$this->listMeta->writeMode = $mode;
		}
	}
	
	protected function setListEditIcon($label=null, $mode=null){
		if($this->isCallable('edit') or $this->isOwnerMode){
			if(empty($label)) $label = tt('edit');
			if(empty($mode)) $mode = $this->fullModuleName.'_edit';
			$this->listMeta->addOperation($mode, $label, 'edit.png');
		}
	}
	
	protected function setListDropIcon($label=null, $mode=null){
		if($this->isCallable('drop') or $this->isOwnerMode){
			if(empty($label)) $label = tt('drop');
			if(empty($mode)) $mode = $this->fullModuleName.'_drop';
			$this->listMeta->addOperation($mode, $label, 'drop.png');
		}
	}
	
	protected function renderDefaultListPage($description=null){
		if(empty($this->listMeta->title)){
			$this->listMeta->title = sprintf(tt('List of %s'), $this->moduleTitle);
		}
		$this->listMeta->mode = $this->fullModuleName;
		$this->listMeta->defaultOrder = 'id';
		$this->listMeta->description = $description;
		$this->setListWriteIcon();
		$this->setListEditIcon();
		$this->setListDropIcon();
		return $this->renderListPage();
	}
	
	protected function renderListPage(){
		$this->initListPage();
		foreach($this->listPageColumn as $key => $value){
			if($value == 'id') unset($this->listPageColumn[$key]);
		}
		$this->listMeta->column = $this->listPageColumn;
		$render = $this->listPage->render($this->data);
		$render.= $this->pager->render($this->listPage->getPageMode());
		$render.= $this->listPage->renderSearchForm();
		return $render;
	}
	
	protected function setPrepareAction($methodName, $prepareMethodName){
		$this->prepareMethodMap[$mode] = $methodName;
	}
	
	protected function renderInsertForm($setPath=true){
		$this->prepareForm($setPath);
		return $this->formRender->renderInsertForm();
	}
	
	protected function renderUpdateForm($setPath=true){
		$this->prepareForm($setPath);
		if(isset($this->data[$this->id])){
			$this->formMeta->mode .= '/'.$this->id.'/page/'.MODE_PAGE;
			return $this->formRender->renderUpdateForm($this->data[$this->id]);
		}elseif(state_is_develop()){
			notify_add_error('No data for update-form rendering.');
		}
	}
	
	protected function postSelect($column){
		$column = $this->checkPostSelectColumn($column);
		$this->data = $this->modelConnector->triggerSelect($this->data, $column);
	}
	
	protected function prepareForm(){
		if(!$this->formRender){
			include_once('FormRender.class.inc');
			$this->formRender = new FormRender($this->modelConnector, $this->formMeta);
		}
		$this->formMeta->configForm = $this->configForm;
		if($this->formMeta->isSetTitle){
			document_set_title($this->formMeta->title);
		}
	}
	
	protected function getListColumn($additionColumn=array()){
		$column = $this->modelConnector->getColumnNameByBoolean('isList');
		$column = array_merge(array('id'), $column);
		$this->listPageColumn = array_merge($column, $additionColumn);
		$this->listPageColumn = array_unique($this->listPageColumn);
	}
	
	protected function renderWriteIcon(){
		if($this->isCallable('write')){
			$this->checkWriteLabel();
			$mode = $this->fullModuleName.'_write';
			return render_write_icon($mode, $this->writeLabel);
		}
	}
		
	private function initDataModifier(){
		if(!$this->dataModifier){
			include_once('DataModifier.class.inc');
			$this->dataModifier = new DataModifier($this->tableName, $this->modelConnector);
		}
		execute_set_db();
	}
	
	private function initListPage(){
		if(!$this->listPage){
			include_once('ListPage.class.inc');
			$this->listPage = new ListPage($this->modelConnector, $this->listMeta);
		}
	}
	
	private function checkDBColumn($columns){
		$dbColumn = $this->modelConnector->getColumnNameByBoolean('isDB');
		$columns = array_intersect($columns, $dbColumn);
		$columns[] = 'id';
		return $columns;
	}
	
	private function checkPostSelectColumn($columns){
		$dbColumn = $this->modelConnector->getColumnNameByBoolean('isDB');
		$linkColumn = $this->modelConnector->getColumnNameByBoolean('isLink');
		$allowedColumn = array_merge($dbColumn, $linkColumn);
		$columns = array_intersect($columns, $allowedColumn);
		$columns[] = 'id';
		return $columns;
	}
	
	private function setDefaultRedirect(){
		if(empty($this->insertRedirect)){
			if($this->redirectToIDPage) $this->insertRedirect = $this->fullModuleName.'/#id';
			else $this->insertRedirect = $this->fullModuleName;
		}
		if(empty($this->updateRedirect)){
			if($this->redirectToIDPage) $this->updateRedirect = $this->fullModuleName.'/#id';
			else $this->updateRedirect = $this->fullModuleName;
		}
		if(empty($this->dropRedirect)){
			$this->dropRedirect = $this->fullModuleName;
		}
	}
	
	private function setDefaultAction(){
		$this->addAction($this->fullModuleName, 'index');
		$this->addAction($this->fullModuleName.'_list', 'listPage');
		$this->addAction($this->fullModuleName.'_write', 'write');
		$this->addAction($this->fullModuleName.'_edit', 'edit');
		$this->addAction($this->fullModuleName.'_insert', 'insert');
		$this->addAction($this->fullModuleName.'_update', 'update');
		$this->addAction($this->fullModuleName.'_drop', 'drop');
		$this->addAction($this->fullModuleName.'_print', 'printPage');
		$this->addAction($this->fullModuleName.'_translate_form', 'renderTranslateForm');
		$this->addAction($this->fullModuleName.'_translate', 'setTranslate');
	}
	
	private function setDefaultPrepareMethod(){
		$this->prepareMethodMap['edit'] = 'prepareModifyDB';
		$this->prepareMethodMap['update'] = 'prepareModifyDB';
		$this->prepareMethodMap['drop'] = 'prepareModifyDB';
	}
	
	private function setDefaultNotify(){
		if(empty($this->insertNotify)){
			$this->insertNotify = sprintf(tt('%s has been successfully inserted.'), $this->moduleTitle);
		}
		if(empty($this->updateNotify)){
			$this->updateNotify = sprintf(tt('%s has been successfully updated.'), $this->moduleTitle);
		}
		if(empty($this->dropNotify)){
			$this->dropNotify = sprintf(tt('%s has been successfully dropped.'), $this->moduleTitle);
		}
	}
	
	private function initObject($modelObj){
		$modelFactory = new ModelFactory($modelObj);
		$this->modelConnector = $modelFactory->bind();
		$this->column = $this->modelConnector->getColumnName();
		$this->formMeta = new FormMetaData();
		$this->listMeta = new ListPageMetaData();
	}
	
	private function setRedirect($redirect){
		if(!is_array($this->id)){
			$redirect = str_replace('#id', $this->id, $this->updateRedirect);
		}
		document_set_redirect($redirect);
	}
	
	private function unserializeData(){
		$columnArray = $this->modelConnector->getColumnNameByBoolean('isSerialize');
		foreach($this->data as $key => $value){
			foreach($columnArray as $column){
				if(isset($this->data[$key][$column])){
					$this->data[$key][$column] = unserialize($value[$column]);
				}
			}
		}
	}
	
	private function setOwnerByID(){
		if($this->id and strlen($this->ownerColumn) and isset($this->dataByID[$this->ownerColumn])){
			$owner = $this->dataByID[$this->ownerColumn];
			if(is_array($owner)) $this->setOwner($owner['id']);
			else $this->setOwner($owner);
		}
	}
	
	private function checkWriteLabel(){
		if(strlen($this->writeLabel) == 0){
			$this->writeLabel = sprintf(tt('Write new %s entry'), $this->moduleTitle);
		}
	}
}
?>
