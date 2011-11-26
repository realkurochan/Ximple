<?php
execute_add_function('sectioninfo_check_name_exists');
execute_add_function('sectioninfo_check_path_exists');

function sectioninfo_check_name_exists(){
	$name = addslashes((string) gg('name'));
	$sid = (int) gg('sid');
	if(!empty($sid)) $clause = "WHERE (name='".$name."' AND id!=".$sid.")";
	else $clause = "WHERE (name='".$name."' )";
	$user_name = db_select_single_column('section', 'name', $clause);
	execute_set_ajax();
	if(count($user_name)){
		return 'exist';
	}else{
		return 'not exist';
	}
}

function sectioninfo_check_path_exists(){
	$path = addslashes((string) gg('path'));
	$sid = (int) gg('sid');
	if(!empty($sid)) $clause = "WHERE (path='".$path."' AND id!=".$sid.")";
	else $clause = "WHERE (path='".$path."' )";
	$user_name = db_select_single_column('section', 'name', $clause);
	execute_set_ajax();
	if(count($user_name)){
		return 'exist';
	}else{
		return 'not exist';
	}
}

?>
