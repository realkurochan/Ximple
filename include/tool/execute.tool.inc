<?php
/// @file
/// This file provides functions for execution of called functions using XimpleMODE and XimpleURI.
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// XimpleMODE is a string to reference any callable functions or object methods in XimpleCMS.
/// XimpleMODE is composition of three string parts separated by under scroll sign(_).
/// For example : blog_write, gallery_config_save.
/// The first part give us module name of the called functions.
/// The second part is name of the module extension (optional).
/// For modules with multiple classes, module extension is very useful.
/// There is one main class without extension.
/// Other classes can be referenced by its extension.
/// The last part of XimpleMODE is function reference.

/// XimpleURI is the string for function execution.
/// It gives XimpleMODE to reference function and other parts to give function parameters.
/// Pattern : XimpleMODE/ID/parameter_key/parameter_value/ parameter_key/parameter_value/...
/// Note that ID is optional.

/// Security Concept :
/// Only registered function (per execute_add_function())
/// and class method (per execute_add_class() and execute_add_extended_class()) can be executed.
/// See more : class/Authority.class.inc

/// Internal global variables for execute-tool.
/// These variables should not be used outside this file.
global $execute_callable, $execute_mode;
global $execute_class, $execute_class_extension;

$execute_callable = array();
$execute_class_extension = array();
$execute_class = array();
$execute_mode = 0;

/// Execute given XimpleMODE.
/// @param $mode : given XimpleMODE
/// @param $params : array of parameters for called function or method
/// @param $result : result from called function or method
/// @return Result from called function or method.
function execute($mode, $params=null, $result=null){
	global $execute_callable;
	list($module, $extension) = execute_get_module($mode);
	execute_include_files($module, $extension);
	if(!execute_is_callable($mode)) error_denied();
	if(function_exists($mode) and in_array($mode, $execute_callable)){
		return $mode($params, $result);
	}else{
		return execute_oop($mode, $module, $params, $result);
	}
}

/// Execute given XimpleURI.
/// @param $uri : given XimpleURI
/// @param $params : array of parameters for called function or method
/// @param $result : result from called function or method
/// @return Result from called function or method
function execute_string($uri, $params=null, $result=null){
	$uri = execute_parse_smart_uri('/'.$uri);
	globalvar_set_get($uri);
	document_lock_set_path();
	$result = execute($uri['mode'], $params, $result);
	globalvar_reset_get();
	document_release_set_path();
	return $result;
}

/// Execute object method by given XimpleMODE.
/// @param $mode : given XimpleMODE
/// @param $module : name of module containing called object.
/// @param $params : array of parameters for called method
/// @param $result : result from called method
/// @return Result from called  method
function execute_oop($mode, $module, $params, $result){
	$class = execute_get_class($mode, $module);
	$success = false;
	if($class and class_exists($class)){
		$obj = new $class();
		if(!is_a($obj, 'Authority')){
			error_denied();
		}else{
			$method = $obj->getAction($mode);
			$obj->prepareMethod($method);
			if(!$obj->isCallable($method)){
				error_denied();
			}elseif($method){
				$result = $obj->$method($params, $result);
				$success = true;
			}
		}
	}
	if($success === false) error_not_found('System can not execute : '.$mode);
	return $result;
}

/// Add function as executable function.
/// @param $mode : name of function
function execute_add_function($mode){
	global $execute_callable;
	if(!in_array($mode, $execute_callable)){
		$execute_callable[] = $mode;
	}
}

/// Add class as executable class.
/// @param $module : name of module containing called object
/// @param $class : name of class
function execute_add_class($module, $class){
	global $execute_class;
	require_once(INCLUDE_PATH.'class/Control.class.inc');
	if(!isset($execute_class[$module])){
		$execute_class[$module] = $class;
	}elseif(state_is_develop()){
		$exist_class = $execute_class[$module];
		notify_add_error('Module : '.$module.' has already defined a class : '.$exist_class);
	}
}


/// Add class as executable class with module extension.
/// @param $module : name of module containing called object
/// @param $extension : name of module extension
/// @param $class : name of class
function execute_add_extended_class($module, $extension, $class){
	global $execute_class_extension;
	require_once(INCLUDE_PATH.'class/Control.class.inc');
	if(isset($execute_class_extension[$module])){
		if(!isset($execute_class_extension[$module][$extension])){
			$execute_class_extension[$module][$extension] = $class;
		}elseif(state_is_develop()){
			$exist_class = $execute_class[$module];
			notify_add_error('Module : '.$module.' has already defined a class : '.$exist_class);
		}
	}else{
		$execute_class_extension[$module] = array($extension => $class);
	}
	
}

