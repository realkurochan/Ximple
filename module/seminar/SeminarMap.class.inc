<?php
include_once('seminar.tool.inc');
class SeminarMap extends Control {
	function SeminarMap(){
		seminar_map_create_table();
		$insert_data = array(
		'seminar' => int,
		'attendant' => int,
		);
		db_insert($seminarMapTable, $insert_data);
	}
}
?>
