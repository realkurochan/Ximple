<?php
require('UserLoginHashColumn.class.inc');
require('UserPasswordColumn.class.inc');
require('AvatarColumn.class.inc');

class UserModel extends Model{
	function UserModel(){
		$help = tt('Login name must be longer than 4 characters and unique.');
		$this->login_name = $this->title(tt('login name'), $help);
		$this->login_name->listMode = 'profile';
		$this->email = $this->email(tt('email'), tt('Email must be unique.'));
		$this->email->isList = true;
		$this->email->isReachable = true;
		$this->password = new UserPasswordColumn();
		$this->confirm_password = $this->password(tt('confirm password'));
		$this->confirm_password->isDB = false;
		$this->status = $this->hidden('active');
		$this->avatar = new AvatarColumn();
		$this->section = $this->section();
		$this->space_used = $this->fileSize(tt('used space'));
		$this->login_name_hash = new UserLoginHashColumn();
	}
}
?>
