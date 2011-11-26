<?php
require_once('CustomURI.class.inc');
execute_add_function('customuri_map');
execute_add_function('customuri_post_render');

function customuri_map(){
	$called_uri = substr(vv('QUERY_STRING'), 1);
	$splitted = split('\/', $called_uri);
	
	if(in_array($splitted[0], cc('supported_language'))){
		$called_uri = substr($called_uri, strlen($splitted[0]) + 1);
		$language = '/'.$splitted[0];
	}else{
		$language = '';
	}
	
	$clause = "WHERE mapped_URI='$called_uri' AND section=".SECTION;
	$data = db_select_single_column('customuri', 'original_URI', $clause, false);
	if(count($data)){
		$_GET = array($language.'/'.$data[0] => '');
		$_SERVER['QUERY_STRING'] = $language.'/'.$data[0];
	}
}

function customuri_post_render($render){
	if(!document_has_form()){
		$clause = "WHERE section=".SECTION;
		$data = db_select('customuri', array('original_URI', 'mapped_URI'), $clause);
		$language = ss('language');
		if(strlen($language)) $language = $language.'/';
		else $language = '';
		foreach($data as $key => $value){
			$original = '"'.SECTION_URI.$value['original_URI'].'"';
			$mapped = '"'.SECTION_URI.$language.$value['mapped_URI'].'"';
			$render = str_replace($original, $mapped, $render);
			
			$original = '"'.SECTION_URI.$language.$value['original_URI'].'"';
			$mapped = '"'.SECTION_URI.$language.$value['mapped_URI'].'"';
			$render = str_replace($original, $mapped, $render);
		}
	}
	return $render;
}
?>
