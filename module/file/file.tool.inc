<?php

if(authority_is_guest()) error_denied();

/// Get the file list from the $_SESSION.
/// The file list to cut and copy is saved in the $_SESSION.
/// By pasting, the system read the file list and pate to the current directory.
/// @param $key : sess_key of the $_SESSION, which saves file list (cutfile, copyfile).
function file_set_session_file_list($sess_key){
	$file = gg('file');
	if(!isset($_SESSION[$sess_key])) $_SESSION[$sess_key] = array();
	if($file){
		if(!in_array($file, $_SESSION[$sess_key])) $_SESSION[$sess_key][] = $file;
	}else{
		$get = gg();
		foreach($get as $key=>$value){
			if(substr($key, 0, 5) == 'file_' and !in_array($value, $_SESSION[$sess_key])){
				$_SESSION[$sess_key][] = $value;
			}
		}
	}
}

/// Render file  list for file copying.
/// @return Rendered file list.
function file_copy_list(){
	$file_copy_list = '<ul>';
	$copy_file = ss('copyfile');
	if(is_array($copy_file)){
		foreach($copy_file as $file){
			$file_copy_list.= '<li>'.$file."</li>";
		}
	}
	$file_copy_list.= '</ul>';
	return $file_copy_list;
}

/// Render file  list for file cutting.
/// @return Rendered file list.
function file_cut_list(){
	$file_cut_list = '<ul>';
	$cut_file = ss('cutfile');
	if(is_array($cut_file)){
		foreach($cut_file as $file){
			$file_cut_list.= '<li>'.$file."</li>";
		}
	}
	$file_cut_list.= '</ul>';
	return $file_cut_list;
}

/// Paste copied files into current directory.
/// @return Rendered list of file name.
function file_paste_copy_file($path){
	$file_list = '';
	$fail_file_list = '';
	$dir = gg('dir');
	$copy = ss('copyfile');
	foreach($copy as $file){
		if(is_file($path.$file)){
			$success = true;
			$basename = basename($path.$file);
			@copy($path.$file , $path.$dir.$basename) or $success = false;
			if($success) $file_list .= '<li>'.$basename.'</li>';
			else $fail_file_list .= '<li>'.$basename.'</li>';
		}
	}
	if(strlen($fail_file_list)){
		'<ul style="margin-left:40px;">'.$fail_file_list.'</ul>';
		notify_add_info(tt('Problem by pasting in this directory.').$fail_file_list);
	}
	unset($_SESSION['copyfile']);
	return $file_list;
}

/// Paste cutted files into current directory.
/// @return Rendered list of file name.
function file_paste_cut_file($path){
	$file_list = '';
	$fail_file_list = '';
	$dir = gg('dir');
	$copy = ss('cutfile');
	foreach($copy as $file){
		if(is_file($path.$file)){
			$success = true;
			$basename = basename($path.$file);
			@rename($path.$file , $path.$dir.$basename) or $success = false;
			if($success) $file_list .= '<li>'.$basename.'</li>';
			else $fail_file_list .= '<li>'.$basename.'</li>';
		}
	}
	if(strlen($fail_file_list)){
		'<ul style="margin-left:40px;">'.$fail_file_list.'</ul>';
		notify_add_info(tt('Problem by pasting in this directory.').$fail_file_list);
	}
	unset($_SESSION['cutfile']);
	return $file_list;
}

/// Render directory navigation.
/// @param $dir : directory name for navigation rendering.
/// @param $mode : called mode of file manager.
/// @return Rendered directory navigation.
function file_get_dir_nav($dir, $mode){
	$dir_nav='<a href="?mode='.$mode.'">Home</a>';
	$split_dir = split('\/',$dir);
	$path = '';
	foreach($split_dir as $value){
		if(strlen($value) > 0){
			$path .= $value.'/';
			$dir_nav .= '&#160;&#187;&#160;<a href="?mode='.$mode.'&amp;dir='.$path.'">'.$value.'</a>';
		}
	}
	return $dir_nav.'&#160;&#160;'.file_get_paste_icon($dir, $mode);
}


/// Render paste icon in a directory, if there are files to paste
/// @return Rendered paste icon
function file_get_paste_icon($dir, $mode){
	if(ss('copyfile') || ss('cutfile')){
		$dir_nav  = '<a href="'.SECTION_URI.'?mode=file_paste&amp;dir='.$dir.'&amp;next_mode='.$mode.'">';
		$dir_nav .= '<img src="'.ROOT_URI.'files/icon/editpaste.png" title="paste" border="0"/>';
		$dir_nav .= tt('paste files');
		$dir_nav .= '</a>';
		return $dir_nav;
	}
}

function file_get_mode_path($mode){
	if($mode=='file_manage_user'){
		$dir = FILES_PATH.'member/';
		if(!is_dir($dir)) mkdir($dir);
		return $dir.USER_ID.'/';
	}elseif($mode=='file_manage_admin'){
		return FILES_PATH.'';
	}else{
		$file_mode_path = cc('file_mode_path');
		return $file_mode_path[$mode];
	}
}

function file_avoid_up_dir($dir){
	if(count(split('\/\.\.', $dir)) >= 2) error_denied();
}

function file_check_allow(){
	if(SECTION == 1) return true;
	$allowed_dir = array(
		'smiley/'.SECTION.'/',
	);
	if(in_array(gg('dir'), $allowed_dir)) return true;
	return false;
}
?>
