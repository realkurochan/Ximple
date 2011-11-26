<?php
execute_add_function('translate_set_language');
execute_add_function('translate_unset_language');

global $translate_directory;
global $translate_is_prepare_central;

$translate_directory = array();
$translate_is_prepare_central = false;

function translate_define_language(){
	if(!defined('LANGUAGE')){
		$language = ss('language');
		if($language){
			define('LANGUAGE', $language);
		}else{ 
			$language = uu('language');
			if($language) define('LANGUAGE', $language);
			else define('LANGUAGE', DEFAULT_LANGUAGE);
		}
	}
	translate_add_directory(ROOT_PATH.'include');
}

function translate_get_interface_language(){
	$language = array(
		'en' => 'English',
		'de' => 'Deutsch',
		'th' => 'ไทย',
		'jp' => 'Japanese',
	);
	return $language;
}

function translate_get_language(){
	$language = ss('language');
	if(strlen($language) == 0){
		$language = cc('supported_language');
		if(is_array($language) and count($language)){	
			$language = $language[0];
		}else{
			$language = 'DEFAULT';
		}
	}
	return $language;
}

function translate_unset_language(){
	unset($_SESSION['language']);
	execute_set_db();
	document_set_redirect('');
}

function translate_set_language(){
	require_once('country.tool.inc');
	$_SESSION['language'] = gg('language');
	$country = country();
	$language = $country[gg('language')];
	$message  = sprintf(tt('Contents are set to %s language. '), $language);
	$message .= tt('Only translated contents will be shown. ');
	$message .= tt('Other cases, the original content will be shown.');
	notify_add_info($message);
	execute_set_db();
	document_set_redirect('');
}

function translate_content($content, $module, $module_id){
	$langauge = translate_get_language();
	if(strlen($langauge)){
		$translated = translate_get_published_data($module, $module_id, $langauge);
		if(isset($translated['id'])){
			unset($translated['id']);
			unset($translated['status']);
			$result = array();
			foreach($content as $key => $value){
				if(isset($translated[$key])) $result[$key] = $translated[$key];
				else $result[$key] = $value;
			}
			return $result;
		}else{
			return $content;
		}
	}else{
		return $content;
	}
}

function translate_get_published_data($module, $module_id, $langauge){
	$clause = "WHERE module='$module' AND module_id='$module_id' AND status=1 AND language='$langauge'";
	return translate_get_data($clause);
}

function translate_get_all_data($module, $module_id, $language){
	$clause = "WHERE module='$module' AND module_id='$module_id' AND language='$language'";
	return translate_get_data($clause);
}

function translate_get_data($clause){
	$column = array('id', 'translation', 'status');
	$data = db_select('translation', $column, $clause);
	if(count($data)){
		$data = array_values($data);
		if(strlen($data[0]['translation'])){
			$translation = unserialize($data[0]['translation']);
			$translation['id'] = $data[0]['id'];
			$translation['status'] = $data[0]['status'];
			return $translation;
		}
	}
	return array('status' => 0);
}

function translate_set_data($translation){
	$data = array(
		'status' => $translation['status'],
		'module' => $translation['module'],
		'module_id' => $translation['module_id'],
		'language' => $translation['language'],
		'translation' => serialize(input_strip_slash_array($translation)),
	);
	if(isset($translation['id'])){
		db_update('translation', $data, $translation['id']);
	}else{
		db_insert('translation', $data);
	}
}

function translate_add_directory($directory){
	global $translate_directory;
	if(defined('LANGUAGE')){
		translate_prepare_central();
		$translate_directory[] = $directory;
		$locale_file = $directory.'/locale/'.LANGUAGE.'.locale.inc';
		if(is_file($locale_file)){
			require_once($locale_file);
		}
	}
}

function translate_prepare_central(){
	global $translate_is_prepare_central;
	if(!$translate_is_prepare_central){
		$translate_is_prepare_central = true;
		$file_name = FILES_PATH.'locale/'.SECTION.'/'.LANGUAGE;
		globalvar_add_locale(io_unserialize($file_name));
	}
}

function translate_save_central($text){
	global $translate_directory;
	global $locale;
	$directory = translate_get_central_directory();
	$central_locale = io_unserialize($directory.LANGUAGE);
	if(!isset($central_locale[$text])){
		$original_locale = $locale;
		$languages = translate_get_interface_language();
		foreach($languages as $language => $label){
			$locale = array();
			foreach($translate_directory as $locale_directory){
				$locale_file = $locale_directory.'/locale/'.$language.'.locale.inc';
				if(is_file($locale_file)) require($locale_file);
			}
			if(isset($locale[$text])){
				$central_locale = io_unserialize($directory.$language);
				$central_locale[$text] = $locale[$text];
				io_serialize($central_locale, $directory.$language);
			}
		}
		$locale = $original_locale;
	}
}

function translate_get_central_directory(){
	$directory = FILES_PATH.'locale/';
	if(!is_dir($directory)) mkdir($directory);
	$directory.= SECTION.'/';
	if(!is_dir($directory)) mkdir($directory);
	return $directory;
}

function translate_date_th($date){
	$date = date('j F ', strtotime($date));
	$year = (date('Y', strtotime($date)) + 543);
	$date = $date.substr($year,2,2);
	$number = array(
		'0' => '๐',
		'1' => '๑',
		'2' => '๒',
		'3' => '๓',
		'4' => '๔',
		'5' => '๕',
		'6' => '๖',
		'7' => '๗',
		'8' => '๘',
		'9' => '๙',
	);
	$month = array(
		'January' => 'ม.ค.',
		'February' => 'ก.พ.',
		'March' => 'มี.ค.',
		'April' => 'เม.ย.',
		'May' => 'พ.ค.',
		'June' => 'มิ.ย.',
		'July' => 'ก.ค.',
		'August' => 'ส.ค.',
		'September' => 'ก.ย.',
		'October' => 'ต.ค.',
		'November' => 'พ.ย.',
		'December' => 'ธ.ค.',
	);
	foreach($number as $key => $value){
		$date = str_replace($key, $value, $date);
	}
	
	foreach($month as $key => $value){
		$date = str_replace($key, $value, $date);
	}
	return $date;
}
?>
