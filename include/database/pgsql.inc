<?php
/// @file
/// This file provides functions for database handling with PostgreSQL (support >= PostgreSQL-8.2).
/// The functions are extended with caching system and transaction.
/// However, developers are not free to send any query to database due to the security.
/// Queries are generated with query-templates and given array of data.
/// This method does not simplify the development in all cases but is more secure.

/// Internal global variables for postgresql-database-tool.
/// These variables should not be used outside this file.

/// Create a database on Ubuntu
/// $sudo su postgres
/// $createuser username --password
/// $createdb databasename
/// $psql databasename
/// databasename=# ALTER USER username WITH PASSWORD 'password';

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
	$conn_string = 'host='.DB_HOST.' dbname='.DB_NAME.' user='.DB_USER.' password='.DB_PASS;
	@$db_connect = pg_connect($conn_string) or error_db_connect();
	@pg_query($db_connect, 'START TRANSACTION') or error_db_log($query, pg_last_error());
	$db_transaction = true;
}

function db_select($table, $columns, $clause=null, $cachable=true){ //O.D. edited
	global $db_connect;
	$table = DB_TABLE_PREFIX.$table;
	$query = 'SELECT '.join(', ', $columns).' FROM '.$table.' '.$clause;
	if($cachable){
		$cache_file = cache_get_db_file($table, $query);
		if(cache_check_db($cache_file)){
			return cache_get_db_data($cache_file);
		}
	}
	@$state = pg_query($db_connect, $query) or error_db_log($query, pg_last_error());
	$data = array();
	while($row = pg_fetch_assoc($state)){
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
	@$state = pg_query($db_connect, $query) or error_db_log($query, pg_last_error());
	$row = pg_fetch_assoc($state);
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
	@$state = pg_query($db_connect, $query) or error_db_log($query, pg_last_error());
	$data = array();
	while($row = pg_fetch_assoc($state)){
		$row[$column] = stripslashes($row[$column]);
		$data[] = $row;
	}
	if($cachable) cache_save_db($table, $query, $data);
	db_increase_query($table);
	return $data;
}

function db_insert_id($result){
	return pg_last_oid($result);
}

function db_exist_table($table){
	global $db_connect;
	$table = DB_TABLE_PREFIX.$table;
	$tables = db_select('relname', array('pg_class'), "WHERE relname = '$table';");
	if(count($tables))  return true;
	else return false;
}

function db_get_tables(){
	global $db_connect;
	$tables = db_select('relname', array('pg_class'), "WHERE relname = '$table';");
	return array_values($tables);
}

function db_exist_column($table, $column){
	global $db_connect, $db_column;
	$table = DB_TABLE_PREFIX.$table;
	$columns = db_array("SELECT * FROM $table LIMIT 1");
	$columns = array_keys($db_cols);
	return in_array($column, $columns);
}

function db_modify($table, $query){
	global $db_connect;
	@$state = pg_query($db_connect, $query) or error_db_log($query, pg_last_error());
	cache_clear_db($table);
	db_increase_query($table);
	return $state;
}

function db_get_type($type){
	$typeMap = array(
		'int' => 'INT',
		'real' => 'DECIMAL',
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
		pg_query($db_connect, 'COMMIT;');// semi collon?
	}
}

function db_rollback(){
	global $db_connect;
	global $db_transaction;
	if($db_transaction){
		pg_query($db_connect, 'ROLLBACK;'); // semi collon?
	}
}

?>
