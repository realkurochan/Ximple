<?php
execute_add_class('group', 'Group');
include('role.module.inc');
include('GroupModel.class.inc');

class Group extends Control{
	private $systemRole;
	private $moduleRole;
	
	function Group(){
		$this->tableName = 'groups';
		$this->moduleName = 'group';
		$this->moduleTitle = 'User-Group';
		$this->setAdmin('group_admin');
		$this->init(new GroupModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	function index(){
		$this->listMeta->title = sprintf(tt('List of %s'), $this->moduleTitle);
		$role_label = tt('manage users and roles in User-Group.');
		$description = tt('You can use this page to manage User-Group in the Section. ');
		$description.= tt('User-Group is the main concept of the permission management in Ximple. ');
		$description.= tt('Every group contains Users and Roles. ');
		$description.= tt('The Users in a group have the same permission, which can be controlled by the contained Roles. ');
		$description.= tt('A User can be in many User-Groups.');
		$description.= '<p><img src="'.ROOT_URI.'files/icon/config.png" /> : '.$role_label.'</p>';
		$this->listMeta->description = $description;
		$this->listMeta->mode = 'group';
		$this->listMeta->defaultOrder = 'id';
		$this->setListWriteIcon(tt('Add a new User-Group'));
		$this->listMeta->addOperation('group_role', $role_label, 'config.png');
		$this->setListEditIcon();
		$this->setListDropIcon();
		$this->selectListPage(10, "WHERE (section=".SECTION." OR id=1 OR id=2 OR id=3)");
		return $this->renderListPage();
	}
	
	public function edit(){
		$gid = (int)gg('id');
		if($gid != 1 and $gid != 2 and $gid != 3){
			return Control::edit();
		}else{
			execute_set_db();
			document_set_redirect('group');
			notify_add_error(tt('The selected group is not allowed to be edited.'));
		}
	}
	
	public function drop(){
		$gid = (int)gg('id');
		if($gid != 1 and $gid != 2 and $gid != 3){
			return Control::drop();
		}else{
			execute_set_db();
			document_set_redirect('group');
			notify_add_error(tt('The selected group is not allowed to be deleted.'));
		}
	}
}
?>
