<?php
function module_get_info_advertise(){
	return array(
		'name' => tt('Advertise'),
		'description' => tt('Module for Advertising and Customer management.')
	);
}

function module_init_advertise(){
	include_once('Advertise.class.inc');
	include_once('AdvertiseConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	
	$advertise = new Advertise();
	$advertise->initModule('Advertise.class.inc');
	$advertiseConfig = new AdvertiseConfig();
	$advertiseConfig->initModule('AdvertiseConfig.class.inc');
	
	$admin = new RoleManager('advertise_admin');
	$admin->setName('Advertise Administrator');
	$admin->setDescription('Who can edit and drop advertise.');
	$admin->submit();
	
	$panel = new PanelManager('Advertise');
	$panel->appendControl($advertise);
	$panel->append('advertise_write', 'create new Advertise');
	$panel->append('advertise_list', 'list Advertise');
	$panel->append('advertise_config', 'config Advertise');
	$panel->appendRole($admin);
	$panel->submit();
	
	config_set('advertise_widget_position', array(tt('Advertise Primaty Position')));
	widget_add(tt('Advertise Primaty Position'), 'advertise_banner/0');
	
	$path = FILES_PATH.'advertise/';
	if(!is_dir($path)) mkdir($path);
}

function module_drop_advertise(){
	include_once('Advertise.class.inc');
	include_once('AdvertiseConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	
	$advertise = new Advertise();
	$advertise->dropModule();
	
	$advertiseConfig = new AdvertiseConfig();
	$advertiseConfig->dropModule();
	
	$advertise_position = cc('advertise_widget_position');
	foreach($advertise_position as $key => $value){
		widget_drop('advertise_banner/'.$key);
	}
	
	$admin = new RoleManager('advertise_admin');
	$admin->drop();
	
	$panel = new PanelManager('Advertise');
	$panel->drop();
}
?>
