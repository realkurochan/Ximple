<?php
function module_get_info_seminar(){
	return array(
		'name' => tt('Seminar'),
		'description' => tt('Seminar for any appointments and events')
	);
}

function module_init_seminar(){
	include_once('Seminar.class.inc');
	include_once('seminar.tool.inc');
	include_once(INCLUDE_PATH.'tool/block.tool.inc');
	
	$seminar = new seminar();
	$seminar->initModule('Seminar.class.inc');
	seminar_map_create_table();
	
	module_add_role('seminar_admin', 'Seminar Administrator', 'Who can configure, edit and drop seminar.');
	module_add_role('seminar_moderator', 'Seminar Moderator', 'Who can edit, drop seminar and admin');
	module_add_role('seminar_organizer', 'Seminar Organizer', 'Who can set seminar.');
	module_add_role('seminar_attendant', 'Seminar Attendant', 'Who attend the seminar.');
	
//	module_add_panel('Seminar', 'seminar_admin', 'seminar_config', 'configure seminar');
	module_add_panel('Seminar', 'seminar_admin', 'seminar_list', 'list all seminars');
	module_add_panel('Seminar', 'seminar_admin', 'seminar_displayAll', 'show all seminars');
//	module_add_panel('Seminar', 'seminar_admin', 'seminar_mylist', 'list my seminar');
	module_add_panel('Seminar', 'seminar_admin', 'category/module/seminar', 'seminar category');
	module_add_panel('Seminar', 'seminar_admin', 'seminar_write', 'submit new seminar');
	
	module_add_panel('Seminar', 'seminar_moderator', 'seminar_displayAll', 'show all seminars');
	module_add_panel('Seminar', 'seminar_moderator', 'seminar_write', 'submit new seminar');
	
	module_add_panel('Seminar', 'seminar_organizer', 'seminar_displayAll', 'show all seminars');
	module_add_panel('Seminar', 'seminar_organizer', 'seminar_write', 'submit new seminar');

	module_add_panel('Seminar', 'seminar_attendant', 'seminar_displayAll', 'show all seminars');	
	module_add_panel('Seminar', 'seminar_attendant', 'seminar_write', 'submit new seminar');
}

function module_drop_seminar(){
	include_once('Seminar.class.inc');
	include_once(INCLUDE_PATH.'tool/block.tool.inc');
	$seminar = new seminar();
	$seminar->dropModule();
	module_drop_role('seminar_admin');
	module_drop_role('seminar_moderator');
	module_drop_role('seminar_organizer');
	module_drop_role('seminar_attendant');
	module_drop_panel('Seminar');
}
?>
