<?php
/// @file
/// This file provides functions for database handling with MySQL (support >= MySQL-5.0).
/// The functions are extended with caching system and transaction.
/// However, developers are not free to send any query to database due to the security.
/// Queries are generated with query-templates and given array of data.
/// This method does not simplify the development in all cases but is more secure.

/// Internal global variables for mysql-database-tool.
/// These variables should not be used outside this file.

global $db_connect, $db_transaction, $db_query_number;
global $db_query_all_number, $db_query_count;
global $db_query_all_count, $db_column;

require_once('common.inc');

$db_connect = null;
$db_transaction = false;
$db_query_number = array();
$db_query_all_number = 0;
$db_query_count = array();
$db_query_all_count = 0;
$db_column = array();

function db_init(){
	global $db_connect;
	global $db_transaction;
	@$db_connect = mysql_connect(DB_HOST,DB_USER,DB_PASS) or error_db_connect();
	@mysql_db_query(DB_NAME, 'START TRANSACTION;', $db_connect) or error_db_log('START TRANSACTION', mysql_error());
	$db_transaction = true;
}

function db_select($table, $columns, $clause=null, $cachable=true){
	global $db_connect;
	$table = DB_TABLE_PREFIX.$table;
	$query = 'SELECT '.join(', ', $columns).' FROM '.$table.' '.$clause.';';
	if($cachable){
		$cache_file = cache_get_db_file($table, $query);
		if(cache_check_db($cache_file)){
			return cache_get_db_data($cache_file);
		}
	}
	if(empty($db_connect)) db_init();
	@$state = mysql_db_query(DB_NAME, $query, $db_connect) or error_db_log($query, mysql_error());
	$data = array();
	while($row = mysql_fetch_assoc($state)){
		if(count($columns) != count($row)){
			error_syslog('Error by fetching data from database. Column number mismatch.');
		}
		foreach($row as $key=>$value){
			if(is_string($value)) $row[$key] = stripslashes($value);
		}
		if(isset($row['id'])){
			$data[$row['id']] = $row;
		}
		else $data[] = $row;
	}
	if($cachable) cache_save_db($table, $query, $data);
	db_increase_query($table);
	return $data;
}

function db_get_rows_number($table, $clause=null){
	global $db_connect;
	$table = DB_TABLE_PREFIX.$table;
	$query = 'SELECT COUNT(id) as rows_number FROM '.$table.' '.$clause;
	$cache_file = cache_get_db_file($table, $query);
	if(cache_check_db($cache_file)){
		return cache_get_db_data($cache_file);
	}
	if(empty($db_connect)) db_init();
	@$state = mysql_db_query(DB_NAME, $query, $db_connect) or error_db_log($query, mysql_error());
	$row = mysql_fetch_assoc($state);
	cache_save_db($table, $query, $row['rows_number']);
	db_increase_query($table);
	return $row['rows_number'];
}

function db_count_group_by($table, $column, $where=null, $cachable=true){
	global $db_connect;
	$table = DB_TABLE_PREFIX.$table;
	if(strlen($where)) $where = 'WHERE '.$where;
	$query = "SELECT $column, COUNT($column) as number FROM $table $where GROUP BY $column";
	$cache_file = cache_get_db_file($table, $query);
	if($cachable){
		if(cache_check_db($cache_file)){
			return cache_get_db_data($cache_file);
		}
	}
	if(empty($db_connect)) db_init();
	@$state = mysql_db_query(DB_NAME, $query, $db_connect) or error_db_log($query, mysql_error());
	$data = array();
	while($row = mysql_fetch_assoc($state)){
		$row[$column] = stripslashes($row[$column]);
		$data[] = $row;
	}
	if($cachable) cache_save_db($table, $query, $data);
	db_increase_query($table);
	return $data;
}


function db_insert_id($result){
	return mysql_insert_id();
}

function db_exist_table($table){
	global $db_connect;
	if(empty($db_connect)) db_init();
	$table = DB_TABLE_PREFIX.$table;
	$tables = mysql_list_tables(DB_NAME, $db_connect);
	while($row = mysql_fetch_row($tables)){
		if($row[0] == $table) return true;;
	}
	return false;
}

function db_get_tables(){
	global $db_connect;
	if(empty($db_connect)) db_init();
	$tables = array();
	$state = mysql_list_tables(DB_NAME, $db_connect);
	while($row = mysql_fetch_row($state)){
		$tables[] = $row[0];
	}
	return $tables;
}

function db_exist_column($table, $column){
	global $db_connect, $db_column;
	if(empty($db_connect)) db_init();
	if(isset($db_column[$table])){
		$columns = $db_column[$table];
	}else{
		$columns = array();
		$table = DB_TABLE_PREFIX.$table;
		$state = mysql_list_fields(DB_NAME, $table, $db_connect);
		$number = mysql_num_fields($state);
		for($i = 0; $i < $number; $i++){
			$columns[] = mysql_field_name($state, $i);
		}
		$db_column[$table] = $columns;
	}
	return in_array($column, $columns);
}

function db_modify($table, $query){
	global $db_connect;
	if(empty($db_connect)) db_init();
	@$state = mysql_db_query(DB_NAME, $query, $db_connect) or error_db_log($query, mysql_error());
	cache_clear_db($table);
	db_increase_query($table);
	return $state;
}

function db_get_type($type){
	$typeMap = array(
		'int' => 'INT',
		'real' => 'REAL',
		'char' => 'VARCHAR(250)',
		'shorttext' => 'TEXT',
		'text' => 'MEDIUMTEXT',
		'date' => 'DATE',
		'timestamp' => 'TIMESTAMP',
		'array' => 'TEXT',
	);
	return $typeMap[$type];
}

function db_close(){
	global $db_connect;
	global $db_transaction;
	if($db_connect and $db_transaction){
		mysql_db_query(DB_NAME, 'COMMIT;', $db_connect);
	}
	if($db_connect){
		mysql_close($db_connect);
	}
}

function db_rollback(){
	global $db_connect;
	global $db_transaction;
	if($db_transaction){
		mysql_db_query(DB_NAME, 'ROLLBACK;', $db_connect);
	}
	if($db_connect){
		mysql_close($db_connect);
	}
}


function db_query($query){
	global $db_connect;
	return mysql_db_query(DB_NAME, $query, $db_connect);
}
?>
