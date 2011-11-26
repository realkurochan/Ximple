<?php
/// Include necessary tools.
function init_include(){
	include(INCLUDE_PATH.'tool/io.tool.inc');
	include(INCLUDE_PATH.'tool/error.tool.inc');
	include(INCLUDE_PATH.'tool/render.tool.inc');
	include(INCLUDE_PATH.'tool/cache.tool.inc');
	include(INCLUDE_PATH.'tool/authority.tool.inc');
	include(INCLUDE_PATH.'tool/postrender.tool.inc');
	include(INCLUDE_PATH.'tool/extension.tool.inc');
	include(INCLUDE_PATH.'tool/execute.tool.inc');
	include(INCLUDE_PATH.'tool/notify.tool.inc');
	include(INCLUDE_PATH.'tool/document.tool.inc');
	include(INCLUDE_PATH.'tool/config.tool.inc');
	include(INCLUDE_PATH.'tool/input.tool.inc');
	include(INCLUDE_PATH.'tool/count.tool.inc');
	include(INCLUDE_PATH.'tool/translate.tool.inc');
	include(INCLUDE_PATH.'database/'.DB_TYPE.'.inc');
	if(!function_exists('mime_content_type')){
		include_once(INCLUDE_PATH.'external/upgradephp/ext/mime.php');
	}
}

/// Get setup values from setup file.
function init_get_setup(){
	if(!defined('SETUP_DIRECTORY')) $setup_directory = dirname(dirname(__FILE__));
	else $setup_directory = SETUP_DIRECTORY;
	
	$test_setup = $setup_directory.'/setup.test.php';
	$setup = $setup_directory.'/setup.php';
	
	if(state_is_test() and is_file($test_setup)) include($test_setup);
	else include($setup);
}

/// Get configuration array of SECTION
/// @param $section : SECTION-ID
function init_config(){
	$GLOBALS['config'] = io_unserialize(FILES_PATH.'config/config'.SECTION);
	$GLOBALS['config'] = config_check_default($GLOBALS['config']);
}

function init_set_path(){
	if(state_is_test()) init_set_test_path();
	else init_set_production_path();
}

function init_set_test_path(){
	define('FILES_PATH', ROOT_PATH.'files/');
	if(is_dir(ROOT_PATH.'test/include/')){
		define('INCLUDE_PATH', ROOT_PATH.'test/include/');
	}else{
		define('INCLUDE_PATH', ROOT_PATH.'include/');
	}
	
	if(is_dir(ROOT_PATH.'test/module/')){
		define('MODULE_PATH', ROOT_PATH.'test/module/');
	}else{
		define('MODULE_PATH', ROOT_PATH.'module/');
	}
	
	if(is_dir(ROOT_PATH.'test/theme/')){
		define('THEME_PATH', ROOT_PATH.'test/theme/');
	}else{
		define('THEME_PATH', ROOT_PATH.'theme/');
	}
}

function init_set_production_path(){
	define('FILES_PATH', ROOT_PATH.'files/');
	define('INCLUDE_PATH', ROOT_PATH.'include/');
	define('MODULE_PATH', ROOT_PATH.'module/');
	define('THEME_PATH', ROOT_PATH.'theme/');
}

function init_set_time_zone(){
	if(strlen(cc('time_zone')) & cc('time_zone') != 'UTC'){
		date_default_timezone_set(cc('time_zone'));
	}
}

/// Initialize global variables.
function init_var(){
	$_SESSION['xblc_section'] = SECTION;
	unset($_SESSION['is_admin']);
	
	define('NOW', date('Y-m-d H:i:s'));
	define('TODAY', date('d F Y'));
	
	$language = ss('language');
	if(strlen($language)) $language = $language.'/';
	
	if(cc('friendly_url_enable')) define('Q', ''.$language);
	else define('Q', '?/'.$language);
	init_section_uri();
	init_theme_uri();
	init_uri_param();
}

