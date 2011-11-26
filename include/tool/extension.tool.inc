<?php
/// @file
/// This file provides functions for execution of extensions.
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Extension is the further concept to make Ximple more extensible.
/// Unlike regular module, which is called from outside of XimpleCMS,
/// extensions are called from part of process in XimpleCMS.
/// In other words, we can inject some functions inside the code.
/// Hence, core of XimpleCMS can also be extended.
/// Moreover, extensions can also be extended.
/// Security concept of extension is the same as regular module.

/// Execute given extension.
/// @param $reference : reference name of extension.
/// @param $params : array of parameters for called function or method
/// @return Result from extension.
function extension_execute($reference, $params=null){
	$extension = cc('extension');
	if(isset($extension[$reference])){
		foreach($extension[$reference] as $key => $value){
			$params = execute_string($value, $params);
		}
	}
	return $params;
}

/// Execute given extension.
/// @param $reference : reference name of extension.
/// @param $params : array of parameters for called function or method
/// @param $result : result from called function or method
/// @return Result from extemsion.
function extension_execute_append_result($reference, $params, $result){
	$extension = cc('extension');
	if(isset($extension[$reference])){
		foreach($extension[$reference] as $key => $value){
			$result = execute_string($value, $params, $result);
		}
	}
	return $result;
}

/// Add extension to given reference.
/// @param $reference : reference name of extension.
/// @param $extension_name : name of extension (XimpleMODE).
function extension_add($reference, $extension_name){
	$extension = cc('extension');
	if(!is_array($extension)){
		$extension = array();
	}
	if(!isset($extension[$reference])){
		$extension[$reference] = array();
	}
	if(!in_array($extension_name, $extension[$reference])){
		$extension[$reference][] = $extension_name;
	}
	config_set('extension', $extension);
}

/// Drop extension from system.
/// @param $reference : reference name of extension.
/// @param $extension_name : name of extension (XimpleMODE).
function extension_drop($reference, $extension_name){
	$extension = cc('extension');
	if(!is_array($extension)){
		$extension = array();
	}
	if(isset($extension[$reference])){
		foreach($extension[$reference] as $key => $value){
			if($value == $extension_name){
				unset($extension[$reference][$key]);
				break;
			}
		}
	}
	config_set('extension', $extension);
}

?>
