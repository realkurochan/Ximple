<?php

if(authority_is_guest()) error_denied();
include_once(INCLUDE_PATH.'tool/icon.tool.inc');

function file_manage_user(){
	$dir = gg('dir');
	if(!$dir) $dir = '';
	$root_path = file_get_mode_path('file_manage_user');
	if(!is_dir($root_path)) mkdir($root_path);
	$root_path .= $dir;
	$title = tt('File Manager');
	document_set_title($title);
	document_set_path($title);
	return file_manage($root_path, $dir, 'file_manage_user', $title);
}

/// Render the user-interface to file manager for system administrator.
/// The root path set to ROOT_PATH/files.
/// @return Associate array of rendered user-interface to file manager.
function file_manage_admin(){
	if(!(authority_group('file_admin') and file_check_allow())){
		error_denied();
	}
	$dir = gg('dir');
	if(!$dir) $dir = '';
	$root_path = file_get_mode_path('file_manage_admin');
	$root_path .= $dir;
	$title = tt('System File Manager');
	document_set_title($title);
	document_set_path($title);
	return file_manage($root_path, $dir, 'file_manage_admin', $title);
}

/// Render the user-interface to file manager.
/// @param $root_path :	root path of file manager.
/// @param $dir : opened directory.
/// @param $mode : called mode of file manager
/// @param $title : title of file manager.
/// @return Rendered user-interface to file manager.
function file_manage($root_path, $dir, $mode, $title){
	include_once(INCLUDE_PATH.'tool/userinfo.tool.inc');
	file_avoid_up_dir($root_path);
	$file_list = file_get_list($root_path, $dir, $mode);
	$file_list = file_sort($file_list, $root_path);
	
	$user_used_space = userinfo_get_used_space(USER_ID);
	$free_space = MAX_USER_SPACE_SIZE-$user_used_space;
	$render_data = array(
		'title' => $title,
		'dir_nav' => file_get_dir_nav($dir, $mode),
		'dir' => $dir,
		'mode' => $mode,
		'file_copy_list' => file_copy_list(),
		'file_cut_list' => file_cut_list(),
		'filelist' => $file_list,
		'uri' => SECTION_URI.'?mode='.$mode.'&amp;dir='.$dir,
		'user_used_space' => render_file_size($user_used_space),
		'free_space' => render_file_size($free_space),
	);
	$render = render($render_data, 'module/file/systemtheme/file.tpl', 'file_manager');
	head_add_css('list.css');
	head_add_css('form.css');
	head_add_css('file.css');
	return $render;
}

/// Get array of file list and file information.
/// @param $root_path : Root-Path for file manager.
/// @param $dir : opened directory.
/// @param $mode : called mode of file manager.
/// @return Array of file name, file changed time, file size and file type.
function file_get_list($root_path, $dir, $mode){
	$file_list = array();
	@$d = dir($root_path) or error_denied();
	while (false !== ($entry = $d->read())) {
		if($entry != '.' and $entry != '..'){
			if(strlen($entry) < 30) $name = $entry;
			else $name = substr($entry, 0, 30).'...';
			
			$file_list[$entry]['permission'] = file_get_mod($root_path.$entry);
			if(is_dir($root_path.$entry)){
				$file_list = file_set_dir_info($file_list, $entry, $dir, $mode, $name);
			}else{
				$file_list = file_set_info($file_list, $entry, $root_path, $name);
			}
			
			$file_list = file_set_icon($file_list, $entry);
			$file_list = file_set_time($file_list, $entry, $root_path);
			$file_list = file_set_size($file_list, $entry, $root_path);
		}
	}
	$d->close();
	return $file_list;
}


/// Sort file list by given parameter.
/// @prara $file_list : array of files in directory.
/// @param $root_path : directory relative to ROOT_PATH.
/// @return Sorted file list.
function file_sort($file_list, $root_path){
	$order = gg('order');
	if(!$order) $order = 'name';
	$func = 'file_sort_by_'.$order;
	$time_list = array();
	$type_list = array();
	$size_list = array();
	
	$n = count($file_list);
	$i = 0;
	foreach($file_list as $key => $value){
		$type_list[$value['type'].$key] = $key;
		$time_list[($value['timestamp']*$n + $i).''] = $key;
		$size_list[filesize($root_path.$key)*$n + $i] = $key;
		$i++;
	}
	
	$file_list = $func($file_list, $time_list, $type_list, $size_list);
	return $file_list;
}


/// Set directory information of given file list.
/// @prara $file_list : array of files in directory.
/// @param $entry : directory name in file list to get information.
/// @param $dir : directory name for file manager.
/// @param $mode : called mode of file manager.
/// @param $name : short name of directory.
/// @return File list with directory information.
function file_set_dir_info($file_list, $entry, $dir, $mode, $name){
	$file_list[$entry]['type'] = '.dir';
	$file_list[$entry]['href'] = '<a href="?mode='.$mode.'&amp;dir='.$dir.$entry.'/">'.$name.'</a>';
	$file_list[$entry]['is_dir'] = true;
	return $file_list;
}

