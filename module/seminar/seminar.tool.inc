<?php
include_once('SeminarMapModel.class.inc'); //create 'seminar_map' table.

function seminar_map_create_table(){
	$modelFactory = new ModelFactory('seminar_map', new SeminarMapModel());
	$modelConnector = $modelFactory->bind();
	$tableCreator = new TableCreator($modelConnector, 'seminar_map');
	$tableCreator->create();
}
?>
