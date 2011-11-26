<?php
if(PHP_SAPI !== 'cli') die('Databsae test is only executable by CLI mode.');
require_once('../setup.php');
require_once(DB_TYPE.'.inc');

function db_test(){
	echo('Initialize database connection.');
	db_init();
	
}
?>
