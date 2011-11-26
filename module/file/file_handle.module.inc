<?php

if(authority_is_guest()) error_denied();

/// Create a directory for file manager.
function file_create_dir(){
	$dir = gg('dir');
	$new_dir = pp('new_dir');
	file_avoid_up_dir($dir);
	$path = file_get_mode_path(gg('next_mode')).$dir.'/';
	
	$success=true;
	if(is_dir($path)) @mkdir($path.$new_dir) or $success=false;
	
	if($success) notify_add_info(sprintf(tt('Directory <b>%s</b> has been created.'), $new_dir));
	else notify_add_error(sprintf(tt('Problem by creating directory <b>%s</b>.'), $new_dir));
	
	execute_set_db();
	document_set_redirect('?mode='.gg('next_mode').'&dir='.$dir);
}

function file_upload(){
	$dir = gg('dir');
	file_avoid_up_dir($dir);
	$path = file_get_mode_path(gg('next_mode')).$dir.'/';
	if(is_dir($path)){
		$file_array = input_reorder_multiple_files('file');
		foreach($file_array as $key => $value){
			$fname = io_upload($value, $path, false, true, true);
			if($value['size']){
				if($fname){
					$message = sprintf(tt('File <b>%s</b> has been uploaded.'), $fname);
					notify_add_info($message);
				}else{
					notify_add_error(sprintf(tt('Problem by uploading.')));
				}
			}
		}
	}
	
	execute_set_db();
	document_set_redirect('?mode='.gg('next_mode').'&dir='.$dir);
}

/// Delete a file from system.
function file_drop(){
	$dir = gg('dir');
	$file = gg('file');
	$mode = gg('next_mode');
	file_avoid_up_dir($dir);
	$success = true;
	$path = file_get_mode_path($mode).$dir.'/'.$file;
	
	if(is_dir($path)) @rmdir($path) or $succesee=false;
	elseif(is_file($path)) io_drop_file($path);
	
	if($success) notify_add_info(sprintf(tt('File or directory <b>%s</b> has been dropped.'), $path));
	else notify_add_error(sprintf(tt('Problem by dropping <b>%s</b>.'), $path));
	
	execute_set_db();
	document_set_redirect('?mode='.$mode.'&dir='.$dir);
}

/// Delete a many files at the same time from system.
function file_drop_multiple(){
	$dir = gg('dir');
	$mode = gg('next_mode');
	file_avoid_up_dir($dir);
	$root_path = file_get_mode_path($mode);
	$file_list = '';
	$fail_file_list = '';
	$get = gg();
	foreach($get as $key => $filename){
		if(substr($key, 0, 5) == 'file_'){
			$success = true;
			$path = $root_path.$filename;
			if(is_dir($path)) @ rmdir($path) or $success=false;
			else io_drop_file($path);
			
			if($success) $file_list .= '<li>'.$filename.'</li>';
			else $fail_file_list .= '<li>'.$filename.'</li>';
		}
	}
	
	if(strlen($file_list)){
		$file_list = '<ul style="margin-left:40px;">'.$file_list.'</ul>';
		notify_add_info(tt('Followed files have been dropped.').$file_list);
	}
	if(strlen($fail_file_list)){
		$fail_file_list = '<ul style="margin-left:40px;">'.$fail_file_list.'</ul>';
		notify_add_error(tt('Problem by dropping followed files.').$fail_file_list);
	}
	
	execute_set_db();
	document_set_redirect('?mode='.$mode.'&dir='.$dir);
}

/// Change mod of the selected file.
function file_chmod(){
	$dir = pp('dir');
	$file = pp('file');
	$mode = gg('next_mode');
	file_avoid_up_dir($dir);
	$success=true;
	$perm = 0;
	foreach(array('o', 'g', 'a') as $oga){
		foreach(array('r', 'w', 'x') as $rwx){
			$perm += pp($oga.$rwx);
		}
	}
	$path  = file_get_mode_path($mode).$dir.'/'.$file;
	if(is_file($path) or is_dir($path)){
		@chmod($path, octdec($perm)) or $success=false;
	}
	
	if($success) notify_add_info(sprintf(tt('Mod of file <b>%s</b> has been changed.'), $file));
	else notify_add_error(sprintf(tt('Problem by changing mod of <b>%s</b>.'), $file));
	execute_set_db();
	document_set_redirect('?mode='.$mode.'&dir='.$dir);
}

/// Rename the selected file.
function file_rename(){
	$dir = pp('dir');
	$file = pp('file');
	$mode = gg('next_mode');
	file_avoid_up_dir($dir);
	$success=true;
	$path  = file_get_mode_path($mode).$dir.'/';
	$old_name = $path.$file;
	$new_name = $path.pp('new_name');
	if(is_file($old_name) or is_dir($old_name)){
		 @rename($old_name, $new_name) or $success=false;
	}
	if($success) notify_add_info(sprintf(tt('File <b>%s</b> has been renamed to <b>%s</b>.'), $old_name, $new_name));
	else notify_add_info(sprintf(tt('Problem by renaming <b>%s</b>.'), $old_name));
	execute_set_db();
	document_set_redirect('?mode='.$mode.'&dir='.$dir);
}

/// Get the selected files to the cut list by using AJAX.
function file_cut(){
	file_set_session_file_list('cutfile');
	$list = '';
	if(gg('clear')) unset($_SESSION['cutfile']);
	else $list = file_cut_list();
	$list .= '<a href="javascript:clearcut()"><b>'.tt('clear files to cut').'</b></a>';
	execute_set_ajax();
	return $list;
}

/// Get the selected files to the copy list by using AJAX.
function file_copy(){
	file_set_session_file_list('copyfile');
	$list = '';
	if(gg('clear')) unset($_SESSION['copyfile']);
	else $list = file_copy_list();
	$list .= '<a href="javascript:clearcopy()"><b>'.tt('clear files to copy').'</b></a>';
	execute_set_ajax();
	return $list;
}

/// Paste the select files to current directory using ajax.
function file_paste(){
	$dir = gg('dir');
	$mode = gg('next_mode');
	file_avoid_up_dir($dir);
	$path  = file_get_mode_path($mode);
	$file_list = '';
	if(ss('copyfile')) $file_list .= file_paste_copy_file($path);
	if(ss('cutfile'))  $file_list .= file_paste_cut_file($path);
	$file_list = '<ul style="margin-left:40px;">'.$file_list.'</ul>';
	notify_add_info(tt('Following files have been pasted in this directory.').$file_list);
	execute_set_db();
	document_set_redirect('?mode='.$mode.'&dir='.$dir);
}

?>
