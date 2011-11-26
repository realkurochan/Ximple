<?php
global $search_page;
$search_page = false;

function search_set_page(){
	global $search_page;
	$search_page = true;
}

function search_increase_point(){
	if(gg('search_keyword')){
		$query = 'UPDATE search SET points=points+1, clicks=clicks+1 WHERE id='.((int)gg('search_id'));
		db_modify('search', $query);
	}
}

function search_drop_index(){
	db_drop('search', gg('search_id'));
}

function search_add_source_page($content){
	global $search_page;
	if($search_page and MODE_ID != 0){
		$clause = 'WHERE '.search_get_where_clause(MODE, MODE_ID);
		$source = db_select('search_source', array('id'), $clause);
		if(count($source) == 0){
			$data = array(
				'title' => document_get_title(),
				'description' => document_get_description(),
				'keyword' => document_get_keywords(),
				'content' => input_trim(htmlspecialchars_decode(strip_tags($content))),
				'mode' => MODE,
				'mode_id' => MODE_ID,
				'insert_time' => NOW,
				'language' => translate_get_language(),
			);
			db_insert('search_source', $data);
		}
	}
}

function search_add_source($mode, $mode_id, $title, $description, $keywords, $content){
	$data = array(
		'title' => $title,
		'description' => $description,
		'keyword' => $keywords,
		'content' => input_trim(strip_tags($content)),
		'mode' => $mode,
		'mode_id' => $mode_id,
		'insert_time' => NOW,
		'language' => translate_get_language(),
	);
	db_insert('search_source', $data);
}

function search_drop_source($mode=MODE, $mode_id=MODE_ID){
	$clause = search_get_where_clause($mode, $mode_id);
	db_drop_many('search_source', $clause);
}

function search_update_source($mode, $mode_id, $title, $description, $keywords, $content){
	$data = array(
		'title' => $title,
		'description' => $description,
		'keyword' => $keywords,
		'content' => input_trim(strip_tags($content)),
		'mode' => $mode,
		'mode_id' => $mode_id,
		'insert_time' => NOW,
	);
	$clause = search_get_where_clause($mode, $mode_id);
	db_update_many('search_source', $data, $clause);
}

function search_get_where_clause($mode, $mode_id){
	$clause  = "mode='".$mode."' ";
	$clause .= "AND mode_id=".$mode_id." ";
	$clause .= "AND language='".translate_get_language()."'";
	return $clause;
}
?>
