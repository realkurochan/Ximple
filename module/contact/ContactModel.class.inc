<?php
class ContactModel extends Model{
	function ContactModel(){
		$this->name = $this->title(tt('contact name'));
		$this->name->listMode = 'contact';
		$this->name->enableTranslate();
		
		$this->permission = $this->group(tt('allowed group'));
		$this->permission->help = tt('Check guest to allow everybody.');
		
		$this->enable_attachment = $this->enable(tt('enable attachment'));
		
		$this->email_list = $this->plain(tt('email list'));
		$this->email_list->help = tt('Use comma for multiple email addresses. ');
		$this->email_list->help.= tt('You can get email from visitor from the addresses.');
		
		$this->description = $this->shortText(tt('contact description'));
		$this->description->enableTranslate();
		$this->description->help = tt('Description text will be shown on the Contact page.');
		$this->description->help.= tt('You can write addition contact information e.g. telephone number here.');
		
		$this->reply_message = $this->shortText(tt('reply message'));
		$this->reply_message->help = tt('Reply message to visitor, who want to contact you.');
		$this->reply_message->enableTranslate();
		
		$this->section = $this->section();
	}
}
?>
