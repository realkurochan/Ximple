<?php
include('RoleManager.class.inc');
include('PanelManager.class.inc');
include('WidgetManager.class.inc');

global $role_config, $panel_config;

function module_init_role(){
	global $role_config;
	$role_config = io_unserialize(FILES_PATH.'config/role'.SECTION);
}

function module_init_panel(){
	global $panel_config;
	$panel_config = io_unserialize(FILES_PATH.'config/panel'.SECTION);
}

function module_save_role(){
	global $role_config;
	io_serialize($role_config, FILES_PATH.'config/role'.SECTION);
}

function module_save_panel(){
	global $panel_config;
	io_serialize($panel_config, FILES_PATH.'config/panel'.SECTION);
}

function module_add_role($reference, $name, $description){
	global $role_config;
	$role_config[$reference]['name'] = $name;
	$role_config[$reference]['description'] = $description;
}

function module_drop_role($reference){
	global $role_config;
	unset($role_config[$reference]);
}

function module_add_panel($module_name, $role, $mode, $name){
	global $panel_config;
	$panel_config[$module_name][$role][$name] = $mode;
}

function module_drop_panel($module_name){
	global $panel_config;
	unset($panel_config[$module_name]);
}


function module_add($reference, $requirement, $name=null, $extension=null){
	$module_require = cc('module_require');
	if(!empty($extension)){
		if(!isset($module_require[$reference])){
			$module_require[$reference] = array($extension => $requirement);
		}elseif(is_array($module_require[$reference])){
			$module_require[$reference][$extension] = $requirement;
		}else{
			$module_require[$reference] = array(
				'#primary' => $module_require[$reference],
				$extension => $requirement,
			);
		}
	}else{
		$module_require[$reference] = $requirement;
	}
	config_set('module_require', $module_require);
	if(strlen($name)){
		config_append_assc('module_name', $reference, $name);
	}
}

function module_drop($reference){
	config_drop_assc('module_require', $reference);
	config_drop_assc('module_name', $reference);
}

function module_add_feed($mode, $name){
	config_append_assc('feed_available_mode', $mode, $name);
}

function module_drop_feed($mode){
	config_drop_assc('feed_available_mode', $mode);
}

function module_add_search($mode, $topic, $freetags, $raw){
	$search = array(
		'topic' => $topic,
		'freetags' => $freetags,
		'raw' => $raw,
	);
	config_append_assc('search_modules', $mode, $search);
}

function module_drop_search($mode){
	config_drop_assc('search_modules', $mode);
}


function module_add_cache($mode){
	config_append('cachable_mode', $mode);
}

function module_drop_cache($mode){
	config_drop_by_value('cachable_mode', $mode);
}


function module_add_cron($reference, $time_span){
	$cron = cc('cron');
	$cron[$reference] = $time_span;
	config_set('cron', $cron);
}

function module_drop_cron($reference){
	$cron = cc('cron');
	unset($cron[$reference]);
	config_set('cron', $cron);
}
?>
