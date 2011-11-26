<?php
function module_get_info_directory(){
	$description = tt('Directory is a tree-structured content management system and major module of Ximple. ');
	$description.= tt('A Directory can contain many Sub-Directories and Pages. ');
	$description.= tt('It is powerful, flexible and fully extensible.');
	return array(
		'name' => tt('Directory'),
		'description' => $description,
	);
}

function module_init_directory(){
	include_once('PageDirectory.class.inc');
	include_once('DirectoryConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/menu.tool.inc');
	
	$directory = new PageDirectory();
	$page = $directory->initModule();
	
	$directoryConfig = new DirectoryConfig();
	$directoryConfig->initModule('DirectoryConfig.class.inc');
	
	$directory_name = FILES_PATH.'directory';
	if(!is_dir($directory_name)) mkdir($directory_name);
	
	$panel = new PanelManager('Page');
	$panel->drop();
	
	$admin = new RoleManager('directory_admin');
	$admin->setName('Directory Administrator');
	$admin->setDescription('Who can create, edit and drop all Directories.');
	$admin->submit();
	
	$contributor = new RoleManager('directory_contributor');
	$contributor->setName('Directory Contributor');
	$contributor->setDescription('Who can create Sub-Directories in allowed Directories.');
	$contributor->submit();
	
	$panel = new PanelManager('Directory');
	$panel->appendControl($directory);
	$panel->appendRole($admin);
	$panel->appendRole($contributor);
	$panel->append('directory_list', 'list my Directory');
	$panel->append('directory_editor_list', 'list Directory in section');
	$panel->submit();
	
	list($admin, $editor, $contributor) = content_add_role('page', tt('Page'));
	$panel->appendControl($page);
	$panel->append('category/module/page', 'Page category');
	$panel->append('page_contributor_list', 'list my Page');
	$panel->append('page_editor_list', 'list Page in Section');
	$panel->appendRole($admin);
	$panel->appendRole($editor);
	$panel->appendRole($contributor);
	$panel->submit();
	
	menu_add('directory', tt('Main Page'));
	module_add_cache('directory');
	notify_add_info(tt('Directory has been successfully activated'));
}

function module_drop_directory(){
	include_once('PageDirectory.class.inc');
	include_once('DirectoryConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/menu.tool.inc');
	
	$directory = new PageDirectory();
	$directory->dropModule();
	
	$directoryConfig = new DirectoryConfig();
	$directoryConfig->dropModule();
	
	
	$admin = new RoleManager('directory_admin');
	$admin->drop();
	
	$contributor = new RoleManager('directory_contributor');
	$contributor->drop();
	
	content_drop_module('page', tt('Page'));
	menu_drop('directory');
	module_drop('page');
	module_drop_cache('directory');
	module_drop_panel('Directory');
	notify_add_info(tt('Directory has been successfully deactivated'));
}
?>
