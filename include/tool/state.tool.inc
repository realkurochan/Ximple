<?php
/// @file 
/// This file provides functions related to state of section.
/// This file must be minimal, because it is loaded for all requests.
/// Section state in Ximple :
///		- install  : Ximple is not installed on system
///		- product  : Site is ready to be published
///		- maintain : Site is maintained and all contents are hided, only admin-page can be accessed.
///		- develop  : Develop mode of Ximple, all error notices is shown.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>
/// @state complete.

/// Is section is develop state?
/// @return TRUE, if section is develop state.
function state_is_develop(){
	return STATE == 0;
}

/// Is section is product state?
/// @return TRUE, if section is product state.
function state_is_product(){
	return STATE == 1;
}

/// Is section is maintenance state?
/// @return TRUE, if section is maintenance state.
function state_is_maintain(){
	return STATE == 2;
}

/// Is section is install state?
/// @return TRUE, if section is install state.
function state_is_install(){
	return STATE == 3;
}

/// Is section is test state?
/// @return TRUE, if section is test state.
function state_is_test(){
	return STATE == 4;
}

/// Read section state from state file.
/// @param $section : SECTION-ID
/// @return ID of section state.
function state_read(){
	$state_dir = PRE_DEFINED_ROOT_PATH.'/files/state/';
	if(!is_file($state_dir.'install')) return 3;
	
	$state_dir .= SECTION.'/';
	if(is_file($state_dir.'develop')) return 0;
	if(is_file($state_dir.'product')) return 1;
	if(is_file($state_dir.'maintain')) return 2;
	if(is_file($state_dir.'test')) return 4;
	
	return 1;
}

?>
