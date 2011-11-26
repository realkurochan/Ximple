<?php
execute_add_extended_class('activity', 'config', 'ActivityConfig');
include_once(MODULE_PATH.'content/ContentConfig.class.inc');
include_once('ActivityConfigModel.class.inc');

class ActivityConfig extends ContentConfig{
	function ActivityConfig(){
		$this->moduleName = 'activity';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('Activity Configuration');
		$this->setAdmin('activity_admin');
		$this->init(new ActivityConfigModel());
		$this->setMode('activity');
	}
	
	public function index(){
		document_set_path(tt('Activity'), 'activity');
		$this->groupConfigForm();
		$this->appearanceFormGroup->append('activity_icon_size');
		$this->formMeta->mode = 'activity_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
	
	public function getConfig(){
	
	}
}
?>
