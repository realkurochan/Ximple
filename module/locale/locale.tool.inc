<?php
function locale_get($directory, $language){
	if(strlen($directory)){
		$locale = array();
		$file = locale_get_file(ROOT_PATH.$directory, array());
		$key = locale_get_key($file);
		locale_add_predefined($language);
		$key = locale_check_exist($key);
		return locale_render_code($directory, $language, $key);
	}
}

function locale_render_code($directory, $language, $key){
	if(count($key)){
		$locale = locale_get_processed($directory, $language, $key);
		return "&lt;?php\nglobalvar_add_locale(".input_dump_var($locale).");\n?&gt;";
	}
}

function locale_get_file($directory, $file){
	if(is_dir($directory)){
		$entry = io_get_directory_entry($directory);
		foreach($entry as $key => $value){
			$path = $directory.'/'.$value;
			if(is_dir($path)) $file = locale_get_file($path, $file);
			else $file[] = $path;
		}
	}
	return $file;
}

function locale_get_key($file){
	$key = array();
	foreach($file as $value){
		$code = io_read_file($value);
		$key  = array_merge($key, locale_regular_expression($code));
	}
	return array_unique($key);
}

function locale_regular_expression($code){
	$key = array();
	$code = str_replace('\"', '{backslash_double}', $code);
	$code = str_replace("\\'", '{backslash_single}', $code);
	preg_match_all("/[\W](tt)[\s]*\([\s]*\'[^\']*\'[\s]*\)/", $code, $out);
	foreach($out[0] as $tt){
		$tt = substr($tt, 3);
		$tt = preg_replace("/[\s]*\([\s]*\'/", "", $tt);
		$tt = preg_replace("/\'[\s]*\)/", "", $tt);
		$tt = str_replace('{backslash_single}', "\\'", $tt);
		$key[] = $tt;
	}

	preg_match_all('/[\W](tt)[\s]*\([\s]*\"[^\"]*\"[\s]*\)/', $code, $out);
	foreach($out[0] as $tt){
		$tt = substr($tt, 3);
		$tt = preg_replace('/[\s]*\([\s]*\"/', "", $tt);
		$tt = preg_replace('/\"[\s]*\)/', "", $tt);
		$tt = str_replace('{backslash_double}', "\\\"", $tt);
		$key[] = $tt;
	}
	return $key;
}

function locale_add_predefined($language){
	$predefined = array(
		INCLUDE_PATH,
		MODULE_PATH.'content',
	);
	foreach($predefined as $key => $value){
		$file = $value.'locale/'.$language.'.locale.inc';
		if(is_file($file)) require_once($file);
	}
}

function locale_check_exist($key){
	global $locale;
	return array_diff($key, array_keys($locale));
}

function locale_get_processed($directory, $language, $key){
	global $locale;
	$processed = array();
	$file = $directory.'/locale/'.$language.'.locale.inc';
	if(is_file($file)) require_once($file);
	foreach($key as $tt){
		if(isset($locale[$tt])) $processed[$tt] = $locale[$tt];
		else $processed[$tt] = '';
	}
	return $processed;
}

function locale_init(){
	$modules = io_get_directory_entry(MODULE_PATH);
	foreach($modules as $module){
		locale_init_module($module);
	}
}

function locale_get_module(){
	$system = io_get_directory_entry(MODULE_PATH);
	$system = array_combine($system, $system);
	$system = locale_check_directory($system, MODULE_PATH);
	
	$private_path = SECTION_PATH.'private/module/';
	$private = io_get_directory_entry($private_path);
	$private = array_combine($private, $private);
	$private = locale_check_directory($private, $private_path);
	
	$module = array_merge($system, $private);
	ksort($module);
	$select = array('' => tt('Please, select module.'));
	$module = array_merge($select, $module);
	return $module;
}

function locale_get_theme(){
	$system = io_get_directory_entry(THEME_PATH);
	$system = array_combine($system, $system);
	$system = locale_check_directory($system, THEME_PATH);
	
	$private_path = SECTION_PATH.'private/theme/';
	$private = io_get_directory_entry($private_path);
	$private = array_combine($private, $private);
	$private = locale_check_directory($private, $private_path);
	
	$theme = array_merge($system, $private);
	ksort($theme);
	$select = array('' => tt('Please, select theme.'));
	$theme = array_merge($select, $theme);
	return $theme;
}

function locale_init_module($module){
	$locale_directory = MODULE_PATH.$module.'/locale/';
	if(is_dir(MODULE_PATH.$module)){
		$locale_directory = MODULE_PATH.$module.'/locale/';
		$code_directory = 'module/'.$module;
	}else{
		$locale_directory = SECTION_PATH.'private/module/'.$module.'/locale/';
		$code_directory = 'private/module/'.$module;
	}
	locale_init_directory($locale_directory, $code_directory);
}

function locale_init_theme($theme){
	$locale_directory = THEME_PATH.$theme.'/locale/';
	if(is_dir(THEME_PATH.$theme)){
		$locale_directory = THEME_PATH.$theme.'/locale/';
		$code_directory = 'theme/'.$theme;
	}else{
		$locale_directory = SECTION_PATH.'private/theme/'.$theme.'/locale/';
		$code_directory = 'private/theme/'.$theme;
	}
	locale_init_directory($locale_directory, $code_directory);
}

function locale_check_directory($directory, $parent){
	foreach($directory as $key => $value){
		if(!is_dir($parent.$value)) unset($directory[$key]);
	}
	return $directory;
}

function locale_init_directory($locale_directory, $code_directory){
	$languages = translate_get_interface_language();
	echo($locale_directory);
	if(!is_dir($locale_directory)) mkdir($locale_directory);
	foreach($languages as $language => $label){
		$code = locale_get($code_directory, $language);
		$code = str_replace('&lt;', '<', $code);
		$code = str_replace('&gt;', '>', $code);
		if(strlen($code)){
			$file = $locale_directory.$language.'.locale.inc';
			io_write_file($file, $code);
		}
	}
}
?>
