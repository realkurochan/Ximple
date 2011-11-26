<?php
/// @file
/// This file provides functions for input and request data manipulation.
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Check email conformation of given input data
/// @param $email : given input to check conformation.
/// @return TRUE, if given in put is conformed for email.
function input_is_email($email){
	return preg_match('/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/', $email);
}

/// Trim white space from given input.
/// @param $input : given input
/// @return Input without white space.
function input_trim($input){
	$input = preg_replace('/\s+/', ' ', $input);
	return $input;
}

/// Split givrn string to array of string using comma to separate string.
/// @param $input : given input
/// @return Array of splitted string.
function input_split_comma($input){
	if(is_array($input)){
		return$input;
	}elseif(strlen($input)){
		$input = split(',', str_replace(' ', '', input_trim($input)));
		foreach($input as $key => $value){
			if(strlen($value) == 0) unset($input[$key]);
		}
		return $input;
	}
	return array();
}

/// Strip image tag from given input.
/// @param $input : given input
/// @return String without image tag.
function input_strip_image($input){
	return  preg_replace("/<img[^>]+\>/i", "", $input);
}

/// Reorder array of submitted files information to more usable form.
/// @param $name : name of file input
/// @return Reordered array of file information.
function input_reorder_multiple_files($name){
	$reordered = array();
	$received = ff($name);
	$n = count($received['name']);
	if(is_array($received['name'])){
		foreach($received['name'] as $i => $value){
			$reordered[$i]['name']     = $received['name'][$i];
			$reordered[$i]['tmp_name'] = $received['tmp_name'][$i];
			$reordered[$i]['type']     = $received['type'][$i];
			$reordered[$i]['size']     = $received['size'][$i];
			$reordered[$i]['error']    = $received['error'][$i];
		}
	}
	return $reordered;
}

/// Process special characters of requested URI.
/// @param $uri_get : original $_GET input
/// @param $uri_server : original requested URI
/// @return Requested URI with processed special characters.
function input_special_char($uri_get, $uri_server){
	foreach($uri_server as $key => $value){
		if(strpos($value, '%') !== false){
			$split1 = str_split($uri_get[$key]);
			$split2 = split('%', $value);
			$res = '';
			foreach($split1 as $ind => $chr){
				if($chr != '.' and $chr != ' ' and $chr != '_') $res .= $split1[$ind];
				elseif(isset($split2[$ind+1])) $res .= chr(hexdec($split2[$ind+1]));
			}
			$uri_server[$key] = $res;
		}
	}
	return $uri_server;
}

/// Strip slash from array of string (recursive)
/// @param $array : array of string
/// @return Array of string without slash.
function input_strip_slash_array($array){
	foreach($array as $key => $value){
		if(is_array($value)) $array[$key] = input_strip_slash_array($value);
		else $array[$key] = stripslashes($value);
	}
	return $array;
}

/// Strip unwanted HTML tag from input string to prevent XSS.
/// @param $data : data to strip HTML tag
/// @param $allowed_tag : allowed HTML tag
/// @return String without unwanted HTML tag.
function input_strip_tags($data, $allowed_tag){
	$data = str_replace('<!--', '&lt;!--', $data);
	$data = str_replace('--!>', '--!&gt;', $data);
	$data = strip_tags($data, $allowed_tag);
	$data = str_replace('&lt;!--', '<!--', $data);
	$data = str_replace('--!&gt;', '--!>', $data);
	return $data;
}


/// Get month and year from $_GET. Set to current month and year, if $_GET is not set.
/// @return Month and year.
function input_get_month(){
	$month = (int) gg('month');
	$year = (int) gg('year');
	
	if(empty($month)) $month = (int) date('n');
	if(empty($year)) $year = (int) date('Y');
	
	return array($month, $year);
}

/// Dump variable into parsable string.
/// @param $input : input to dump.
/// @return String of dumped variable. 
function input_dump_var($input){
	ob_start();
	var_export($input);
	$dump = ob_get_contents();
	ob_end_clean();
	return str_replace('  ', "\t", $dump);
}
?>