/// Initialize constant SECTION_URI.
function init_section_uri(){
	if(SECTION == 1){
		define('SECTION_URI', ROOT_URI);
	}elseif(!defined('SECTION_URI')){
		if(isset($config['section_uri']) and strlen($config['section_uri'])){
			define('SECTION_URI', cc('section_uri'));
		}else{
			define('SECTION_URI', ROOT_URI.'section/'.cc('section_path').'/');
		}
	}
}

function init_theme_uri(){
	if(is_dir(THEME_PATH.cc('theme'))){
		define('THEME_URI', ROOT_URI.'theme/'.cc('theme').'/');
	}elseif(is_dir(SECTION_PATH.'private/theme/'.cc('theme'))){
		define('THEME_URI', SECTION_URI.'private/theme/'.cc('theme').'/');
	}
}

function init_uri_param(){
	$get = gg();
	define('IS_INDEX', count($get) == 0);
	$get_value = array_values($get);
	if(count($get) == 1 and strlen($get_value[0]) == 0){
		$process_get = init_process_smart_uri();
	}else{
		$process_get = init_process_common_uri();
	}
	$process_get = init_check_mode($process_get);
	$module = split('_', MODE);
	define('MODULE', $module[0]);
	define('MODE_ID', $process_get['id']);
	define('MODE_PAGE', $process_get['page']);
	$_GET = $process_get;
	globalvar_reset_get();
}

function init_process_smart_uri(){
	$key = array_keys(gg());
	if(count($key)) $query_string = $key[0];
	else $query_string = '';
	$uri_get = execute_parse_smart_uri($query_string);
	$uri_server = execute_parse_smart_uri(vv('QUERY_STRING'));
	$uri = input_special_char($uri_get, $uri_server);
	define('MODE', $uri['mode']);
	define('ORIG_REQUEST', substr(vv('QUERY_STRING'),1));
	define('SMART_URI', true);
	return $uri;
}

function init_process_common_uri(){
	$get = gg();
	if(isset($get['mode'])) define('MODE', $get['mode']);
	else define('MODE', cc('index_mode'));
	init_define_orig_request();
	define('SMART_URI', false);
	return $get;
}

function init_define_orig_request(){
	$get = gg();
	if(count($get)){
		$orig_request = '?';
		foreach($get as $key=>$value){
			$orig_request .= $key.'='.$value.'&';
		}
		define('ORIG_REQUEST', $orig_request);
	}else{
		define('ORIG_REQUEST', '');
	}
}

function init_check_mode($process_get){
	if(!isset($process_get['id'])) $process_get['id'] = '0';
	if(!isset($process_get['page'])) $process_get['page'] = '0';
	if(!ctype_digit($process_get['id'])) error_syslog(tt('Invalid MODE_ID'));
	if(!ctype_digit($process_get['page'])) error_syslog(tt('Invalid MODE_PAGE'));
	return $process_get;
}

function init_check_cron(){
	$cron = cc('cron');
	if(is_array($cron) and count($cron)){
		if((time() - init_get_cron_time()) > 5){
			init_send_cron_error_mail();
		}
	}
}

function init_start_new_cron(){
	echo(SECTION_URI.'cron.php'."\n");
	$ch = curl_init(SECTION_URI.'cron.php');
	$result = curl_exec($ch);
	curl_close($ch);
	return $result;
}

function init_send_cron_error_mail(){
	require_once('userinfo.tool.inc');
	$message = tt('Your system has Cron-Jobs. '); 
	$message.= tt('However Cron-Jobs do not run correctly. ');
	$message.= tt('Please check System Cron Script.');
	$uid = userinfo_group_to_id(array(1));
	notify_send_mail($message, userinfo_get_email($uid));
}

function init_get_cron_time(){
	$file_name = FILES_PATH.'cron/'.SECTION.'.txt';
	$time = io_read_file($file_name);
	return (int) $time;
}
?>
