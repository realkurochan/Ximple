<?php
/// @file 
/// This file provides functions for php.ini manipulation.
/// See http://php.net/manual/en/ini.core.php for more information about php.ini.
/// This file must be minimal, because it is loaded for all requests.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>
/// @state complete.

require('state.tool.inc');
require('globalvar.tool.inc');

/// Set php.ini of X-BLC
/// @param $section : SECION-ID
function setini(){
	define('STATE', state_read());
	if((state_is_product() or state_is_install() or state_is_maintain()) and false){
		ini_set('display_errors', 0);
	}else{
		ini_set('display_errors', 1);
		if(defined('E_DEPRECATED')){
			ini_set('error_reporting', E_ALL & ~E_DEPRECATED );
		}else{
			ini_set('error_reporting', E_ALL);
		}
	}
	
	ini_set('html_errors', 1);
	ini_set('session.use_only_cookies', 0);
	ini_set('session.name', 'PHPSESSID');
	ini_set('safe_mode', 1);
	ini_set('post_max_size', '40M');
	ini_set('upload_max_filesize', '40M');
}

?>
