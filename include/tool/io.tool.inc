<?php

/// Read content of a file
/// @param  $file_name : file name with absolute path
/// @return Content of the file
function io_read_file($file_name){
	if(!is_file($file_name)) $file_name = ROOT_PATH.$file_name;
	if(IS_APC and $cache = apc_fetch('read_file_'.$file_name)) return $cache;
	$content = '';
	if(is_file($file_name)){
		$file = fopen($file_name,'r');
		$size = filesize($file_name);
		if($size) $content = fread($file, $size);
	}
	if(IS_APC) apc_add('read_file_'.$file_name, $content);
	return $content;
}

/// Write content into a file.
/// @param $file_name : file name with absolute path
/// @param $content   : input content to write
function io_write_file($file_name, $content, $mode='w'){
	if(is_file($file_name)){
		$file = @fopen($file_name, $mode);
	}else{
		$file = @fopen($file_name, 'x');
	}
	if($file){
		flock($file, LOCK_EX);
		fwrite($file,$content);
		flock($file, LOCK_UN);
		fclose($file);
	}elseif(state_is_develop()){
		notify_add_error('Error by writing file : '.$file_name);
	}
	if(IS_APC) apc_store('read_file_'.$file_name, $content);
}

function io_drop_file($file_name){
	$size = filesize($file_name);
	userinfo_decrease_used_space(USER_ID, $size);
	unlink($file_name);
}

function io_append_file($file_name, $content){
	io_write_file($file_name, $content, 'a');
}

function io_drop_uploaded_file($file_name){
	$p = tt('Problem by dropping file : %s.');
	$size = filesize($file_name);
	@unlink($file_name) or notify_add_error(sprintf($p, $file_name));
	userinfo_increase_used_space(USER_ID, $size);
}

function io_clear_directory($dir_name){
	if(is_dir($dir_name)){
		$success = true;
		@$dir = dir($dir_name) or $success = false;
		while ($success and (false !== ($entry = $dir->read()))) {
			if($entry != '.' and $entry != '..'){
				if(is_dir($dir_name.'/'.$entry)){
					io_clear_directory($dir_name.'/'.$entry);
				}else{
					unlink($dir_name.'/'.$entry);
				}
			}
		}
		if ($success) rmdir($dir_name);
	}
}

function io_copy_directory($source, $dest){
	if(is_dir($source) and is_dir($dest)){
		$dir = dir($source);
		while (false !== ($entry = $dir->read())) {
			if($entry != '.' and $entry != '..'){
				if(is_dir($source.'/'.$entry)){
					if(!is_dir($dest.'/'.$entry)) mkdir($dest.'/'.$entry);
					io_copy_directory($source.'/'.$entry, $dest.'/'.$entry);
				}else{
					copy($source.'/'.$entry, $dest.'/'.$entry);
				}
			}
		}
	}elseif(state_is_develop()){
		notify_add_error('Directory : '.$source.' or '.$dest.' does not exist. Files cannot be copied.');
	}
}

function io_get_directory_entry($dir_name){
	$entry_list = array();
	if(is_dir($dir_name)){
		$dir = dir($dir_name);
		while (false !== ($entry = $dir->read())) {
			if($entry != '.' and $entry != '..'){
				$entry_list[] = $entry;
			}
		}
	}elseif(state_is_develop()){
		notify_add_error('Directory : '.$dir_name.' does not exist (io_get_directory_entry).');
	}
	return $entry_list;
}

function io_serialize($data, $file_name){
	$txt = '<?php die();'.serialize($data).'?>';
	io_write_file($file_name.'.php', $txt);
	if(IS_APC) apc_store('serialize_'.$file_name, $data);
}

function io_unserialize($file_name){
	if(is_file($file_name.'.php')){
		if(IS_APC and $cache = apc_fetch('serialize_'.$file_name)) return $cache;
		$txt = io_read_file($file_name.'.php');
		$txt = substr($txt, 12, -2);
		$data = unserialize($txt);
		if(IS_APC) apc_add('serialize_'.$file_name, $data);
		return $data;
	}
}

