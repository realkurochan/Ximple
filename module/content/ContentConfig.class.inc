<?php
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include_once('content.tool.inc');
include_once('ContentModel.class.inc');
include_once('ContentConfigModel.class.inc');

abstract class ContentConfig extends ConfigControl{
	private $mode;
	
	protected $appearanceFormGroup;
	protected $publishFormGroup;
	protected $commentFormGroup;
	
	public function save(){
		$this->executeExtension();
		ConfigControl::save();
	}
	
	protected function groupConfigForm(){
		$this->appearanceFormGroup = $this->getNewConfigForm(tt('configure appearance'));
		$this->appearanceFormGroup->setShowByDefault(true);
		$this->appearanceFormGroup->append($this->mode.'_appearance');
		$this->appearanceFormGroup->append($this->mode.'_comment_write_label');
		$this->appearanceFormGroup->append($this->mode.'_comment_page_length');
		
		$this->publishFormGroup = $this->getNewConfigForm(tt('configure publishing status'));
		$this->publishFormGroup->append($this->mode.'_publishing_status');
		$this->publishFormGroup->append($this->mode.'_publishing_status_default');
		
		$this->commentFormGroup = $this->getNewConfigForm(tt('configure comment permission'));
		$this->commentFormGroup->append($this->mode.'_comment_permission');
		$this->commentFormGroup->append($this->mode.'_comment_permission_default');
		$this->executeExtension();
	}
	
	protected function setMode($mode){
		$this->mode = $mode;
	}
	
	protected function setConfig($key, $value){	
		$comment_permission = $this->mode.'_comment_permission';
		$comment_permission_default = $this->mode.'_comment_permission_default';
		$publishing_status = $this->mode.'_publishing_status';
		$publishing_status_default = $this->mode.'_publishing_status_default';
		if($key == $comment_permission){
			$avaiable = pp($comment_permission);
			if(!is_array($avaiable) or count($avaiable) == 0) $value = array(0, 1, 2, 3);
		}elseif($key == $publishing_status){
			$avaiable = pp($publishing_status);
			if(!is_array($avaiable) or count($avaiable) == 0) $value = array(2, 3);
		}elseif($key == $comment_permission_default){
			$avaiable = pp($comment_permission);
			if(!is_array($avaiable) or count($avaiable) == 0) $value = 0;
			elseif(!in_array($value, $avaiable)) $value = $avaiable[0];
		}elseif($key == $publishing_status_default){
			$avaiable = pp($publishing_status);
			if(!is_array($avaiable) or count($avaiable) == 0) $value = 3;
			elseif(!in_array($value, $avaiable)) $value = $avaiable[0];
		}
		ConfigControl::setConfig($key, $value);
	}
	
	protected function getConfig(){
		if($this->mode == null and state_is_develop()){
			notify_add_error('Mode of configuration is not set. Use ContentConfig::setMode() to set it.');
		}
		return cc();
	}
	
	private function executeExtension(){
		$additionalConfig = $this->getAdditionalConfig();
		foreach($additionalConfig as $label => $models){
			$configForm = $this->getNewConfigForm($label);
			foreach($models as $key => $model){
				if($this->checkModelClass($key, $model)){
					$name = $this->mode.'_'.$key;
					$this->modelConnector->addModel($name, $model);
					$configForm->append($name);
				}
			}
		}
	}
	
	private function checkModelClass($name, $model){
		if(is_a($model, 'ModelColumn'))return true;
		elseif(state_is_develop()) notify_add_error(sprintf(tt('%s is not an instance of ModelColumn.'), $name)); 
		return false;
	}
	
	private function getAdditionalConfig(){
		$additionalConfig = array('module' => $this->mode);
		$additionalConfig = extension_execute('content_config', $additionalConfig);
		unset($additionalConfig['module']);
		return $additionalConfig;
	}
}
?>
