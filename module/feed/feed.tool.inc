<?php
function feed_insert($mode, $data){
	if(in_array($mode, cc('feed_mode'))){
		db_insert('feed', feed_format_data($mode, $data));
		feed_render();
	}
}

function feed_update($mode, $data){
	if(in_array($mode, cc('feed_mode'))){
		$clause = 'WHERE mode_id='.$data['id']." AND mode='".$mode."'";
		$feed_id = db_select_single_column('feed', 'id', $clause, false);
		if(count($feed_id)){
			db_update('feed', feed_format_data($mode, $data), $feed_id[0]);
		}else{
			db_insert('feed', feed_format_data($mode, $data));
		}
		feed_render();
	}
}

function feed_drop($mode, $id){
	if(in_array($mode, cc('feed_mode'))){
		db_drop_many('feed', 'mode_id='.$id." AND mode='".$mode."'");
		feed_render();
	}
}

function feed_render(){
	$feed_column = array(
		'mode', 'mode_id', 'topic', 'summary', 'update_time',
		'publish_time', 'writer', 'comment_no', 'uri', 'freetag'
	);
	$clause = "WHERE section=".SECTION." ORDER BY update_time DESC LIMIT ".cc('feed_number');
	$data = db_select('feed', $feed_column, $clause, false);
	$data = feed_format_render_data($data);
	if(in_array('rss', cc('feed_type'))) feed_render_rss($data);
	if(in_array('atom', cc('feed_type'))) feed_render_atom($data);
}


function feed_render_rss($data){
	$render = render(array('data' => $data), 'module/feed/theme/rss.tpl', 'rss_feed');
	$feed_dir = feed_get_dir();
	io_write_file($feed_dir.'rss.xml', $render);
}

function feed_render_atom($data){
	$render = render(array('data' => $data), 'module/feed/theme/atom.tpl', 'atom_feed');
	$feed_dir = feed_get_dir();
	io_write_file($feed_dir.'atom.xml', $render);
}

function feed_format_render_data($data){
	$data_out = array();
	$i = 0;
	foreach($data as $key => $value){
		$data_out[$i] = $value;
		$data_out[$i]['publish_time'] = date('r', strtotime($value['publish_time']));
		$data_out[$i]['update_time'] = date('r', strtotime($value['publish_time']));
		$data_out[$i]['freetag'] = input_split_comma($value['freetag']);
		$data_out[$i]['summary'] = stripslashes($value['summary']);
		$data_out[$i]['id'] = $value['mode_id'];
		$i++;
	}
	return $data_out;
}

function feed_format_data($mode, $data){
	$feed_column = array('topic', 'summary', 'update_time', 'publish_time', 'writer', 'comment_no', 'uri', 'freetag');
	$data_out = array();
	$data_out['mode'] = $mode;
	$data_out['mode_id'] = $data['id'];
	foreach($feed_column as $column){
		if(isset($data[$column])) $data_out[$column] = $data[$column];
	}
	$data_out['section'] = SECTION;
	if(!isset($data_out['comment_no'])) $data_out['comment_no'] = 0;
	return $data_out;
}

function feed_get_dir(){
	$feed_dir = FILES_PATH.'xml/';
	if(!is_dir($feed_dir)) mkdir($feed_dir);
	$feed_dir.= SECTION.'/';
	if(!is_dir($feed_dir)) mkdir($feed_dir);
	return $feed_dir;
}
?>
