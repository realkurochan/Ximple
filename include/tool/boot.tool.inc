<?php
/// @file
/// This file provides function for bootstrap of XimpleCMS.

/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Bootstrap of XimpleCMS.
function boot(){
	define('PRE_DEFINED_ROOT_PATH', dirname(dirname(dirname(__FILE__))).'/');
	session_start();
	require('precache.tool.inc');
	$to_die = precache_get_page();
	if(!$to_die){
		if(isset($_POST['logout']) and $_POST['logout'] == 'logout'){
			session_destroy();
		}
		require('setini.tool.inc');
		setini();
		define('IS_APC', function_exists('apc_add') and !state_is_develop());
		require('main.tool.inc');
		require('init.tool.inc');
		main_init();
		main_exec();
	}
}

/// Bootstrap of Cron-Jobs.
function boot_cron(){
	define('IS_APC', function_exists('apc_add'));
	session_start();
	require('setini.tool.inc');
	setini();
	define('IS_APC', function_exists('apc_add') and !state_is_develop());
	require('cron.tool.inc');
	require('init.tool.inc');
	cron_init();
	cron_exec();
}
?>
