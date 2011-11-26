<?php
class ContactFormModel extends Model{
	function ContactFormModel($contactID, $enableAttachment){
		$this->email = $this->email(tt('your email address'));
		$this->subject = $this->title(tt('subject'));
		if($enableAttachment){
			$this->attachment = $this->file('attachment', 'files/contact/');
			$this->attachment->help = sprintf(tt('maximum file size : %s'), render_file_size(MAX_UPLOADED_FILE_SIZE));
		}
		$this->message = $this->shortText(tt('message'));
		$this->message->isNotNull = true;
		$this->id = $this->hidden($contactID, 'int');
	}
}
?>
