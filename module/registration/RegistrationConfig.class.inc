<?php
execute_add_class('registration', 'RegistrationConfig');
include(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('RegistrationConfigModel.class.inc');

class RegistrationConfig extends ConfigControl{
	function RegistrationConfig(){
		$this->moduleTitle = tt('Registration Configuration');
		$this->addAction('registration_config', 'index');
		$this->addAction('registration_config_save', 'save');
		$this->setAdmin('user_admin');
		$this->init(new RegistrationConfigModel());
		$this->saveRedirection = 'registration_config';
	}
	
	function index(){
		document_set_path(tt('Registration'), 'registration');
		$this->formMeta->id = 'registration_config';
		$this->formMeta->title = $this->moduleTitle;
		$this->formMeta->mode = 'registration_config_save';
		return $this->render();
	}
	
	function save(){
		if(pp('registration_enable')){
			$navigation = cc('navigation');
			if(!isset($navigation['registration'])){
				config_append_assc('navigation', 'registration', tt('Registration'));
				config_append('navigation_enabled', 'registration');
			}
		}else{
			config_drop_assc('navigation', 'registration');
			config_drop_by_value('navigation_enabled', 'registration');
		}
		ConfigControl::save();
	}
}

?>
