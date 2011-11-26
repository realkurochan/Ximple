<?php
/// @file
/// Get cache file for agressive cache-mode.
/// Agressive cache-mode is cache system of Ximple, which provides extreme fast
/// page caching. The rendered page is saved in a file. By this mode, the file
/// is read and shown and Ximple does not do nothing elese. Hence side-effects
/// can occur. This mode is based on page-cache system of Ximple with reduction
/// of functionalities.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>
/// @state complete.

/// Check read and show page cache from cache file.
/// @return TRUE, if page-cache exists.
function precache_get_page(){
	return false;
	list($mode, $params) = precache_get_param();
	$mode_id = precache_get_mode_id($params);
	$uid = preacache_get_user_id();
	list($root, $cache_file) = precache_get_file($uid, $mode);
	
	if(is_file($cache_file)){
		precache_increase_read_no($mode_id, $root, $params);
		precahce_show($cache_file);
		return true;
	}
	return false;
}

/// Read and show page-cache.
/// @param $cache_file : location of cache file to read.
function precahce_show($cache_file){
	$file = @ fopen($cache_file,'r');
	$page = @ fread($file,filesize($cache_file));
	$site_info = precache_get_site_info();
	$page = precache_inject_site_info($page, $site_info);
	echo($page);
}

/// Get location of cache file.
/// @param $uid  : USER-ID.
/// @param $mode : mode name of called page.
/// @return Root directory of Ximple and location of cache file.
function precache_get_file($uid, $mode){
	$root = realpath(dirname(__FILE__).'/../').'/files/cache/precache/';
	$cache_dir = $root.'pre'.$mode.'cache';
	$cache_file = $cache_dir.'/'.md5($_SERVER['REQUEST_URI'].$uid).'.txt';
	return array($root, $cache_file);
}

/// Inject site information into shown page.
/// @param $page : rendered page to show.
/// @param $site_info : site information to inject
/// @return Page with site information.
function precache_inject_site_info($page, $site_info){
	$bpage = substr($page,-2000);
	$bpage = str_replace('<!--si-->', '<!--', $bpage);
	$bpage = str_replace('<!--is-->', '-->'.$site_info, $bpage);
	return substr($page,0,-2000).$bpage;
}

/// Get site information (information about the interpretation of page).
/// @return Site information.
function precache_get_site_info(){
	$end_time = (float)microtime(true);
	$time = abs($end_time-BOOT_TIME)*1e3;
	$mem = memory_get_usage()/1e6;
	$pattern = '[Ximple agressive cache | time : %.2f ms | memory : %.2f MB | no query]';
	return sprintf($pattern, $time, $mem);
}

/// Store number of the page reader in a file.
/// @param $mode_id : ID of the page mode.
/// @param $root : root directory of Ximple.
/// @param $params : array of parameters from $_GET.
function precache_increase_read_no($mode_id, $root, $params){
	if($mode_id != 0){
		$file = fopen($root.'/read_no.txt', 'a');
		flock($file,LOCK_EX);
		fwrite($file, $params[1].' '.$mode_id."\n");
		flock($file,LOCK_UN);
		fclose($file);
	}
}

/// Extract information from $_GET into regular array (see get_param() in preinclude.inc).
/// @return Page mode and array of parameters.
function precache_get_param(){
	if(count($_GET) > 1 or isset($_GET['mode'])){
		$mode = $_GET['mode'];
		$params = array();
	}else{
		$params = array_keys($_GET);
		if(count($params)){
			$params = split('\/',$params[0]);
			$mode = '_'.$params[1].'_';
		}else{
			$mode = '_index_';
			$params[1] = 'index';
		}
	}
	return array($mode, $params);
}

/// Get ID of the page mode.
/// @param $params : array of parameters from $_GET.
function precache_get_mode_id($params){
	if(isset($params[2]) and ctype_digit($params[2])) $mode_id = $params[2];
	else $mode_id = 0;
	return $mode_id;
}

/// Get USER_ID from $_SESSION.
/// @return USER_ID.
function preacache_get_user_id(){
	if(isset($_SESSION['xblc_id'])) $uid = $_SESSION['xblc_id'];
	else $uid = -1;
	return $uid;
}
?>
