<?php

global $cache_lock_page_cache;
global $cache_lock;
$cache_lock_page_cache = false;
$cache_lock = false;

function cache_get_dir($mode){
	$cache_dir  = FILES_PATH.'cache/';
	if(!is_dir($cache_dir)) mkdir($cache_dir);
	$cache_dir .= SECTION.'/';
	if(!is_dir($cache_dir)) mkdir($cache_dir);
	$cache_dir .= $mode.'/';
	if(!is_dir($cache_dir)) mkdir($cache_dir);
	return $cache_dir;
}

function cache_lock_page(){
	global $cache_lock_page_cache;
	$cache_lock_page_cache = true;
}

function cache_lock(){
	global $cache_lock;
	$cache_lock = true;
}

function cache_release(){
	global $cache_lock;
	$cache_lock = false;
}

function cache_is_lock(){
	global $cache_lock;
	return $cache_lock;
}

function cache_check_page($file_name){
	return is_file($file_name);
}

function cache_show_page($file_name){
	$render = io_read_file($file_name);
	$render = postrender_get_read_number($render);
	$render = postrender_get_site_info($render);
	echo($render);
}

function cache_save_page($page, $file_name){
	global $cache_lock_page_cache;
	$cache_mode = cc('cache_mode');
	$is_cache = false;
	if(!$cache_lock_page_cache and $cache_mode >= 2 and in_array(MODE, cc('cachable_mode'))){
		if(!is_file($file_name)) io_write_file($file_name, $page);
		$is_cache = true;
	}
	
	if($is_cache and $cache_mode >= 3){
		$cache_dir = FILES_PATH.'precache/'.SECTION.'/';
		if(!is_dir($cache_dir)) mkdir($cache_dir);
		$file_name = $cache_dir.basename($file_name);
		if(!is_file($file_name)) io_write_file($file_name, $page);
	}
}

function cache_clear_section(){
	$cache_dir = FILES_PATH.'precache/'.SECTION.'/';
	io_clear_directory($cache_dir);
	$cache_dir = FILES_PATH.'cache/'.SECTION.'/';
	io_clear_directory($cache_dir);
}

function cache_get_page_file(){
	if(!defined('CACHE_PAGE_FILE')){
		$cache_dir  = cache_get_dir('page').MODE.'/';
		if(!is_dir($cache_dir)) mkdir($cache_dir);
		$cache_dir .= gg('id').'/';
		if(!is_dir($cache_dir)) mkdir($cache_dir);
		$cache_page_file = $cache_dir.md5($_SERVER['REQUEST_URI'].USER_ID);
		define('CACHE_PAGE_FILE', $cache_page_file);
		return $cache_page_file;
	}else{
		return CACHE_PAGE_FILE;
	}
}

function cache_save_db($table, $query, $data){
	$cache_file = cache_get_db_file($table, $query);
	if(!is_file($cache_file.'.php')) io_serialize($data, $cache_file);
}

function cache_clear_db($table=''){
	io_clear_directory(FILES_PATH.'dbcache/'.$table);
}

function cache_get_db_file($table, $query){
	$cache_dir  = FILES_PATH.'dbcache/';
	if(!is_dir($cache_dir)) mkdir($cache_dir);
	$cache_dir .= $table.'/';
	if(!is_dir($cache_dir)) mkdir($cache_dir);
	return $cache_dir.md5($query);
}

function cache_check_db($file_name){
	return is_file($file_name.'.php');
}

function cache_get_db_data($file_name){
	return io_unserialize($file_name);
}

function cache_clear_by_day($date_file, $directory, $day_count=1){
	$set_clear_date = true;
	if(is_file($date_file)){
		if(is_file($date_file) and io_read_file($date_file) == date('Y-m-d')){
			$set_clear_date = false;
		}else{
			io_clear_directory($directory);
		}
	}
	if($set_clear_date){
		$timestamp = time()+ abs($day_count-1)*60*60*24;
		io_write_file($date_file, date('Y-m-d'), $timestamp);
	}
}

?>
