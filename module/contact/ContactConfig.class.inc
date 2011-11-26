<?php
execute_add_extended_class('contact', 'config', 'ContactConfig');
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('ContactConfigModel.class.inc');

class ContactConfig extends ConfigControl{
	function ContactConfig(){
		$this->moduleName = 'contact';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('Contact Configuration');
		$this->setAdmin('contact_admin');
		$this->init(new ContactConfigModel());
	}
	
	public function index(){
		document_set_path(tt('Contact'), 'contact');
		$this->formMeta->mode = 'contact_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
}
?>
