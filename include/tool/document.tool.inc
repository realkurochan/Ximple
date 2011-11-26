<?php
/// @file
/// This file provides functions for document attribute setting and getting.
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Internal global variables for document-tool.
/// These variables should not be used outside this file.
global $document_title, $document_description;
global $document_keywords, $document_redirect;
global $document_is_back, $document_form, $document_path;
global $document_lock_set_path;

$document_lock_set_path = false;
$document_title = '';
$document_description = '';
$document_redirect = '';
$document_keywords = array();
$document_form = false;
$document_path = array();

/// Set title of document.
/// @param $title : title of document.
function document_set_title($title){
	global $document_title;
	$document_title = $title;
}

/// Get title of document.
/// @return Title of document.
function document_get_title(){
	global $document_title;
	return $document_title;
}

/// Set description of document.
/// @param $description : description of document.
function document_set_description($description){
	global $document_description;
	$document_description = $description;
}

/// Get description of document.
/// @return Description of document.
function document_get_description(){
	global $document_description;
	if(strlen($document_description)) return $document_description;
	else return cc('section_description');
}

/// Add keyword into document.
/// @param $keyword : keyword to add into document.
function document_add_keyword($keyword){
	global $document_keywords;
	if(!in_array($keyword, $document_keywords)){
		$document_keywords[] = $keyword;
	}
}

/// Add keywords into document.
/// @param $keywords : Array of keywords to add into document.
function document_add_keywords($keywords){
	global $document_keywords;
	$document_keywords = array_merge($document_keywords, $keywords); 
}

/// Get array of keywords of document.
/// @return Array of keywords of document.
function document_get_keywords(){
	global $document_keywords;
	$keywords = join(', ', $document_keywords);
	if(strlen($keywords)) return $keywords;
	else return cc('section_keywords');
}

/// Set target URI to redirect document.
/// @param $redirect : target URI.
function document_set_redirect($redirect){
	global $document_redirect;
	$document_redirect = $redirect;
}

/// Get target URI to redirect document.
/// @return Target URI.
function document_get_redirect(){
	global $document_redirect;
	return $document_redirect;
}

/// Set target URI of page redirection to previous URI.
function document_set_redirect_back(){
	global $document_is_back;
	$document_is_back = true;
}

/// Get target URI to redirect document.
/// @return Target URI.
function document_get_redirect_back(){
	global $document_is_back;
	return $document_is_back;
}

/// Register form to sytem.
function document_init_form(){
	global $document_form;
	$document_form = true;
}

/// Check form in document.
/// @return TRUE, if document has a form.
function document_has_form(){
	global $document_form;
	return $document_form;
}

/// Lock document to set path.
function document_lock_set_path(){
	global $document_lock_set_path;
	$document_lock_set_path = true;
}

/// Release document to set path.
function document_release_set_path(){
	global $document_lock_set_path;
	$document_lock_set_path = false;
}

/// Set navigation path of document to give
/// User information about structure of web-site.
function document_set_path($name, $uri=''){
	global $document_path;
	global $document_lock_set_path;
	if(!$document_lock_set_path){
		$document_path[$name] = $uri;
	}
}

/// Render navigation path of document.
/// @return Rendered navigation path.
function document_render_path(){
	global $document_path, $document_title;
	if(IS_INDEX) return '';
	$i = 0;
	$render = '';
	$size = count($document_path);
	$p = '<a href="%s">%s</a>';
	foreach($document_path as $name => $uri){
		if($i == 0){
			$render .= sprintf($p, SECTION_URI, $name);
		}elseif($i != $size-1){
			$render .= sprintf($p, SECTION_URI.Q.$uri, $name);
		}else{
			$render .= $name;
			if(strlen($document_title) == 0){
				$document_title = $name;
			}
		}
		if($i != $size-1){
			$render .= '&#160;&#187;&#160;';
		}
		$i++;
	}
	return $render;
}

?>
