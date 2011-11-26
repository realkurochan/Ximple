<?php
require('ProfileTypeColumn.class.inc');

class ProfileModel extends Model{
	function ProfileModel(){
		$this->owner = $this->hidden(USER_ID, 'int');
		$this->profiletype = new ProfileTypeColumn();
		$this->profile = $this->shortText(tt('profile'));
	}
}
?>
