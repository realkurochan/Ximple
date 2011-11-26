<?php
execute_add_function('module_manage');
execute_add_function('module_init');
execute_add_function('module_activate');
execute_add_function('module_deactivate');
execute_add_function('module_refresh');

function module_manage(){
	if(!authority_group('module_admin')) error_denied();
	$title = tt('Manage Module');
	document_set_title($title);
	$module = module_get_information();
	head_add_css('list.css');
	head_add_css('module.css');
	document_set_path($title);
	$render_array = array(
		'module' => $module,
		'has_private' => module_has_private($module),
	);
	return render($render_array, 'module.tpl', 'module_manage');
}

function module_init(){
	if(!authority_group('module_admin')) error_denied();
	module_prepare();
	$module_array = module_get_information();
	$submit_array = pp();
	$activated_module = array();
	foreach($submit_array as $key => $value){
		if(isset($module_array[$key]) and !$module_array[$key]['installed']){
			$function = 'module_init_'.$key;
			$function();
			$activated_module[] = $key;
		}
	}
	extension_execute('module_activate', $activated_module);
	
	$deactivated_module = array();
	foreach($module_array as $key => $value){
		if(!isset($submit_array[$key]) and $value['installed']){
			$function = 'module_drop_'.$key;
			$function();
			$deactivated_module[] = $key;
		}
	}
	extension_execute('module_deactivate', $deactivated_module);
	
	config_save();
	module_post();
	notify_add_info(tt('Module configuration has been successfully saved.'));
	execute_set_db();
	document_set_redirect('module_manage');
}

function module_activate(){
	if(!authority_group('module_admin')) error_denied();
	module_prepare();
	module_activate_module();
	notify_add_info(tt('Module has been successfully activated.'));
	module_post();
	config_save();
	execute_set_db();
	document_set_redirect('module_manage');
}

function module_deactivate(){
	if(!authority_group('module_admin')) error_denied();
	module_prepare();
	module_deactivate_module();
	notify_add_info(tt('Module has been successfully deactivated.'));
	module_post();
	config_save();
	execute_set_db();
	document_set_redirect('module_manage');
}

function module_refresh(){
	if(!authority_group('module_admin')) error_denied();
	module_prepare();
	module_deactivate_module();
	module_activate_module();
	module_post();
	config_save();
	notify_add_info(tt('Module has been successfully refreshed.'));
	execute_set_db();
	document_set_redirect('module_manage');
}

function module_activate_module(){
	$module_array = module_get_information();
	$module_name = gg('module_name');
	if(array_key_exists($module_name, $module_array)){
		if(!$module_array[$module_name]['installed']){
			module_include_locale($module_name, $module_array[$module_name]);
			$function = 'module_init_'.$module_name;
			$function();
			extension_execute('module_activate', array($module_name));
		}
	}
}

function module_deactivate_module(){
	$module_array = module_get_information();
	$module_name = gg('module_name');
	$set_require = false;
	$require = cc('module_require');
	if(array_key_exists($module_name, $module_array)){
		if($module_array[$module_name]['installed']){
			module_include_locale($module_name, $module_array[$module_name]);
			$function = 'module_drop_'.$module_name;
			$function();
			extension_execute('module_deactivate', array($module_name));
			if(isset($require[$module_name]) and count($require[$module_name]) == 0){
				unset($require[$module_name]);
				$set_require = true;
			}
		}
		if($set_require){
			config_set('module_require', $require);
			config_save();
		}
	}
}

function module_get_information(){
	list($module, $init_file) = module_get_init_file(MODULE_PATH, false);
	
	$private_path = SECTION_PATH.'private/module/';
	if(is_dir($private_path)){
		list($module_private, $init_file_private) = module_get_init_file($private_path, true);
		$module = array_merge($module_private, $module);
		$init_file = array_merge($init_file_private, $init_file);
	}
	
	module_include_init_file($init_file);
	$module = module_check_function($module);
	$module = module_check_status($module);
	$module = module_get_description($module);
	return $module;
}

function module_get_init_file($dir_name, $is_private){
	$init_array = array();
	$module_array = array();
	$dir = dir($dir_name);
	while (false !== ($entry = $dir->read())) {
		if($entry != '.' and $entry != '..'){
			$init_file = $dir_name.$entry.'/module.init.inc';
			if(is_file($init_file)){
				$init_array[] = $init_file;
				$module_array[$entry]['is_private'] = $is_private;
			}
		}
	}
	ksort($module_array);
	return array($module_array, $init_array);
}

function module_include_init_file($init_file){
	foreach($init_file as $file){
		if(state_is_develop()){
			include_once($file);
		}else{
			@include_once($file);
		}
	}
}

function module_check_function($module){
	$module_out = array();
	foreach($module as $name => $value){
		$functions_exist = function_exists('module_init_'.$name);
		$functions_exist = $functions_exist and function_exists('module_get_info_'.$name);
		$functions_exist = $functions_exist and function_exists('module_drop_'.$name);
		if($functions_exist){
			$module_out[$name] = $value;
		}
	}
	return $module_out;
}

function module_check_status($module){
	$module_out = array();
	$installed = cc('module_require');
	foreach($module as $name => $value){
		$module_out[$name] = $value;
		$module_out[$name]['installed'] = array_key_exists($name, $installed);
	}
	
	return $module_out;
}

function module_get_description($module){
	foreach($module as $name => $value){
		$function_name = 'module_get_info_'.$name;
		$module[$name] = array_merge($module[$name], $function_name());
	}
	return $module;
}

function module_prepare(){
	require_once('module.tool.inc');
	module_init_panel();
	module_init_role();
}

function module_post(){
	module_save_panel();
	module_save_role();
}

function module_has_private($module){
	foreach($module as $key => $value){
		if($value['is_private']) return true;
	}
	return false;
}

function module_include_locale($module, $info){
	if($info['is_private']){
		translate_add_directory(SECTION_PATH.'private/module/'.$module);
	}else{
		translate_add_directory(ROOT_PATH.'module/'.$module);
	}
}
?>
