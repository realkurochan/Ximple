<?php
/// @file
/// This file provides functions for counting and auto increment.
/// In XimpleCMS many modules requires counting function.
/// Normally, the counting function is implemented with database,
/// where performance can be influenced.
/// Counting functions implemented here use binary file to store counting value.

/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Increase counting by given mode and ID.
/// @param $mode : mode to increase counting.
/// @param $id : ID of page to increase counting. For multiple pages, $id can be array of ID.
/// @param $offset : offset value of counting.
function count_increase($mode, $id, $offset=0){
	$count_dir = FILES_PATH.'count/';
	if(!is_dir($count_dir)) mkdir($count_dir);
	$count_dir.= SECTION.'/';
	if(!is_dir($count_dir)) mkdir($count_dir);
	$file_name = $count_dir.$mode.'.bin';
	$file = null;
	if(!is_array($id)) $file = count_increase_scalar($file_name, $file, $id, $offset);
	else $file = count_increase_array($file_name, $file, $id, $offset);
	if($file) fclose($file);
}

/// Set counting to the given value.
/// @param $mode : mode to increase counting.
/// @param $id : ID of page to increase counting.
/// @param $value : given value so set.
function count_set($mode, $id, $value){
	$count_dir = FILES_PATH.'count/';
	if(!is_dir($count_dir)) mkdir($count_dir);
	$count_dir.= SECTION.'/';
	if(!is_dir($count_dir)) mkdir($count_dir);
	$file_name = $count_dir.$mode.'.bin';
	if(is_file($file_name)) $file = fopen($file_name, 'r+');
	else $file = fopen($file_name, 'x');
	$bin = pack('V', $value);
	fseek($file, $id*4);
	flock($file, LOCK_EX);
	fwrite($file, $bin);
	flock($file, LOCK_UN);
	if($file) fclose($file);
}

/// Low level implementation of counting function for scalar mode.
/// @param $file_name : name of file, which store counting values.
/// @param $file : file pointer of storage file.
/// @param $id : ID of page to increase counting (not allowed to be array).
/// @param $offset : offset value of counting.
function count_increase_scalar($file_name, $file, $id, $offset){
	if(is_file($file_name)){
		$size = filesize($file_name);
		if($file == null) $file = fopen($file_name, 'r+');
		fseek($file, $id*4, SEEK_SET);
		if($size >= $id*4 + 4){
			$bin = fread($file, 4);
			if(strlen($bin)) $number = unpack('V', $bin);
			else $number = array(0,0);
			$bin = pack('V', $number[1]+1+$offset);
		}else{
			$bin = pack('V', 1+$offset);
		}
	}else{
		if($file == null) $file = fopen($file_name, 'x');
		$bin = pack('V', 1+$offset);
	}
	fseek($file, $id*4);
	flock($file, LOCK_EX);
	fwrite($file, $bin);
	flock($file, LOCK_UN);
	return $file;
}

/// Low level implementation of counting function for array mode.
/// @param $file_name : name of file, which store counting values.
/// @param $file : file pointer of storage file.
/// @param $id_array : array of ID.
/// @param $offset : offset value of counting.
function count_increase_array($file_name, $file, $id_array, $offset){
	foreach($id_array as $id){
		$file = count_increase_scalar($file_name, $file, $id, $offset);
	}
	return $file;
}


/// Get counting value of given mode and ID.
/// @param $mode : mode to increase counting.
/// @param $id : ID of page to increase counting (scalar only).
/// @return Counting value of given mode and ID.
function count_get($mode, $id){
	$file_name = FILES_PATH.'count/'.SECTION.'/'.$mode.'.bin';
	if(!is_file($file_name)) return 0;
	$size = filesize($file_name);
	if($size < $id*4 + 4) return 0;
	$file = fopen($file_name, 'r');
	fseek($file, $id*4);
	$count = unpack('V', fread($file, 4));
	return $count[1];
}

/// Get all members of array of counting values of given mode.
/// @param $mode : mode to increase counting.
/// @return Array of counting value of given mode and ID.
function count_get_all($mode){
	$file_name = FILES_PATH.'count/'.SECTION.'/'.$mode.'.bin';
	if(!is_file($file_name)) return array();
	$file = fopen($file_name, 'r');
	$size = filesize($file_name)/4;
	$result = array();
	for($i=0;$i<$size;$i++){
		fseek($file, $i*4);
		$count = unpack('V', fread($file, 4));
		$result[] = $count[1];
	}
	return $result;
}

/// Get array of counting values of given mode and given IDs.
/// @param $mode : mode to increase counting.
/// @param $id_array : array of ID.
/// @return Array of counting value of given mode and ID.
function count_get_array($mode, $id_array){
	$file_name = FILES_PATH.'count/'.SECTION.'/'.$mode.'.bin';
	if(!is_file($file_name)){
		$count = array();
		foreach($id_array as $id){
			$count[$id] = 0;
		}
		return $count;
	}
	$size = filesize($file_name);
	$file = fopen($file_name, 'r');
	$count = array();
	foreach($id_array as $id){
		if($size < $id*4 + 4){
			$count[$id] = 0;
		}else{
			fseek($file, $id*4);
			$number = unpack('V', fread($file, 4));
			$count[$id] = $number[1];
		}
	}
	return $count;
}
?>
