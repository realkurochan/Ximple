<?php
/// @file
/// This file provides functions for highlight source code in content using GeSHi
/// (http://qbnz.com/highlighter/).
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Internal global variables for highlight-tool.
/// These variables should not be used outside this file.
global $hightlight_geshi_included;
$hightlight_geshi_included = false;

/// Highlight source code in content.
/// @param $content : content to highlight
/// @return Content with highlighted source code.
function highlight($content){
	$code_array = highlight_getcode($content);
	highlight_clear_cache();
	foreach($code_array as $key => $value){
		$cache_file = highlight_get_cache_file($value['tag']);
		if(is_file($cache_file)){
			$hightlighted = io_read_file($cache_file);
		}else{
			$hightlighted = highlight_execute($value);
			io_write_file($cache_file, $hightlighted);
		}
		$content = str_replace($value['tag'], $hightlighted, $content);
	}
	return $content;
}

/// Extract source code from content.
/// @param $content : content to extract
/// @return Array of extracted source code.
function highlight_getcode($content){
	preg_match_all("/<pre\s[^>]*>[^<]*<\/pre>/", $content, $preg_result);
	$code_array = array();
	foreach($preg_result[0] as $key => $value){
		preg_match('/lang="\w*"/', $value, $lang);
		$code = strip_tags($value);
		$code = str_replace("&lt;", "<", $code);
		$code = str_replace("&gt;", ">", $code);
		$code_array[] = array(
			'tag' => $value,
			'language' => substr($lang[0], 6, -1),
			'code' => $code,
		);
	}
	return $code_array;
}

/// Execute source code highlighting with GeSHi
/// @param $code : source code to highlight
/// @return Highlighted source code.
function highlight_execute($code){
	global $hightlight_geshi_included;
	if(!$hightlight_geshi_included){
		include_once(INCLUDE_PATH.'external/geshi/geshi.php');
		$hightlight_geshi_included = true;
	}
	$geshi = new GeSHi($code['code'], $code['language']);
	$geshi->enable_line_numbers(GESHI_FANCY_LINE_NUMBERS);
	return '<div class="code">'.$geshi->parse_code().'</div>';
}

/// Get cache directory for source code highlighting.
/// @return Cache directory for source code highlighting.
function highlight_get_cache_directory(){
	$directory = FILES_PATH.'highlight/';
	if(!is_dir($directory)) mkdir($directory);
	return $directory;
}

/// Get cache file for source code highlighting.
/// @return Cache file for source code highlighting.
function highlight_get_cache_file($code){
	$directory = highlight_get_cache_directory();
	return $directory.md5($code).'.php';
}

/// Delete all cache in directory.
function highlight_clear_cache(){
	$directory = highlight_get_cache_directory();
	$date_file = $directory.'lastclear.txt';
	cache_clear_by_day($date_file, $directory, 30);
}
?>
