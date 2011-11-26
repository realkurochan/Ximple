<?php
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('ContactFormModel.class.inc');

class ContactForm extends ConfigControl{
	function ContactForm($contactID, $contactName, $enableAttachment){
		$this->moduleTitle = $contactName;
		$this->init(new ContactFormModel($contactID, $enableAttachment));
		$this->formMeta->isCaptcha = true;
	}
	
	public function render($description){
		$this->formMeta->id = 'contact';
		$this->formMeta->description = $description;
		$this->formMeta->mode = 'contact_send';
		$this->formMeta->title = $this->moduleTitle;
		return ConfigControl::render();
	}
	
	protected function getConfig(){
	}
}
?>