/// Get class, which is mapped to given XimpleMODE.
/// @param $mode : given XimpleMODE
/// @param $module : name of module containing called object
function execute_get_class($mode, $module){
	global $execute_class;
	global $execute_class_extension;
	$splitted =  split('_', $mode);
	if(count($splitted) > 1){
		if(isset($execute_class_extension[$module][$splitted[1]])){
			return $execute_class_extension[$module][$splitted[1]];
		}
	}
	
	if(isset($execute_class[$module])){
		return $execute_class[$module];
	}else{
		return null;
	}
}

/// Set execution as Site (regular web site).
function execute_set_site(){
	global $execute_mode;
	$execute_mode = 0;
}

/// Set execution as Database Handler (show nothing only redirect).
function execute_set_db(){
	global $execute_mode;
	$execute_mode = 1;
}

/// Set execution as Ajax Site (no additional part like menu or widget).
function execute_set_ajax(){
	global $execute_mode;
	$execute_mode = 2;
}

/// Set execution as File (special HTTP header).
function execute_set_file(){
	global $execute_mode;
	$execute_mode = 3;
}

/// Set execution as Print Page (with print style sheet and print script).
function execute_set_print(){
	global $execute_mode;
	$execute_mode = 4;
}

/// Check execution as Site.
/// @return TRUE, if execution is set as Site.
function execute_as_site(){
	global $execute_mode;
	return $execute_mode == 0;
}

/// Check execution as Database Hanler.
/// @return TRUE, if execution is set as Database Hanler.
function execute_as_db(){
	global $execute_mode;
	return $execute_mode == 1;
}

/// Check execution as Ajax.
/// @return TRUE, if execution is set as Ajax.
function execute_as_ajax(){
	global $execute_mode;
	return $execute_mode == 2;
}

/// Check execution as File.
/// @return TRUE, if execution is set as File.
function execute_as_file(){
	global $execute_mode;
	return $execute_mode == 3;
}

/// Check execution as Print.
/// @return TRUE, if execution is set as Print.
function execute_as_print(){
	global $execute_mode;
	return $execute_mode == 4;
}

/// Check given function name callable.
/// @param $mode : name of function.
/// @return TRUE, if function name is callable.
function execute_is_callable($mode){
	global $execute_callable;
	return in_array($mode, $execute_callable) or 1;
}

/// Get module name from given XimpleMODE.
/// @param $mode : XimpleMODE.
/// @return Name of module.
function execute_get_module($mode){
	$split = split('_', $mode);
	if(count($split) > 1) return array($split[0], $split[1]);
	else return array($split[0], null);
}

/// Include required files for execution.
/// @param $module : name of module containing called object
/// @param $extension : name of module extension
function execute_include_files($module, $extension){
	$module_require = cc('module_require');
	if(isset($module_require[$module])){
		$module_require = $module_require[$module];
		$module_directory = execute_get_module_directory($module);
		translate_add_directory($module_directory);
		if(!is_array($module_require)){
			execute_include($module_directory.$module_require, $module);
		}elseif(isset($module_require[$extension])){
			execute_include($module_directory.$module_require[$extension], $module);
		}else{
			execute_include($module_directory.$module_require['#primary'], $module);
		}
	}
}


/// Get directory for given module.
/// @param $module : given module name
/// @return Directory for given module.
function execute_get_module_directory($module){
	if(is_dir(MODULE_PATH.$module.'/')){
		$module_directory = MODULE_PATH.$module.'/';
	}else{
		$module_directory = SECTION_PATH.'private/module/'.$module.'/';
	}
	return $module_directory;
}

/// Low level of including required file for execution.
/// @param $require : absolute path of required file.
function execute_include($require, $module){
	if(is_file($require)){
		require_once($require);
	}elseif(state_is_develop()){
		error_syslog('Error execute_include_files by module : '.$module.', file : '.$require);
	}else{
		error_not_found('System can not include : '.$require);
	}
}

/// Parse requested URI to parameter array.
/// @param $uri : requested URI.
function execute_parse_smart_uri($uri){
	$result = array();
	$splitted = split('\/',$uri);
	$size = count($splitted);
	$result['mode'] = $splitted[1];
	$laguange = cc('supported_language');
	if(is_array($laguange) and in_array($result['mode'], $laguange)){
		$_SESSION['language'] = $result['mode'];
		$result['mode'] = $splitted[2];
		$splitted = array_slice($splitted, 1);
		$size = count($splitted);
	}
	
	if($size >= 3){
		if(ctype_digit($splitted[2])){
			$result['id'] = $splitted[2];
			$begin = 3;
		}else{
			$result['id'] = '0';
			$begin = 2;
		}
		for($i=$begin ; $i <= $size ; $i+=2){
			if(isset($splitted[$i+1])){
				$result[$splitted[$i]] = $splitted[$i+1];
			}
		}
	}
	return $result;
}
?>
