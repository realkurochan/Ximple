<?php
include_once(INCLUDE_PATH.'tool/menu.tool.inc');

function content_init_module($content){
	$moduleName = $content->fullModuleName;
	$moduleTitle = $content->moduleTitle;
	module_add_feed($moduleName, $moduleTitle);
	module_add_cache($moduleName);
	content_add_panel($content);
	menu_add($moduleName, $moduleTitle);
	notify_add_subscribe($moduleName, tt('someone posts in : ', true).$moduleTitle);
}

function content_add_role($moduleName, $moduleTitle){
	$admin = new RoleManager($moduleName.'_admin');
	$admin->setName($moduleTitle.' Administrator');
	$admin->setDescription(sprintf('Who can configure, insert, edit and drop %s from everyone.', $moduleTitle));
	$admin->submit();
	
	$editor = new RoleManager($moduleName.'_editor');
	$editor->setName($moduleTitle.' Editor');
	$editor->setDescription(sprintf('Who can insert, edit and drop %s from everyone.', $moduleTitle));
	$editor->submit();
	
	$contributor = new RoleManager($moduleName.'_contributor');
	$contributor->setName($moduleTitle.' Contributor');
	$contributor->setDescription(sprintf('Who can insert, edit and drop own %s.', $moduleTitle));
	$contributor->submit();
	
	$commentAdmin = new RoleManager($moduleName.'_comment_admin');
	$commentAdmin->setName($moduleTitle.' Comment Administrator');
	$commentAdmin->setDescription(sprintf('Who can edit and drop comments in %s.', $moduleTitle));
	$commentAdmin->submit();
	return array($admin, $editor, $contributor);
}

function content_add_panel($content){
	$moduleName = $content->fullModuleName;
	$moduleTitle = $content->moduleTitle;
	list($admin, $editor, $contributor) = content_add_role($moduleName, $moduleTitle);
	$panel = new PanelManager($moduleTitle);
	$panel->appendControl($content);
	$panel->append('category/module/'.$moduleName, $moduleTitle.' category');
	$panel->append($moduleName.'_write', 'write new '.$moduleTitle.' entry');
	$panel->append($moduleName.'_contributor_list', 'list my '.$moduleTitle);
	$panel->append($moduleName.'_editor_list', 'list '.$moduleTitle.' in Section');
	$panel->append($moduleName.'_config', 'configure '.$moduleTitle);
	$panel->appendRole($admin);
	$panel->appendRole($editor);
	$panel->appendRole($contributor);
	$panel->submit();	
}

function content_drop_module($moduleName, $moduleTitle){
	module_drop_feed($moduleName);
	module_drop_cache($moduleName);
	
	$admin = new RoleManager($moduleName.'_admin');
	$admin->drop();
	
	$editor = new RoleManager($moduleName.'_editor');
	$editor->drop();
	
	$contributor = new RoleManager($moduleName.'_contributor');
	$contributor->drop();
	
	$panel = new PanelManager($moduleTitle);
	$panel->drop();
	
	config_drop($moduleName.'_appearance');
	config_drop($moduleName.'_comment_permission');
	config_drop($moduleName.'_comment_permission_default');
	config_drop($moduleName.'_publishing_status');
	config_drop($moduleName.'_publishing_status_default');
	
	config_drop($moduleName.'_comment_write_label');
	config_drop($moduleName.'_comment_page_length');
	config_drop($moduleName.'_comment_show_avatar');
	
	menu_drop($moduleName);
	notify_drop_subscribe($moduleName);
}

function content_get_comment_permission(){
	return array(
		0 => tt('everybody can comment'),
		1 => tt('only authenticated user can comment'),
		2 => tt('read only'),
		3 => tt('disable comment'),
	);
}

function content_get_publishing_status(){
	return array(
		0 => tt('published'),
		1 => tt('featured'),
		2 => tt('in review'),
		3 => tt('draft'),
	);
}
?>
