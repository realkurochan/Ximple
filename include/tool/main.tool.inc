<?php
/// @file
/// This file provides bootstrap functions and module execution with cache management for Ximple.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>


/// Initialize Ximple environment.
function main_init(){
	define('XIMPLE_CMS', 'initialized');
	main_handle_install();
	init_get_setup();
	init_set_path();
	main_handle_maintain();
	init_include();
	main_count_visitor();
	init_config();
	extension_execute('main_init');
	init_set_time_zone();
	init_var();
	main_handle_drop();
	authority();
	translate_define_language();
	config_translate();
	translate_add_directory(THEME_PATH.''.cc('theme'));
}

function main_exec(){
	extension_execute('pre_main_exec');
	$cache_file = cache_get_page_file();
	if(cache_check_page($cache_file)){
		cache_show_page($cache_file);
	}else{
		$render = main_render();
		$render = postrender_correct_uri($render);
		cache_save_page($render, $cache_file);
		if(execute_as_site()){
			$render = postrender_get_read_number($render);
			$render = postrender_get_site_info($render);
			$render = postrender_target_link($render);
		}
		echo($render);
	}
	notify_save();
	init_check_cron();
}

function main_render(){
	include(INCLUDE_PATH.'tool/head.tool.inc');
	include(INCLUDE_PATH.'tool/userinfo.tool.inc');
	include(INCLUDE_PATH.'tool/sectioninfo.tool.inc');
	include(INCLUDE_PATH.'tool/search.tool.inc');
	document_set_path(cc('home_label'), '');
	extension_execute('pre_main_render');
	$body = main_render_body();
	if(execute_as_site()){
		include(INCLUDE_PATH.'tool/syswidget.tool.inc');
		header('Content-Type: text/html; charset=UTF-8');
		$render = main_render_site($body);
	}elseif(execute_as_db()){
		header('Content-Type: text/html; charset=UTF-8');
		$render = main_render_db();
	}elseif(execute_as_ajax()){
		$body = extension_execute('pre_main_render_ajax', $body);
		header('Content-Type: text; charset=UTF-8');
		$render = $body;
		search_add_source_page($body);
	}elseif(execute_as_print()){
		header('Content-Type: text/html; charset=UTF-8');
		$render = main_render_print($body);
	}else{
		return null;
	}
	$render = extension_execute('post_main_render', $render);
	return $render;
}

function main_render_site($body){
	include_once(INCLUDE_PATH.'tool/menu.tool.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	search_increase_point();
	$body = extension_execute('pre_main_render_site', $body);
	$render_array = array(
		'body' => $body,
		'path' => document_render_path(),
		'menu' => menu_render(),
		'widget' => widget_render(),
		'notify' => notify_render(),
		'head' => head_render(),
	);
	$render_array['section_name'] = cc('section_name');
	$render_array['section_slogan'] = cc('section_slogan');
	search_add_source_page($body);
	$render = render($render_array, 'main.tpl', 'main');
	return $render;
}

function main_render_print($body){
	$body = extension_execute('pre_main_render_print', $body);
	$render_array = array(
		'body' => $body,
		'section_name' => cc('section_name'),
		'section_slogan' => cc('section_slogan'),
		'head' => head_render_print(),
	);
	$render = render($render_array, 'print.tpl', 'print');
	return $render;
}

function main_render_body(){
	$body = execute(MODE);
	return $body;
}

function main_count_visitor(){
	$authority = ss('authority');
	if(empty($authority)) count_increase('site', 0);
}

/// Check the installation of Ximple on server.
function main_handle_install(){
	if(state_is_install() or !is_file(PRE_DEFINED_ROOT_PATH.'include/setup.php')){
		include(PRE_DEFINED_ROOT_PATH.'include/tool/error.tool.inc');
		include(PRE_DEFINED_ROOT_PATH.'include/tool/extension.tool.inc');
		include(PRE_DEFINED_ROOT_PATH.'include/tool/render.tool.inc');
		define('ROOT_PATH', PRE_DEFINED_ROOT_PATH);
		init_set_production_path();
		$root_uri = 'http://'.$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF']).'/';
		define('ROOT_URI', $root_uri);
		$error = 'Ximple is not installed on your system, please try to <a href="'.$root_uri.'install">install</a>';
		error_fatal('Ximple is not installed.', $error);
	}
}

/// Check and handle maintain state of section.
function main_handle_maintain(){
	$is_guest = (!isset($_SESSION['ximple_id']) or $_SESSION['ximple_id'] == -1);
	$is_login = isset($_GET['/login']);
	$is_login_submit = isset($_GET['/login_submit']);
	if(state_is_maintain() and $is_guest and !($is_login or $is_login_submit)){
		include(INCLUDE_PATH.'tool/error.tool.inc');
		include(INCLUDE_PATH.'tool/render.tool.inc');
		$error = 'We are under maintenance, please be patient.';
		error_fatal('Under maintenance', $error);
	}
}

function main_handle_drop(){
	if(preg_match('/drop/', MODE) and gg('confirm') != "true"){
		$confirm = SMART_URI ? '/confirm/true' : '&confirm=true';
		$render_data = array(
			'redirect' => '',
			'check_drop' => true,
			'confirm_uri' => vv('REQUEST_URI').$confirm,
			'is_develop' => state_is_develop(),
		);
		$render = render($render_data, 'redirect.tpl', 'main_handle_drop');
		die($render);
	}
}

function main_render_db(){
	$redirect = document_get_redirect();
	if(document_get_redirect_back()) $redirect = vv('HTTP_REFERER');
	elseif(substr($redirect, 0, 1) == '?') $redirect = SECTION_URI.$redirect;
	elseif(strlen($redirect)) $redirect = SECTION_URI.Q.$redirect;
	else $redirect = SECTION_URI;
	$confirm = SMART_URI ? '/confirm/true' : '&confirm=true';
	$render_data = array(
		'redirect' => $redirect,
		'check_drop' => preg_match('/drop/', MODE) and gg('confirm') != "true",
		'confirm_uri' => vv('REQUEST_URI').$confirm,
		'is_develop' => state_is_develop(),
	);
	$render = render($render_data, 'redirect.tpl', 'db_redirect');
	echo($render);
}
?>
