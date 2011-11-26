<?php
require_once('develcard.tool.inc');

execute_add_function('develcard_widget_resolver');
execute_add_function('develcard_widget_resolver_config');
execute_add_function('develcard_widget_resolver_save');
execute_add_function('develcard_widget_personal');
execute_add_function('develcard_widget_personal_config');
execute_add_function('develcard_widget_personal_save');

function develcard_widget_resolver(){
	if(authority_group('develcard_resolver')){
		return develcard_widget_render(USER_ID);
	}else{
		return null;
	}
}

function develcard_widget_resolver_config(){
	include_once(INCLUDE_PATH.'class/Model.class.inc');
	$item_number = Model::spin(tt('number of shown items'), 3, 50);
	$item_number->data = cc('develcard_widget_resolver_number');
	return array('develcard_widget_resolver_number' => $item_number);
}

function develcard_widget_resolver_save(){
	config_set('develcard_widget_resolver_number', pp('develcard_widget_resolver_number'));
	config_save();
}

function develcard_widget_personal(){
	$is_resolver = authority_group('develcard_resolver');
	$is_moderator = authority_group('develcard_moderator');
	$is_mode = ((MODE == 'develcard_personal') or (MODE == 'develcard_resolver_list'));
	if(($is_resolver or $is_moderator) and $is_mode){
		if(MODE_ID == 0) $uid = USER_ID;
		else $uid = MODE_ID;
		return develcard_widget_render($uid);
	}else{
		return null;
	}
}

function develcard_widget_personal_config(){
	include_once(INCLUDE_PATH.'class/Model.class.inc');
	$item_number = Model::spin(tt('number of shown items'), 3, 50);
	$item_number->data = cc('develcard_widget_personal_number');
	return array('develcard_widget_personal_number' => $item_number);
}

function develcard_widget_personal_save(){
	config_set('develcard_widget_personal_number', pp('develcard_widget_personal_number'));
	config_save();
}

function develcard_widget_render($uid){
	$resolver = develcard_widget_get_resolver_data($uid);
	if(count($resolver)){
		$work_load = develcard_widget_get_resolver_work_load($resolver);
		$render_array = array(
			'data' => develcard_widget_get_data($work_load),
		);
		return render($render_array, 'develcard_widget.tpl', 'develcard_widget');
	}
}

function develcard_widget_get_resolver_data($uid){
	$clause = 'WHERE status=1 AND resolver='.$uid;
	$column = array('work_load', 'develcard');
	return db_select('develcard_resolver', $column, $clause);
}

function develcard_widget_get_resolver_work_load($resolver){
	$work_load = array();
	foreach($resolver as $key => $value){
		$work_load[$value['develcard']] = $value['work_load'];
	}
	return $work_load;
}

function develcard_widget_get_data($work_load){
	$id_clause = 'id='.join(' OR id=', array_keys($work_load));
	$clause = "WHERE $id_clause AND section=".SECTION.' ORDER BY deadline';
	$column = array('id', 'topic', 'deadline', 'priority');
	$develcard = db_select('develcard', $column, $clause);
	$priority = develcard_get_priority_option();
	foreach($develcard as $id => $value){
		$develcard[$id]['work_load'] = $work_load[$id];
		$develcard[$id]['deadline'] = date(cc('time_format'), strtotime($value['deadline']));
		$develcard[$id]['priority'] = $priority[$value['priority']];
	}
	return $develcard;
}
?>