/// Set file information of given file list.
/// @prara $file_list : array of files in directory.
/// @param $entry : directory name in file list to get information.
/// @param $root_path : Root-Path for file manager.
/// @param $name : short name of directory.
/// @return File list with file information.
function file_set_info($file_list, $entry, $root_path, $name){
	$image_type = array("png", "jpeg", "jpg", "gif", "tif");
	$type_name = split('\.',$entry);
	$type_name = strtolower($type_name[count($type_name)-1]);
	
	$file_list[$entry]['type'] = $type_name;
	$uri = ROOT_URI.str_replace(ROOT_PATH, '',$root_path).$entry;
	
	if(in_array($type_name, $image_type)){
		$file_list[$entry]['href'] = '<a href="javascript:showImage(\''.$uri.'\')" >'.$name.'</a>';
	}else{
		$file_list[$entry]['href'] = '<a href="'.$uri.'" target="_blank">'.$name.'</a>';
	}
	$file_list[$entry]['uri'] = $uri;
	$file_list[$entry]['is_dir'] = false;
	return $file_list;
}

/// Set file size information of given file list.
/// @prara $file_list : array of files in directory.
/// @param $entry : directory name in file list to get information.
/// @param $root_path : Root-Path for file manager.
/// @return File list with file size information.
function file_set_size($file_list, $entry, $root_path){
	$file_size = filesize($root_path.$entry);
	$file_list[$entry]['size'] = render_file_size($file_size);
	return $file_list;
}

/// Set file changed time information of given file list.
/// @prara $file_list : array of files in directory.
/// @param $entry : directory name in file list to get information.
/// @param $root_path : Root-Path for file manager.
/// @return File list with file changed time information.
function file_set_time($file_list, $entry, $root_path){
	$file_list[$entry]['timestamp'] = filectime($root_path.$entry);
	$file_list[$entry]['time'] = date("d-m-y",$file_list[$entry]['timestamp']);
	return $file_list;
}

/// Set file icon of given file list.
/// @prara $file_list : array of files in directory.
/// @param $entry : directory name in file list to get information.
/// @return File list with file icon.
function file_set_icon($file_list, $entry){
	$icons = icon();
	if(isset($icons[$file_list[$entry]['type']])){
		$file_list[$entry]['icon'] = ROOT_URI.'files/icon/'.$icons[$file_list[$entry]['type']];
	}else{
		$file_list[$entry]['icon'] = ROOT_URI.'files/icon/'.$icons['unknown'];
	}
	return $file_list;
}

/// Get permision information of a file.
/// @param $filename : file name to get permission information.
/// @return String of permision information ("'owner', 'group', 'or', 'ow', 'ox', 'gr', 'gw', 'gx', 'ar', 'aw', 'ax'").
function file_get_mod($filename){
	$owner = posix_getpwuid(fileowner($filename));
	$group = posix_getgrgid($owner['gid']);
	$perms = fileperms($filename);
	$permarray['or'] = (($perms & 0x0100) ? '1' : '0');
	$permarray['ow'] = (($perms & 0x0080) ? '1' : '0');
	$permarray['ox'] = (($perms & 0x0040) ? (($perms & 0x0800) ? '0' : '1' ) : '0' );
	$permarray['gr'] = (($perms & 0x0020) ? '1' : '0');
	$permarray['gw'] = (($perms & 0x0010) ? '1' : '0');
	$permarray['gx'] = (($perms & 0x0008) ? (($perms & 0x0400) ? '0' : '1' ) : '0' );
	$permarray['ar'] = (($perms & 0x0004) ? '1' : '0');
	$permarray['aw'] = (($perms & 0x0002) ? '1' : '0');
	$permarray['ax'] = (($perms & 0x0001) ? (($perms & 0x0200) ? '0' : '1' ) : '0' );
	return "'$owner[name]', '$group[name]', '".join("', '", $permarray)."'";
}

/// Sort file by type.
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @return Array of file name with sorting by type.
function file_sort_by_type($file_list,$time_list,$type_list){
	ksort($type_list);
	foreach($type_list as $key=>$value){
		$out[$value] = $file_list[$value];
	}
	return($out);
}

/// Sort file by type (backward).
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @return Array of file name with sorting by type (backward).
function file_sort_by_type_desc($file_list,$time_list,$type_list){
	krsort($type_list);
	foreach($type_list as $key=>$value){
		$out[$value] = $file_list[$value];
	}
	return($out);
}

/// Sort file by size.
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @param $size_list : array of file size.
/// @return Array of file name with sorting by size.
function file_sort_by_size($file_list,$time_list,$type_list,$size_list){
	ksort($size_list);
	foreach($size_list as $key=>$value){
		$out[$value] = $file_list[$value];
	}
	return($out);
}

/// Sort file by size (backward).
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @param $size_list : array of file size.
/// @return Array of file name with sorting by size (backward).
function file_sort_by_size_desc($file_list,$time_list,$type_list,$size_list){
	krsort($size_list);
	foreach($size_list as $key=>$value){
		$out[$value] = $file_list[$value];
	}
	return($out);
}

/// Sort file by time.
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @return Array of file name with sorting by time.
function file_sort_by_time($file_list,$time_list,$type_list){
	ksort($time_list);
	foreach($time_list as $key=>$value){$out[$value] = $file_list[$value];}
	return($out);
}

/// Sort file by time (backward).
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @return Array of file name with sorting by time (backward).
function file_sort_by_time_desc($file_list,$time_list,$type_list){
	krsort($time_list);
	foreach($time_list as $key=>$value){$out[$value] = $file_list[$value];}
	return($out);
}


/// Sort file by name.
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @return Array of file name with sorting by name.
function file_sort_by_name($file_list,$time_list,$type_list){
	ksort($file_list);
	return $file_list;
}


/// Sort file by name (backward).
/// @param $file_list : array of file name.
/// @param $time_list : array of changed time of files.
/// @param $type_list : array of file type.
/// @return Array of file name with sorting by name (backward).
function file_sort_by_name_desc($file_list,$time_list,$type_list){
	krsort($file_list);
	return $file_list;
}
?>
