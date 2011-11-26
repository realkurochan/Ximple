<?php
execute_add_function('flashwidget');
execute_add_function('flashwidget_config');
execute_add_function('flashwidget_save');

function flashwidget(){
	$column = array('id', 'mode', 'table_name', 'column_name', 'clause', 'item_number');
	$data = db_select('flashwidget', $column, 'WHERE id='.((int)gg('id')));
	$data = $data[gg('id')];
	$clause = $data['clause'].' LIMIT '.$data['item_number'];
	$item = db_select_single_column($data['table_name'], $data['column_name'], $clause);
	if(count($item)){
		$item = flashwidget_translate($data['mode'], $data['column_name'], $item);
		$render_array = array(
			'mode' => $data['mode'],
			'item' => $item,
		);
		return render($render_array, 'module/flashwidget/theme/flashwidget.tpl', 'flashwidget');
	}else{
		return null;
	}
}

function flashwidget_clear_cache($table){
	
}

function flashwidget_add($reference, $mode, $name, $table, $column, $clause){
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	$check_clause = 'WHERE section='.SECTION." AND reference='$reference'";
	$existence_data = db_select_single_column('flashwidget', 'id', $check_clause, false);
	if(count($existence_data) == 0){
		$data = array(
			'reference' => $reference,
			'mode' => $mode,
			'table_name' => $table,
			'column_name' => $column,
			'clause' => $clause,
			'section' => SECTION,
			'item_number' => 5,
		);
		$id = db_insert('flashwidget', $data);
	}else{
		$id = $existence_data[0];
	}
	widget_add($name, 'flashwidget/'.$id, 'disabled', 'flashwidget_config/'.$id, 'flashwidget_save/'.$id);
}

function flashwidget_drop($reference){
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	$clause = "reference='".$reference."' AND section=".SECTION;
	$id = db_select_single_column('flashwidget', 'id', 'WHERE '.$clause, false);
	if(count($id)){
		db_drop_many('flashwidget', $clause);
		widget_drop('flashwidget/'.$id[0]);
	}
}

function flashwidget_config(){
	if(gg('is_translate') == 'false'){
		include_once(INCLUDE_PATH.'class/Model.class.inc');
		$item_number_data = db_select_single_column('flashwidget', 'item_number', 'WHERE id='.gg('id'), false);
		$item_number = Model::spin(tt('number of shown items'), 3, 50);
		$item_number->data = $item_number_data[0];
		return array('flashwidget_item_number' => $item_number);
	}
}

function flashwidget_save(){
	db_update('flashwidget', array('item_number' => pp('flashwidget_item_number')),gg('id'));
}

function flashwidget_translate($mode, $column_name, $data){
	$splitted = explode('_', $mode);
	foreach($data as $key => $value){
		$content_array = array(
			'id' => $key,
			$column_name => $value
		);
		$translated = translate_content($content_array, $splitted[0], $key);
		$data[$key] = $translated[$column_name];
	}
	return $data;
}

?>
