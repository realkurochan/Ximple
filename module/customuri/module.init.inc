<?php
function module_get_info_customuri(){
	$describe = tt('Customize URI of each page to improve SEO (Search Engine Optimization).');
	$describe.= tt('Custom URI can only be enabled by enabling "friendly URI".');
	return array(
		'name' => tt('CustomURI'),
		'description' => $describe,
	);
}

function module_init_customuri(){
	if(cc('friendly_url_enable')){
		include_once('CustomURI.class.inc');
		$customuri = new CustomURI();
		$customuri->initModule('customuri.module.inc');
		
		$admin = new RoleManager('customuri_admin');
		$admin->setName('CustomURI Administrator');
		$admin->setDescription('Who can add edit and drop Custom URI.');
		$admin->submit();
	
		$panel = new PanelManager('Custom URI');
		$panel->appendControl($customuri);
		$panel->append('customuri', 'manage Custom URI');
		$panel->append('customuri_write', 'add new Custom URI');
		$panel->appendRole($admin);
		$panel->submit();
		
		extension_add('main_init', 'customuri_map');
		extension_add('post_main_render', 'customuri_post_render');
	}else{
		notify_add_error(tt('Custom URI cannot be enabled without enabling "friendly URI".'));
	}
}

function module_drop_customuri(){
	include_once('CustomURI.class.inc');
	$customuri = new CustomURI();
	$customuri->dropModule();
	
	$admin = new RoleManager('customuri_admin');
	$admin->drop();
	
	$panel = new PanelManager('Custom URI');
	$panel->drop();
	
	extension_drop('main_init', 'customuri_map');
	extension_drop('post_main_render', 'customuri_post_render');
}
?>