/// Upload file to server with file size and user space checking.
/// MAX_UPLOADED_FILE_SIZE, MAX_USER_SPACE_SIZE are defined in ROOT_PATH/include/setup.php
/// @param $file : meta data of upload file (mostly from $_FILE).
/// @param $path : destination path.
/// @param $is_image : IF TRUE, only .png, .jpeg and .jpg are allowed.
/// @param $increase_space : IF TRUE, increase used space of user.
/// @return File path for success, null else.
function io_upload($file, $path, $is_image=false, $increase_space=true, $original_name=false){
	include_once(ROOT_PATH.'include/tool/userinfo.tool.inc');
	if($file and !io_check_image($file['name'], $is_image)){
		notify_add_error(tt('Only .png, .jpeg and .jpg are allowed to be uploaded.'));
		return null;
	}
	
	if(!io_check_upload_size($file)) return null;
	$name = io_get_upload_file_name($file, $path, $original_name);
	if(USER_ID != -1 and $increase_space and LIMIT_UPLOAD){
		userinfo_increase_used_space(USER_ID, $file['size']);
	}
	return $name;
}


function io_check_upload_size($file){
	if($file['size'] == 0) return false;
	if(LIMIT_UPLOAD and $file['size'] > MAX_UPLOADED_FILE_SIZE){
		notify_add_warning(tt('Size of uploaded file exceeds the limit.'));
		return false;
	}
	if(LIMIT_UPLOAD and $file['size'] > MAX_USER_SPACE_SIZE){
		notify_add_warning(tt('Size of uploaded file exceeds the limit.'));
		return false;
	}
	return true;
}

function io_get_upload_file_name($file, $path, $original_name){
	if($original_name){
		$name = $path.io_format_upload_file_name($file['name']);
	}else{
		$name = $path.md5($file['name']).'.'.end(explode('.', $file['name']));
	}
	
	$name = io_upload_check_exist_file($file, $name);
	
	if(substr($name, 0, strlen(ROOT_PATH)) == ROOT_PATH){
		$name = substr($name, strlen(ROOT_PATH));
	}
	return $name;
}

function io_format_upload_file_name($name){
	$name = str_replace(' ', '-', $name);
	return $name;
}

function io_upload_check_exist_file($file, $name){
	$i = 0;
	while(true){
		if(!is_file($name)){
			move_uploaded_file($file['tmp_name'], $name);
			break;
		}else{
			if($original_name) $name = $path.$i.'_'.$file['name'];
			else $name = $path.md5($file['name'].$i).'.'.end(explode('.', $file['name']));
			$i++;
		}
	}
	return $name;
}

function io_check_image($name, $is_image){
	if(!$is_image) return true;
	if(strlen($name) == 0) return true;
	$splitted = split('\.', $name);
	$ext = strtolower($splitted[1]);
	return ($ext == 'png' or $ext == 'jpeg' or $ext == 'jpg');
}

function io_write_log($log, $type){
	if(cc('syslog_enable')){
		$log = str_replace(',', '', trim($log));
		$log = time().','.ss('ximple_id').','.ss('login_name').','.vv('REQUEST_URI').','.$type.','.$log."\n";
		io_write_file(io_get_log_file(), $log, 'a+');
	}
}

function io_get_log_file($prevois_day=0){
	return io_get_loop_file('log', ceil(cc('syslog_time')/86400), $prevois_day);
}

function io_get_loop_file($loop_name, $interval, $prevois_day=0){
	$log_dir = ROOT_PATH.'files/'.$loop_name.'/';
	if(!is_dir($log_dir)) mkdir($log_dir);
	$log_dir.= SECTION.'/';
	if(!is_dir($log_dir)) mkdir($log_dir);
	$year = ((int) date('y'))*366;
	$day = ((int) date('z')) - $prevois_day;
	$slot = ceil($day/$interval);
	$number = $day%$interval;
	$previos = $log_dir.$year.'_'.($slot-1).'_'.$number.'.txt';
	if(is_file($previos)) unlink($previos);
	return $log_dir.$year.'_'.$slot.'_'.$number.'.txt';
}


function io_get_file_extension($file_name){
	$splitted = split('\.', basename($file_name));
	return $splitted[count($splitted) - 1];
}
?>
