<?php
function module_get_info_dbbackup(){
	return array(
		'name' => tt('Database Backup'),
		'description' => tt('Croned database backup for XimpleCMS.'),
	);
}

function module_init_dbbackup(){
	module_add('dbbackup', 'dbbackup.module.inc');
	module_add_cron('dbbackup', 60*60*24*3);
	
	extension_add('section_config', 'dbbackup_config');
	extension_add('section_config_save', 'dbbackup_config_save');
}

function module_drop_dbbackup(){
	module_drop('dbbackup');
	module_drop_cron('dbbackup');
	
	extension_drop('section_config', 'dbbackup_config');
	extension_drop('section_config_save', 'dbbackup_config_save');
}
?>
