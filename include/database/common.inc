<?php
function db_select_single_column($table, $column, $clause=null, $has_id=true, $cachable=true){
	if($column != 'id' and $has_id){
		$pre_data = db_select($table, array('id', $column), $clause, $cachable);
	}else{
		$pre_data = db_select($table, array($column), $clause, $cachable);
	}
	$data = array();
	$i = 0;
	foreach($pre_data as $key=>$value){
		if($has_id) $data[$key] = $value[$column];
		else $data[$i] = $value[$column];
		$i++;
	}
	return $data;
}

function db_increase_query($table){
	global $db_query_number;
	global $db_query_all_number;
	$table = DB_TABLE_PREFIX.$table;
	if(isset($db_query_number[$table])) $db_query_number[$table]++;
	else $db_query_number[$table] = 0;
	$db_query_all_number ++;
}

function db_start_count($table=null){
	global $db_query_number;
	global $db_query_all_number;
	global $db_query_count;
	global $db_query_all_count;
	$table = DB_TABLE_PREFIX.$table;
	$concurency = false;
	if($table){
		if(isset($db_query_count[$table])) $concurency = true;
		if(isset($db_query_number[$table])) $db_query_count[$table] = $db_query_number[$table];
		else $db_query_count[$table] = 0;
	}else{
		if($db_query_all_count) $concurency = true;
		$db_query_all_count = $db_query_all_number;
	}
	
	if($concurency and is_develop()){
		notify_add_warning(tt('Concurrency problem for db_start_count : '.$table));
	}
}

function db_is_new_query($table=null){
	global $db_query_number;
	global $db_query_all_number;
	global $db_query_count;
	global $db_query_all_count;
	$table = DB_TABLE_PREFIX.$table;
	if($table){
		$is_new =  $db_query_count[$table] < $db_query_number[$table];
		unset($db_query_count[$table]);
	}else{
		$is_new = $db_query_all_count < $db_query_all_number;
		$db_query_all_count = 0;
	}
	return $is_new;
}

function db_get_query_number(){
	global $db_query_all_number;
	return $db_query_all_number;
}

function db_update($table, $data, $id){
	$id = (int) $id;
	if(empty($id)) error_syslog('Update database error.');
	db_update_many($table, $data, 'id='.$id);
}

/// Not secure, use with care.
function db_update_many($table, $data, $where){
	$table = DB_TABLE_PREFIX.$table;
	$value_set = '';
	foreach($data as $key => $value){
		if(is_string($value)  or $value==null) $value_set .= $key."='".addslashes($value)."', ";
		else $value_set .= $key.'='.$value.', ';
	}
	$value_set = substr($value_set,0, -2);
	$query = 'UPDATE '.$table.' SET '.$value_set.' WHERE '.$where;
	db_modify($table, $query);
}

function db_insert($table, $data){
	$table = DB_TABLE_PREFIX.$table;
	$cols = join(',', array_keys($data));
	$value_set = '';
	foreach($data as $key => $value){
		if(is_string($value) or $value==null) $value_set .= "'".addslashes($value)."', ";
		else $value_set .= $value.', ';
	}
	$value_set = substr($value_set,0, -2);
	$query = 'INSERT INTO '.$table.'('.$cols.') VALUES('.$value_set.')';
	$result = db_modify($table, $query);
	return db_insert_id($result);
}

function db_drop($table, $id){
	$table = DB_TABLE_PREFIX.$table;
	$id = (int) $id;
	if(empty($id)) error_syslog('Error by dropping data from database.');
	$query = 'DELETE FROM '.$table.' WHERE id='.$id;
	db_modify($table, $query);
}

/// Not secure, use with care.
function db_drop_many($table, $where){
	$table = DB_TABLE_PREFIX.$table;
	$query = 'DELETE FROM '.$table.' WHERE '.$where;
	db_modify($table, $query);
}

function db_create_table($table, $columns){
	$table = DB_TABLE_PREFIX.$table;
	$query = 'CREATE TABLE '.$table.'( ';
	$query.= 'id int unsigned  auto_increment, ';
	foreach($columns as $column){
		$query .= $column.', ';
	}
	$query.= 'PRIMARY KEY (id)) CHARACTER SET utf8;';
	db_modify($table, $query);
}
?>
