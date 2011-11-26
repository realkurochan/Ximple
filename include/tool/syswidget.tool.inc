<?php

execute_add_function('syswidget_login');
execute_add_function('syswidget_panel');
execute_add_function('syswidget_execute_info');
execute_add_function('syswidget_translate');
execute_add_function('syswidget_navigation');
execute_add_function('syswidget_navigation_config');
execute_add_function('syswidget_navigation_save');

function syswidget_login(){
	if(authority_is_guest()){
		$render_array = array(
			'forget_password' => cc('forget_password'),
			'registable' => cc('registable')
		);
		head_add_js('include/external/sha1.js');
		return render($render_array, 'login_form.tpl', 'login_form');
	}else{
		return render(array(), 'logout_form.tpl', 'logout_form');
	}
}

function syswidget_panel(){
	if(authority_is_guest()) return null;
	$cache_dir = cache_get_dir('panel');
	$cache_file = $cache_dir.USER_ID;
	if(is_file($cache_file) and 0) return io_read_file($cache_file);
	$render_data = array('panel' => syswidget_get_panel_array());
	$render = render($render_data, 'panel.tpl', 'panel');
	io_write_file($cache_file, $render);
	return $render;
}

function syswidget_navigation(){
	$navigation = cc('navigation');
	$navigation['home'] = cc('home_label');
	$navigation = translate_content($navigation, 'widget', gg('id'));
	$navigation_enabled = array();
	foreach(cc('navigation_enabled') as $key){
		$navigation_enabled[$key] = $navigation[$key];
	}
	return render(array('data' => $navigation_enabled), 'navigation.tpl', 'navigation');
}

function syswidget_translate(){
	$render_array = array();
	$render_array['language'] = cc('supported_language');
	if(!is_array($render_array['language'])){
		$render_array['language'] = array();
	}
	return render($render_array, 'translate_widget.tpl', 'translate_widget');
}

function syswidget_navigation_config(){
	if(gg('is_translate') == 'false') return  syswidget_navigation_general_config();
	else return syswidget_navigation_translate_config();
}

function syswidget_navigation_general_config(){
	$navigation = cc('navigation');
	$navigation['home'] = cc('home_label');
	$modelCol = Model::checkedit(tt('enable edit and sort Navigation item'), $navigation);
	$modelCol->data = cc('navigation_enabled');
	$modelCol->isSortable = true;
	return array('navigation' => $modelCol);
}

function syswidget_navigation_translate_config(){
	$translated = translate_get_all_data('widget', gg('module_id'), gg('language'));
	$navigation = cc('navigation');
	$form = array();
	foreach($navigation as $key => $value){
		$model = Model::char($value);
		if(isset($translated[$key])){
			$model->data = $translated[$key];
		}
		$form[$key] = $model;
	}
	return $form;
}

function syswidget_navigation_save(){
	$navigation = pp('navigation');
	$old_navigation = cc('navigation');
	if(is_array($navigation)){
		foreach($navigation as $key => $value){
			unset($old_navigation[$key]);
			if($key == 'home'){
				config_set('home_label', $value);
			}
		}
	}
	$new_navigation = array_merge($navigation, $old_navigation);
	config_set('navigation', $new_navigation);
	config_set('navigation_enabled', array_keys($navigation));
	config_save();
}

function syswidget_execute_info(){
	return '<!--{site_info}-->';
}


function syswidget_get_panel_array(){
	$panel = config_default_panel();
	if(authority_is_admin()){
		$panel = array_merge($panel, syswidget_get_admin_panel_array());
	}else{
		$panel = array_merge($panel, syswidget_get_user_panel_array());
	}
	ksort($panel);
	return $panel;
}

/// Processing panel array for admin.
/// @return Array of panel for admin.
function syswidget_get_admin_panel_array(){
	$panel_conf = syswidget_get_panel_config();
	$panel_out = array();
	foreach($panel_conf as $key => $panel){
		$items = array();
		foreach($panel as $acc_ref => $item){
			$items = array_merge($items, $item);
		}
		ksort($items);
		$panel_out[$key] = $items;
	}
	return $panel_out;
}

/// Processing panel array for regular user (no guest no admin).
/// @return Array of panel for regular user.
function syswidget_get_user_panel_array(){
	$panel_conf = syswidget_get_panel_config();
	$panel_out = array();
	$groups = ss('login_group');
	$groups[] = 3;
	foreach($panel_conf as $key => $panel){
		$items = array();
		foreach($panel as $acc_ref => $item){
			$ref_group = cc($acc_ref);
			if(is_array($ref_group) and count(array_intersect($groups, $ref_group))){
				$items = array_merge($items, $item);
			}
		}
		ksort($items);
		if(count($items)) $panel_out[$key] = $items;
	}
	return $panel_out;
}

function syswidget_get_panel_config(){
	$panel = io_unserialize(FILES_PATH.'config/panel'.SECTION);
	if(!is_array($panel)) $panel = array();
	$panel = array_merge($panel, config_default_panel_access());
	return $panel;
}

function syswidget_get_list(){
	return array(
		'primary' => array('syswidget_login', 'syswidget_panel', 'syswidget_navigation'),
		'secondary' => array('syswidget_execute_info'),
	);
}

?>
