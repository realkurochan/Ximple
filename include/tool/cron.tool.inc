<?php
function cron_init(){
	define('XIMPLE_CMS', 'initialized');
	init_get_setup();
	init_set_path();
	init_include();
	init_config();
	db_init();
	init_set_time_zone();
	init_var();
	init_check_cron();
	authority();
	translate_define_language();
	config_translate();
	translate_add_directory(THEME_PATH.''.cc('theme'));
}

function cron_exec(){
	$cron = cc('cron');
	$message = "XimpleCMS running Cron-Jobs\n";
	if(is_array($cron) and count($cron)){
		$time = time();
		foreach($cron as $key => $value){
			if($value > 0 and $time%$value == 0){
				execute($key);
				$message .= sprintf("Execute %s.\n", $key);
			}
		}
		cron_write_log($time, $message);
		echo(date('Y-m-d H:i:s', $time)."\n$message\n");
	}
}

function cron_write_log($time, $message){
	$dir = FILES_PATH.'cron/';
	$loop = floor($time/3600)%720;
	if(!is_dir($dir)) mkdir($dir);
	io_write_file($dir.SECTION.'.txt', (string) $time);
	io_append_file($dir.SECTION.'_'.$loop.'_log.txt', date('Y-m-d H:i:s', $time)."\n$message\n");
}

function cron_check_process(){
	if(XIMPLE_CRON_PROCESS == 'Cron Process') return true;
	else error_denied();
}
?>
