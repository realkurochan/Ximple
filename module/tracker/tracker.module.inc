<?php
execute_add_function('tracker');

function tracker(){
	if(mid()) $uid = mid();
	else $uid = USER_ID;
	$title = tracker_get_title();
	document_set_title($title);
	document_set_path($title);
	head_add_css('list.css');
	head_add_css('tracker.css');
	$interval = ceil(cc('tracker_time')/86400);
	$tracker = tracker_read($interval, $uid);
	$action = tracker_get_action($tracker);
	$tracker = tracker_filter_action($tracker);
	$render_array = array(
		'uid' => $uid,
		'title' => $title,
		'day' => tracker_get_previous_day($interval, $uid),
		'tracker' => $tracker,
		'selected' => (int) gg('day'),
		'action_number' => $action,
		'action' => gg('action'),
	);
	return render($render_array, 'tracker.tpl', 'tracker');
}

function tracker_get_title(){
	if(mid()){
		$user = db_select_single_column('users', 'login_name', 'WHERE id='.mid(), false);
		$user = $user[0];
	}else{
		$user = USER_NAME;
	}
	return sprintf(tt('Tracker of %s'), $user);
}

function tracker_read($interval, $uid){
	$file_name = io_get_loop_file('tracker/'.$uid.'/', $interval, (int)gg('day'));
	$raw_tracker = io_read_file($file_name);
	$line_tracker = split("\n", $raw_tracker);
	$tracker = array();
	$action = gg('action');
	foreach($line_tracker as $key => $value){
		if(strlen($value)){
			$tracker[] = split(',', $value);
		}
	}
	return array_reverse($tracker);
}

function tracker_get_action($tracker){
	$action = array();
	foreach($tracker as $key => $value){
		if(isset($action[$value[5]])) $action[$value[5]]++;
		else $action[$value[5]] = 1;
	}
	return $action;
}

function tracker_get_previous_day($interval, $uid){
	$day = array();
	for($i=1 ; $i<=$interval; $i++){
		$file_name = io_get_loop_file('tracker/'.$uid.'/', $interval, $i);
		if(is_file($file_name)){
			$day[] = $i;
		}
	}
	return $day;
}

function tracker_filter_action($tracker){
	$action = gg('action');
	if($action){
		foreach($tracker as $key => $value){
			if($tracker[$key][5] != $action) unset($tracker[$key]);
		}
	}
	return $tracker;
}

?>
