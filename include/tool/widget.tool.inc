<?php
function widget_render(){
	$data = widget_get_data();
	widget_init_default();
	$render = array();
	foreach($data as $key => $value){
		$value = translate_content($value, 'widget', $value['id']);
		$pos = $value['position'];
		$item = widget_render_item($value, 'widget_'.$pos.'.tpl');
		if($item) $render[$pos][] = $item;
	}
	$position = cc('widget_position');
	foreach($position as $key => $value){
		if(!isset($render[$key])) $render[$key] = array();
	}
	return $render;
}

function widget_init_default($checked=false){
	$data = db_select_single_column('widget', 'id', 'WHERE section='.SECTION);
	if(count($data) == 0 or $checked){
		include_once(MODULE_PATH.'module/module.tool.inc');
		$position = cc('widget_primary_position');
		widget_add(tt('Navigation'), 'syswidget_navigation', $position, 'syswidget_navigation_config', 'syswidget_navigation_save', 0);
		widget_add(tt('Control Panel'), 'syswidget_panel', $position, '', '', 999);
		widget_add(tt('Login'), 'syswidget_login', $position, '', '', 1000);
		widget_add(tt('Language Selection'), 'syswidget_translate', 'foot');
		widget_add(tt('Execution Information'), 'syswidget_execute_info', 'foot');
		return widget_get_data();
	}
}

function widget_get_data(){
	$column = array('id', 'title', 'body', 'position', 'call_mode', 'appear_group', 'appear_module', 'appear_uri');
	$clause = 'WHERE section='.SECTION.' AND position != \'disabled\' ORDER BY sort_number';
	return db_select('widget', $column, $clause);
}

function widget_render_item($item, $tpl){
	if(widget_check_permision($item)){
		$body = widget_get_body($item);
		if(!empty($body)){
			$render_data = array(
				'title' => $item['title'],
				'body' => $body,
			);
			return render($render_data, $tpl, 'widget_item');
		}
	}
	return null;
}

function widget_get_body($item){
	if(strlen($item['call_mode'])){
		$call_mode = $item['call_mode'].'/'.$item['id'];
		$body = execute_string($call_mode);
		if(strlen($body)) $body = $item['body'].$body;
	}else{
		$body = $item['body'];
	}
	return $body;
}

function widget_check_permision($item){
	$appear_module = unserialize($item['appear_module']);
	$appear_group = unserialize($item['appear_group']);
	$appear_uri = isset($item['appear_uri']) ? $item['appear_uri'] : null;
	if(strlen($appear_uri)){
		if(widget_check_uri_permission($appear_uri)){
			return widget_check_group_permission($appear_group);
		}else{
			return false;
		}
	}elseif(widget_check_module_permission($appear_module)){
		return widget_check_group_permission($appear_group);
	}else{
		return false;
	}
}

function widget_check_uri_permission($appear_uri){
	$appear_mode = MODE.'/'.MODE_ID;
	foreach(input_split_comma($appear_uri) as $key => $value){
		if($value ==  $appear_mode or $value == MODE){
			return true;
		}
	}
	return false;
}

function widget_check_module_permission($appear_module){
	return (count($appear_module) == 0 or in_array(MODULE, $appear_module));
}

function widget_check_group_permission($appear_group){
	if(in_array(0, $appear_group)){
		return true;
	}
	if(count(array_intersect(ss('login_group'), $appear_group))){
		return true;
	}
}

function widget_add_navigation($mode, $title){
	config_append_assc('navigation', $mode, $title);
	config_append('navigation_enabled', $mode);
}

function widget_drop_navigation($mode){
	config_drop_assc('navigation', $mode);
	config_drop_by_value('navigation_enabled', $mode);
}

function widget_add($title, $call_mode, $position='disabled', $form_mode='', $save_mode='', $sort_number=1){
	$clause = 'WHERE section='.SECTION." AND call_mode='$call_mode'";
	$existence_data = db_select_single_column('widget', 'id', $clause);
	if(count($existence_data) == 0){
		$insert_data = array(
			'title' => (string) $title,
			'config_form_mode' => (string) $form_mode,
			'config_save_mode' => (string) $save_mode,
			'call_mode' => (string) $call_mode,
			'position' => $position,
			'appear_module' => (string) serialize(array()),
			'appear_group' => (string) serialize(array(0)),
			'body' => '',
			'sort_number' => $sort_number,
			'section' => SECTION,
		);
		db_insert('widget', $insert_data);
	}
}

function widget_drop($call_mode){
	db_drop_many('widget', "call_mode='".addslashes((string)$call_mode)."'");
}
?>
