<?php
execute_add_function('search');

function search(){
	mb_internal_encoding('utf8');
	$keywords = search_check_keyword();
	$title = tt('search keyword : ').gg('search');
	document_set_path($title);
	document_set_title($title);
	if(count($keywords)){
		$update_time = search_get_update_time($keywords);
		$matched = search_get_match($keywords, $update_time);
		$points = search_calculate_point($keywords, $matched);
		search_update_index($keywords, $matched, $points);
		return search_render_result($keywords);
	}else{
		return '<h3 class="title_label">'.tt('No result').'</h3>';
	}
}

function search_check_keyword(){
	if(strlen(gg('search'))){
		$keywords = split(' ', input_trim(gg('search')));
		foreach($keywords as $key => $keyword){
			if(mb_strlen($keyword) <= 2){
				unset($keywords[$key]);
			}
		}
		return $keywords;
	}else{
		return array();
	}
}

function search_render_result($keywords){
	include_once(INCLUDE_PATH.'class/PagerRender.class.inc');
	$clause = "WHERE keyword='".join("' OR keyword='", $keywords)."' ";
	$result_number = db_select('search', array('COUNT(id) as result_number'), $clause);
	$result_number = $result_number[0]['result_number'];
	$clause.= 'ORDER BY points DESC LIMIT 10 OFFSET '.(10*pid());
	$column = array('id', 'topic', 'content_mode', 'content_id', 'keyword', 'keywords_no', 'rendered', 'points');
	$data = db_select('search', $column, $clause);
	$render_array = array(
		'data' => $data,
		'result_number' => $result_number
	);
	$render = render($render_array, 'search.tpl', 'search');
	$pager = new PagerRender(10, $result_number);
	return $render.$pager->render('search/search/'.gg('search'));
}

function search_calculate_point($keywords, $matched){
	$points = array();
	foreach($keywords as $keyword){
		foreach($matched as $id => $entry){
			$n = substr_count($entry['title'], $keyword);
			$number = $n;
			$point  = 50*$n;
			$n = substr_count($entry['keyword'], $keyword);
			$number+= $n;
			$point += 20*$n;
			$n = substr_count($entry['description'], $keyword);
			$number+= $n;
			$point += 10*$n;
			$n = substr_count($entry['content'], $keyword);
			$number+= $n;
			$point +=  5*$n;
			$points[$keyword][$id]['points'] = $point;
			$points[$keyword][$id]['number'] = $number;
		}
	}
	return $points;
}

function search_get_match($keywords, $update_time){
	$column = array('title', 'keyword', 'description', 'content');
	$clause = array();
	foreach($keywords as $keyword){
		if(isset($update_time[$keyword])){
			$update_clause = "AND insert_time >='".$update_time[$keyword]."'";
		}else{
			$update_clause = "";
		}
		$clause[] = '(('.join(" LIKE '%$keyword%' OR ", $column)." LIKE '%$keyword%') ".$update_clause.')';
	}
	$column[] = 'id';
	$column[] = 'mode';
	$column[] = 'mode_id';
	$clause = 'WHERE '.join(' OR ', $clause);
	return db_select('search_source', $column, $clause);
}

function search_get_update_time($keywords){
	$clause = "WHERE keyword='".join("' OR keyword='", $keywords)."' GROUP BY keyword";
	$data = db_select('search', array('keyword', 'update_time'), $clause);
	$update_time = array();
	foreach($data as $key => $value){
		$update_time[$value['keyword']] = $value['update_time'];
	}
	return $update_time;
}

function search_update_index($keywords, $matched, $points){
	foreach($keywords as $keyword){
		foreach($matched as $id => $entry){
			$data_array = array(
				'topic' => $entry['title'],
				'keyword' => $keyword,
				'rendered' => search_render_entry($entry['content']),
				'content_mode' => $entry['mode'],
				'content_id' => $entry['mode_id'],
				'points' => $points[$keyword][$id]['points'],
				'keywords_no' => $points[$keyword][$id]['number'],
				'update_time' => NOW,
				'clicks' => 0,
			);
			$clause = "WHERE keyword='$keyword' AND content_mode='".$entry['mode']."' AND content_id=".$entry['mode_id'];
			$check_exist = db_select('search', array('id', 'clicks'), $clause);
			$check_exist = array_values($check_exist);
			if(count($check_exist)){
				$data_array['points'] += $check_exist[0]['clicks'];
				$data_array['clicks'] += $check_exist[0]['clicks'];
				db_update('search', $data_array, $check_exist[0]['id']);
			}else{
				db_insert('search', $data_array);
			}
		}
	}
}

function search_render_entry($entry){
	return mb_substr($entry, 0, 500);
}
?>
