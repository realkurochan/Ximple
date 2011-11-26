<?php
include_once('Translatable.class.inc');
include_once('Model.class.inc');
include_once('ModelFactory.class.inc');
include_once('ModelConnector.class.inc');
include_once('FormMetaData.class.inc');

abstract class ConfigControl extends Translatable{
	private $formRender;
	
	protected $saveRedirection;
	protected $isSectionConfig = true;
	
	protected function init($modelObj){
		Translatable::init($modelObj);
		$modelFactory = new ModelFactory($modelObj);
		$this->modelConnector = $modelFactory->bind($this);
		$this->formMeta = new FormMetaData();
		$this->addAction($this->fullModuleName, 'index');
		$this->addAction($this->fullModuleName.'_save', 'save');
		$this->addAction($this->fullModuleName.'_translate_form', 'renderTranslateForm');
		$this->addAction($this->fullModuleName.'_translate', 'setTranslate');
		if(!$this->saveRedirection){
			$this->saveRedirection = $this->fullModuleName;
		}
		$this->modelConnector->setModuleName($this->fullModuleName);
		$this->modelConnector->setID(MODE_ID);
	}
	
	public function initValue(){
		$defaultData = $this->modelConnector->getColumnAttribute('defaultData');
		foreach($defaultData as $name => $data){
			$this->setConfig($name, $data);
		}
		$this->saveConfig();
	}
	
	public function render($setPath=true){
		head_add_js_var('translateModuleName', $this->fullModuleName);
		if(!$this->formRender){
			include_once('FormRender.class.inc');
			$this->formRender = new FormRender($this->modelConnector, $this->formMeta);
		}
		$this->formMeta->configForm = $this->configForm;
		document_set_title($this->moduleTitle);
		if($setPath){
			document_set_path($this->moduleTitle, $this->fullModuleName);
		}
		return $this->formRender->renderUpdateForm($this->getConfig());
	}
	
	public function save(){
		$configName = $this->modelConnector->getColumnNameExcepted(array('id'));
		$type = $this->modelConnector->getPHPType();
		foreach($configName as $name){
			$configValue = pp($name);
			settype($configValue, $type[$name]);
			$this->setConfig($name, $configValue);
		}
		$this->saveConfig();
		execute_set_db();
		document_set_redirect($this->saveRedirection);
		notify_add_info(tt('Configurations have been successfully saved.'));
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
	
	public function initModule($requirement){
		if(strlen($this->moduleExtension)){
			module_add($this->moduleName, $requirement, null, $this->moduleExtension);
		}else{
			module_add($this->moduleName, $requirement, $this->moduleTitle, '#primary');
		}
		if($this->isSectionConfig){
			$default = $this->modelConnector->getColumnAttribute('defaultData');
			foreach($default as $key => $value){
				config_set($key, $value);
			}
		}
	}
	
	public function dropModule(){
		module_drop($this->moduleName);
	}
	
	protected function getConfig(){
		return ucc();
	}
	
	protected function setConfig($key, $value){
		config_set($key, $value);
	}
	
	protected function saveConfig(){
		config_save();
	}
}

?>
