<?php
execute_add_function('syslog_render');

function syslog_render(){
	$title = tt('System Log');
	document_set_title($title);
	document_set_path($title);
	head_add_css('syslog.css');
	head_add_css('list.css');
	$log = syslog_read_log();
	$type = syslog_get_type($log);
	$log = syslog_filter_type($log);
	$render_array = array(
		'day' => syslog_get_previous_day(),
		'log' => $log,
		'type_number' => $type,
		'selected' => mid(),
		'type' => gg('type'),
	);
	
	return render($render_array, 'syslog.tpl', 'syslog');
}

function syslog_read_log(){
	$raw_log = io_read_file(io_get_log_file(mid()));
	$line_log = split("\n", $raw_log);
	$log = array();
	foreach($line_log as $key => $value){
		if(strlen($value)){
			$row = split(',', $value);
			if(count($row) == 6){
				$row[5] = substr($row[5], 0, 120);
				$row[3] = substr($row[3], 0, 60);
				$log[] = $row;
			}
		}
	}
	return array_reverse($log);
}

function syslog_get_previous_day(){
	$day = array();
	$interval = ceil(cc('syslog_time')/86400);
	for($i=1 ; $i<=$interval; $i++){
		if(is_file(io_get_log_file($i))){
			$day[] = $i;
		}
	}
	return $day;
}

function syslog_get_type($log){
	$type = array();
	foreach($log as $key => $value){
		if(isset($type[$value[4]])) $type[$value[4]]++;
		else $type[$value[4]] = 1;
	}
	return $type;
}

function syslog_filter_type($log){
	$type = gg('type');
	if($type){
		foreach($log as $key => $value){
			if($value[4] != $type) unset($log[$key]);
		}
	}
	return $log;
}
?>
