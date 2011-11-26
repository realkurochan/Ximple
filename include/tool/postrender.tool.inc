<?php
function postrender_target_link($render){
	$target = cc('link_target');
	if(!is_array($target)) $target = array();
	$blank = 'target="_blank" ';
	if(in_array('#external', $target)){
		$render = str_replace('href="'.SECTION_URI, '#internel#', $render);
		$render = str_replace('href="http://', $blank.'href="http://', $render);
		$render = str_replace('#internel#', 'href="'.SECTION_URI, $render);
	}
	foreach($target as $key => $value){
		if($value != '#external' and $value!= '#notify'){
			$link = 'href="'.SECTION_URI.Q.$value;
			$render = str_replace($link, $blank.$link, $render);
		}
	}
	if(MODE_ID == 0){
		$page_id = 'href="'.SECTION_URI.Q.MODE;
	}else{
		$page_id = 'href="'.SECTION_URI.Q.MODE.'/'.MODE_ID;
	}
	$render = str_replace($blank.$page_id, $page_id, $render);
	return $render;
}

function postrender_correct_uri($render){
	$render = str_replace('href="?', 'href="'.SECTION_URI.'?', $render);
	$render = str_replace('src="files','src="'.ROOT_URI.'files', $render);
	$render = str_replace('src="Image','src="'.ROOT_URI.'Image', $render);
	if(cc('friendly_url_enable')){
		$render = str_replace('href="'.SECTION_URI.'?/' ,'href="'.SECTION_URI ,$render);
	}
	return $render;
}

function postrender_get_site_info($render){
	if(cc('interp_info')){
		$qn = db_get_query_number();
		$memory = sprintf('%.2f', memory_get_usage()/1e6);
		$time = sprintf('%.2f', ((float)microtime(true) - BOOT_TIME)*1e3);
		$info = '[visitor : '.count_get('site', 0).' | time : '.$time.' ms | memory : '.$memory.' MB | query : '.$qn.']';
		$info = '<p id="site_info"><!--si-->'.$info.'<!--is--><p>';
		$render = str_replace('<!--{site_info}-->', $info, $render);
	}
	return $render;
}

function postrender_get_read_number($render){
	preg_match_all("/{ximple_read\/[\w]*\/[\d]*}/", $render, $reads);
	$mode_map = array();
	foreach($reads[0] as $read){
		list($mode, $id) = split("/", substr($read, 13, -1));
		$mode_map[$mode][] = $id;
	}
	
	if(count($mode_map)){
		foreach($mode_map as $mode => $id_array){
			$read_number = count_get_array($mode, $id_array);
			foreach($read_number as $id => $number){
				$render = str_replace("<!--{ximple_read/$mode/$id}-->",$number, $render);
			}
		}
	}
	return $render;
}
?>
