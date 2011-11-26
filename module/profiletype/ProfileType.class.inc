<?php
execute_add_class('profiletype', 'ProfileType');
include('ProfileTypeModel.class.inc');

class ProfileType extends Control{
	function ProfileType(){
		$this->tableName = 'profiletype';
		$this->moduleName = 'profiletype';
		$this->moduleTitle = 'Profile-Type';
		$this->setAdmin('profile_admin');
		$this->init(new ProfileTypeModel());
		document_set_path(tt('User'), 'user');
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	function index(){
		$this->listMeta->title = sprintf(tt('List of %s'), $this->moduleTitle);
		$description  = tt('You can use this page to manage Profile-Type in the Section. ');
		$description .= tt('The Profile of Users can be extent using Profile-Type. ');
		$description .= tt('This Profile extention is flexible and suitable for database of human resources.');
		$this->listMeta->description = $description;
		$this->listMeta->mode = 'profiletype';
		$this->listMeta->defaultOrder = 'id';
		$this->setListWriteIcon(tt('Add a new Profile-Type'));
		$this->setListEditIcon();
		$this->setListDropIcon();
		$this->selectListPage(10, "WHERE (section=".SECTION.')');
		return $this->renderListPage();
	}
	
	function drop(){
		Control::drop();
		$pid = (int)gg('id');
		if(!empty($pid)) db_drop_many('profile', 'profiletype='.$pid);
	}
}
?>
