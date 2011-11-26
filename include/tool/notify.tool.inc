<?php

execute_add_function('notify_check');
execute_add_function('notify_mark_as_read');

global $notify, $notify_savable, $notify_sent_email;
$notify = array();
$notify_savable = true;
$notify_sent_email = array();

function notify_add_info($message, $uid=USER_ID){
	notify_add($message, 'info', $uid);
}

function notify_add_tip($message, $uid=USER_ID){
	notify_add($message, 'tip', $uid);
}

function notify_add_warning($message, $uid=USER_ID){
	notify_add($message, 'warning', $uid);
	io_write_log($message, 'notify_warning');
}

function notify_add_error($message, $uid=USER_ID){
	notify_add($message, 'error', $uid);
	io_write_log($message, 'notify_error');
}

function notify_add($message, $mode, $uid){
	global $notify;
	if(!isset($notify[$uid]['number']) or $notify[$uid]['number'] == 0){
		$notify[$uid] = array(
			'number' => 0,
			'info' => '',
			'tip' => '',
			'warning' => '',
			'error' => '',
		);
	}
	$notify[$uid]['number']++;
	$notify[$uid][$mode] .= '<li>'.$message.'</li>';
}

function notify_save(){
	global $notify, $notify_savable;
	if($notify_savable){
		foreach($notify as $uid => $entry){
			$file_name = notify_get_file_name($uid);
			$entry = notify_merger($entry, $file_name);
			io_serialize($entry, $file_name);
		}
	}
}

function notify_merger($entry, $file_name){
	if(is_file($file_name.'.php')){
		$old_entry = io_unserialize($file_name);
		foreach($entry as $key => $value){
			if($key == 'number') $entry[$key] += $old_entry[$key];
			else $entry[$key] = $old_entry[$key].$value;
		}
	}
	return $entry;
}

function notify_render(){
	$file_name = notify_get_file_name(USER_ID);
	$read_notify = io_unserialize($file_name);
	if($read_notify['number']){
		unlink($file_name.'.php');
		return notify_render_item($read_notify);
	}
}

function notify_render_item($notify, $show_close=true){
	$render_array = array(
		'show_close' => $show_close,
		'data' => $notify,
		'title' => array(
			'info' => tt('Information Notification'),
			'tip' => tt('Tip Notification'),
			'warning' => tt('Warning Notification'),
			'error' => tt('Error Notification'),
		),
	);
	return render($render_array, 'notify.tpl', 'notify');
}

function notify_get_file_name($uid){
	if($uid == -1){
		$file_name = session_id();
	}else{
		$file_name = (string) $uid;
	}
	$dir_name = FILES_PATH.'notify/';
	if(!is_dir($dir_name)) mkdir($dir_name);
	return $dir_name.$file_name;
}

function notify_check(){
	global $notify_savable;
	execute_set_ajax();
	$file_name = notify_get_file_name(USER_ID);
	$read_notify = io_unserialize($file_name);
	if($read_notify['number']){
		$render = notify_render_item($read_notify, false);
		$rener_array = array(
			'number' => $read_notify['number'],
			'notify' => $render,
		);
		$notify_savable = false;
		return render($rener_array, 'notify_check.tpl', 'notify_check');
	}
}

function notify_mark_as_read(){
	global $notify_savable;
	$file_name = notify_get_file_name(USER_ID);
	unlink($file_name.'.php');
	$notify_savable = true;
}


function notify_subscribe($message, $uid, $reference){
	$subscribe_array = user_info_get_config($uid, 'subscribe');
	if(!is_array($subscribe_array)){
		$subscribe_array = array();
	}
	
	if(in_array($reference, $subscribe_array)){
		$user_info = userinfo_get_info_by_id($uid);
		notify_send_mail($message, $user_info['email']);
	}
}

function notify_multiple_subscribe($message, $uid, $reference){
	$subcribable_uid = array();
	foreach($uid as $key => $value){
		$subscribe_array = user_info_get_config($value, 'subscribe');
		if(!is_array($subscribe_array)){
			$subscribe_array = array();
		}
		if(in_array($reference, $subscribe_array)){
			$subcribable_uid[] = $value;
		}
	}
	
	if(count($subcribable_uid)){
		notify_send_mail($message, userinfo_get_email($subcribable_uid));
	}
}

function notify_subscribe_by_mode($message, $mode, $send_message=true){
	$uid = io_unserialize(FILES_PATH.'subscribe/'.SECTION.'/'.$mode);
	if(is_array($uid)){
		if($send_message){
			foreach($uid as $key => $value){
				notify_add_info($message, $key);
			}
		}
		notify_send_mail($message, userinfo_get_email(array_keys($uid)));
	}
}

function notify_direct_subscribe($message, $email){
	notify_send_mail($message, $email);
}

function notify_send_mail($message, $email){
	require_once('mail.tool.inc');
	global $notify_sent_email;
	if(is_array($email)){
		$email = array_diff($email, $notify_sent_email);
		$notify_sent_email = array_merge($email, $notify_sent_email);
		$email = join(',', $email);
	}else{
		$notify_sent_email[] = $email;
	}
	$subject = notify_get_subscribe_subject();
	$message = notify_unset_locale($message);
	mail_send($email, $subject, $message, true);
}

function notify_get_subscribe_subject(){
	return '['.cc('section_name').'] '.tt('Notification (not reply)');
}

function notify_unset_locale($message){
	$language = ss('language');
	if(strlen($language)) $language = $language.'/';
	$Q = str_replace($language, '', Q);
	$message = str_replace(SECTION_URI.Q, SECTION_URI.$Q, $message);
	return $message;
}

function notify_add_subscribe($module, $label){
	$subscribe = cc('subscribable_module');
	$subscribe[$module] = $label;
	config_set('subscribable_module', $subscribe);
	config_save();
}

function notify_drop_subscribe($module){
	$subscribe = cc('subscribable_module');
	unset($subscribe[$module]);
	config_set('subscribable_module', $subscribe);
	config_save();
}

function notify_get_subscribable_module(){
	$subscribe = cc('subscribable_module');
	$translated = array();
	if(is_array($subscribe)){
		foreach($subscribe as $mode => $label){
			$translated[$mode] = tt($label);
		}
	}
	$translated['comment']= tt('someone comments on my contributions');
	return $translated;
}

function notify_create_subscribe_directory(){
	$directory = FILES_PATH.'subscribe/';
	if(!is_dir($directory)) mkdir($directory);
	$directory.= SECTION.'/';
	if(!is_dir($directory)) mkdir($directory);
	return $directory;
}
?>
