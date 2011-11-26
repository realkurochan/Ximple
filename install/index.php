<?php
/// @file
/// This file provides installation-script for Ximple.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>

include('install.tool.inc');
define('IS_APC', function_exists('apc_add'));
define('PRE_DEFINED_ROOT_PATH', dirname(dirname(__FILE__)).'/');
define('SECTION', 1);
define('MODE', 'install');
define('MODE_ID', 0);
install_boot();

function install_boot(){
	session_start();
	include_once(PRE_DEFINED_ROOT_PATH.'include/tool/setini.tool.inc');
	setini();
	header('Content-Type: text/html; charset=UTF-8');
	install_check_setup();
	define('THEME_URI', ROOT_URI.'theme/default/');
	install_set_path();
	install_main();
}

/// Check the existence of setup.php.
/// If setup.php does not exist, the constants are temporally defined.
function install_check_setup(){
	global $agree;
	install_set_lang();
	$step = (int) substr((string) gg('mode'), -1, 1);
	if(!is_file(dirname(__FILE__).'/../include/setup.php')){
		install_error_setup_not_exist();
	}elseif($step < 2){
		$root_uri = 'http://'.$_SERVER['HTTP_HOST'].dirname(dirname($_SERVER['PHP_SELF'])).'/';
		$root_path = dirname(dirname($_SERVER['SCRIPT_FILENAME']));
		define('INSTALL_ROOT_URI', $root_uri);
		define('INSTALL_ROOT_PATH', $root_path);
		require_once('../include/setup.php');
	}else{
		require_once('../include/setup.php');
		define('INSTALL_ROOT_URI', ROOT_URI);
		define('INSTALL_ROOT_PATH', ROOT_PATH);
	}
}

/// Main function for Ximple installation.
/// @param $warn : warning message from check_setup().
function install_main(){
	install_define_constant();
	if(isset($_GET['mode'])) $function = 'install_'.$_GET['mode'];
	else $function = 'install_step0';
	if(!function_exists($function)) list($title, $body) =install_step0();
	else list($title, $body) = $function();
	$select_langauge = install_language_selection();
	$step = install_step_render();
	include('template/install.tpl');
}

/// Step 0 of installation : License agreement and check existence of setup.php.
function install_step0(){
	$handle = fopen('../LICENSE.txt', 'r');
	$license = fread ($handle, filesize ('../LICENSE.txt'));
	fclose($handle);
	$redner = '<h2>Step 0 : '.tt('License agreement').'</h2><pre>'.$license.'</pre>';
	$redner .=  '
	<form action="index.php" method="get">
		<input type="hidden" name="mode" value="step1"/>
		<input type="submit" value="'.tt('I agree').'">
	</form>';
	return array('Step 0 : '.tt('License agreement'), $redner);
}

/// Step 1 of installation : check constants in setup.php.
function install_step1(){
	$page = '<h2>Step 1 : '.tt('Setup validation').'</h2>';
	$page.= '<p>';
	
	list($next_enable_1, $result) = install_check_database();
	$page .= $result;
	list($next_enable_2, $result) = install_check_path();
	$page .= $result;
	list($next_enable_3, $result) = install_check_uri();
	$page .= $result;
	list($next_enable_4, $result) = install_check_lang();
	$page .= $result;
	
	if(!($next_enable_1 and $next_enable_2 and $next_enable_3 and $next_enable_4)){
		$next_enable = 'disabled="disabled"';
		$try = '<input type="button" value="'.tt('Try again').'" onclick="location.reload(true)">';
		$messsage = tt('You have to edit <b>%s</b> to correct invalid constants.');
		$page.= '<p>'.sprintf($messsage, INCLUDE_PATH.'setup.php').'</p>';
	}else{
		$next_enable = '';
		$try = '';
	}
	$next_step = 2;
	$page .= '</p>'.install_render_next_step($next_step, $next_enable, $try);
	return array('Step 1 : '.tt('Setup validation'), $page);
}

/// Step 2 of installation : check files and folders permissions.
function install_step2(){
	$title = 'Step 2 : '.tt('Permission validation');
	$page = '<h2>'.$title.'</h2>';
	$path = array('files', 'section');
	$is_windows = (strtoupper(substr(php_uname('s'), 0, 3)) == 'WIN');
	foreach($path as $item){
		$item = ROOT_PATH.$item;
		if($is_windows){
			if(fileperms($item) & 0x0002){
				$page.='<b>'.$item.' :</b> <b class="valid">OK</b><br />';
				$next_enable = '';
			}else{
				$page.='<b>'.$item.' :</b> <b class="invalid">Invalid !!! Please make sure that the directory can be modified.</b><br />';
				$next_enable = 'disabled="disabled"';
			}
		}else{
			$owner = posix_getpwuid(posix_geteuid());
			$owner = $owner['uid'];
			$is_mode = (substr(sprintf('%o', fileperms($item)), -4) == '1777');
			$is_owner = (substr(sprintf('%o', fileperms($item)), -4) == '0755' and fileowner($item) == $owner);
			if($is_mode or $is_owner){
				$page.='<b>'.$item.' :</b> <b class="valid">OK</b><br />';
				$next_enable = '';
			}else{
				$page.='<b>'.$item.' :</b> <b class="invalid">Invalid !!! Please change to 1777 or (mod : 0755, owner : '.$owner.')</b><br />';
				$next_enable = 'disabled="disabled"';
			}
		}
	}
	
	if($next_enable != 'disabled="disabled"'){
		$to_mkdir = install_directory_list();
		foreach($to_mkdir as $item){
			if(!is_dir(ROOT_PATH.$item)) mkdir(ROOT_PATH.$item);
		}
		$des = FILES_PATH.'misc/ximple.png';
		if(!is_file($des)) copy(FILES_PATH.'ximple.png', $des);
		$des = FILES_PATH.'avatar/default.png';
		if(!is_file($des)) copy(FILES_PATH.'default.png', $des);
	}
	
	if($next_enable != 'disabled="disabled"') $try = '';
	else $try = '<input type="button" value="'.tt('Try again').'" onclick="location.reload(true)">';
	
	$next_step = 3;
	$page.= '</p><br />'.install_render_next_step($next_step, $next_enable, $try);
	return array($title, $page);
}

/// Step 3 of installation : render form to get root-login information.
function install_step3(){
	$title = 'Step 3 : '.tt('Root-Login information');
	install_create_db_table();
	ob_start();
	require('template/user.tpl');
	$page = ob_get_contents();
	ob_end_clean();
	return array($title, $page);
}

/// Step 4 of installation : initial system and get minimal configuration.
function install_step4(){
	$title = 'Step 4 : '.tt('Configuration');
	install_insert_user();
	install_login();
	$page  = '<h2>'.$title.'</h2>'.install_section_config_page();
	return array($title, $page);
}

/// Step 5 of installation : save configuration and run pre-configuration script.
function install_step5(){
	include('preconfig.inc');
	install_init_section();
	install_init_authority();
	$function_name = 'preconfig_'.pp('preconfig');
	$function_name();
	widget_init_default(true);
	$title = tt('Happy Ending').' : '.tt('Ximple has been successfully installed');
	$page  = '<h2>'.$title.'</h2>';
	$page .= install_finish_message();
	io_write_file(FILES_PATH.'state/install', "");
	$state_dir = ROOT_PATH.'/files/state/1/';
	if(!is_dir($state_dir)) mkdir($state_dir);
	io_write_file($state_dir.'product', "");
	return array(tt('Happy Ending'), $page);
}
?>
