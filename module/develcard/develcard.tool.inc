<?php
function develcard_get_status_option(){
	return array(
		1 => tt('in progress'),
		2 => tt('closed'),
		3 => tt('failed'),
		4 => tt('rejected'),
		0 => tt('created'),
	);
}

function develcard_get_status_name($status){
	$status_option = develcard_get_status_option();
	if(isset($status_option[$status])) return $status_option[$status];
}

function develcard_get_default_type_option(){
	return array(
		0 => tt('bug'),
		1 => tt('enhancement'),
		2 => tt('suggestions'),
		3 => tt('document'),
	);
}

function develcard_get_type_name($type){
	$type_option = cc('develcard_type');
	if(isset($type_option[$type])) return $type_option[$type];
}

function develcard_get_priority_option(){
	return array(
		0 => tt('0 - High'),
		1 => tt('1 - Medium'),
		2 => tt('2 - Low'),
		3 => tt('3 - No-Effect'),
	);
}

function develcard_get_permission_option(){
	return array(
		0 => tt('everyone can submit report'),
		1 => tt('all users can submit report'),
		2 => tt('only users with DevelCard Reporter role can submit report'),
	);
}

function develcard_get_priority_color($priority){
	$color = array(
		0 => cc('develcard_hight_color'),
		1 => cc('develcard_medium_color'),
		2 => cc('develcard_low_color'),
		3 => cc('develcard_no_effect_color'),
	);
	return $color[$priority];
}

function develcard_get_status_color($status){
	$color = array(
		0 => cc('develcard_created_color'),
		1 => cc('develcard_progress_color'),
		2 => cc('develcard_close_color'),
		3 => cc('develcard_fail_color'),
		4 => cc('develcard_rejected_color'),
	);
	return $color[$status];
}

function develcard_is_admin(){
	$is_admin = authority_group('develcard_moderator');
	$is_admin = ($is_admin or authority_group('develcard_admin'));
	return $is_admin;
}
?>
