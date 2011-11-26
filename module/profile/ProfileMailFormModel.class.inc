<?php
class ProfileMailFormModel extends Model{
	function ProfileMailFormModel(){
		if(authority_is_guest()){
			$this->email = $this->email(tt('your email address'));
		}else{
			$this->sender = $this->hidden(USER_ID, 'int');
		}
		$this->subject = $this->title(tt('subject'));
		$this->message = $this->shortText(tt('message'));
		$this->message->isNotNull = true;
		$this->receiver = $this->hidden(gg('id'), 'int');
	}
}
?>
