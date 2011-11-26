<?php
require_once(INCLUDE_PATH.'class/column/PasswordColumn.class.inc');

class UserPasswordColumn extends PasswordColumn{
	function UserPasswordColumn(){
		CharColumn::CharColumn(tt('password'));
		$this->setFormAttribute('type', 'password');
		$this->isDB = true;
	}
	
	function triggerInsertForm(){
		$this->help = tt('Password must be longer than 8 characters');
		$this->isNotNull = true;
	}
	
	function triggerUpdateForm(){
		$this->help = tt('If empty, password will not be changed.');
		$this->help.= tt('Else, password must be longer than 8 characters');
		$this->data = '';
		$this->isNotNull = false;
	}
	
	function triggerPreUpdate(){
		$this->isDB = (strlen(pp('password')) > 0);
	}
}
?>
