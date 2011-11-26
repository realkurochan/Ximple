<?php
function registration_off(){
	notify_add_error(tt('The registration function is turned off. You are not allowed to registry.'));
	execute_set_db();
	document_set_redirect('');
}

function registration_save_user($reply_mode, $text){
	if(registration_verify_data()){
		$register_dir = FILES_PATH.'registration/';
		if(!is_dir($register_dir)) mkdir($register_dir);
		list($user, $key1, $key2) = registration_get_user_data();
		io_serialize($user, $register_dir.$key1);
		$confirm_uri = SECTION_URI.Q.$reply_mode.'/key1/'.$key1.'/key2/'.$key2;
		registration_reply($confirm_uri, $text);
		execute_set_db();
		document_set_redirect('');
	}else{
		registration_error();
	}
}

function registration_verify_data(){
	$name = pp('login_name');
	$exist = userinfo_check_user_exists($name);
	$passwd = pp('password');
	$verify = !empty($name);
	$verify = ($verify and input_is_email(pp('email')));
	$verify = ($verify and !empty($passwd));
	$verify = ($verify and $exist != 'exist');
	return $verify;
}

function registration_get_user_data(){
	$key1 = sha1(rand());
	$key2 = sha1(rand());
	$user = pp();
	$user['key'] = $key2;
	
	unset($user['confirm_password']);
	unset($user['captcha']);
	unset($user['captcha_code_captcha']);
	unset($user['captcha_gentime_captcha']);
	
	$user['avatar'] = io_upload(ff('avatar'), FILES_PATH.'avatar/');
	if(!$user['avatar']) $user['avatar'] = '';
	return array($user, $key1, $key2);
}

function registration_reply($confirm_uri, $reply_text){
	$subject = tt('Register confirmation')." (".cc('section_name').")";
	$message = $reply_text."
	<p>
		".tt('Confirm URL')." : <a href=\"".$confirm_uri."\">".tt('Click here to confirm')."</a>
	</p>
	<p>
		".tt('Alternative Link')." : $confirm_uri
	</p>
	";
	include(INCLUDE_PATH.'tool/mail.tool.inc');
	if(!mail_send(pp('email'), $subject, $message, true)) registration_error();
	else notify_add_info(tt('Your registration has been successfully submitted. Please check your email to confirm the registration.'));
}

function registration_error(){
	$message = tt('System error occurred. Sorry for the inconvenience. ');
	$message.= tt('Your registration can not be proceeded. Please contact the administrator of the web-site.');
	notify_add_error($message);
}

?>
