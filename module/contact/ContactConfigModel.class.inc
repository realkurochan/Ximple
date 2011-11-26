<?php
class ContactConfigModel extends Model{
	function ContactConfigModel(){
		$this->contact_head = $this->title(tt('header subject for contact message'));
		$this->contact_subject = $this->title(tt('subject for reply message'));
		$this->contact_message_receiver_group = $this->group(tt('message receiver groups by sending contact'));
		$this->contact_message_receiver_group->isExceptedGuest = true;
		$this->contact_message_receiver_group->isExceptedAdmin = false;
	}
}
?>
