<?php
execute_add_extended_class('develcard', 'config', 'DevelCardConfig');
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include_once('develcard.tool.inc');
include('DevelCardConfigModel.class.inc');

class DevelCardConfig extends ConfigControl{
	function DevelCardConfig(){
		$this->moduleName = 'develcard';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('DevelCard Configuration');
		$this->setAdmin('develcard_admin');
		$this->init(new DevelCardConfigModel());
	}
	
	public function index(){
		document_set_path(tt('DevelCard'), 'develcard');
		$this->formMeta->mode = 'develcard_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
	
	public function save(){
		$type = pp('develcard_type');
		foreach($type as $key => $value){
			if(strlen($value) == 0) unset($type[$key]);
		}
		config_set('develcard_type', $type);
		ConfigControl::save();
	}
}
?>
