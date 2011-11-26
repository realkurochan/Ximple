<?php
class RegistrationConfigModel extends Model{
	function RegistrationConfigModel(){
		$this->registration_enable = $this->enable(tt('registration enable'));
		$this->registration_login_page = $this->shorttext(tt('text in login page'));
		$this->registration_page = $this->shorttext(tt('text in registration page'));
		$this->registration_reply = $this->shorttext(tt('auto reply text'));
	}
}
?>
