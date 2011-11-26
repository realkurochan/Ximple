<?php
/// @file
/// This file provides functions for error handling in XimpleCMS.
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Fatal error : stop process and give User some information.
/// @param $title : title of error.
/// @param $message : information about error.
function error_fatal($title, $message){
	header('HTTP/1.1 500 Internal Server Error');
	$data = array(
		'title' => $title,
		'message' => $message
	);
	echo(render($data, 'error.tpl', 'error_fatal'));
	if(function_exists('io_write_log')) io_write_log($message, 'fatal');
	if(function_exists('db_rollback')) db_rollback();
	die();
}

/// Acceptable error : do not stop process,
/// give user some information, write information in log file.
/// @param $message : information about error.
/// @param $redirect : target URI for redirection.
function error_syslog($message, $redirect=null){
	header('HTTP/1.1 500 Internal Server Error');

	if($redirect) $uri = SECTION_URI.Q.$redirect;
	else $uri = SECTION_URI;

	$message .= sprintf(tt('<p>You should go to <a href="%s">this page</a> for further action.</p>'), $uri);
	$data = array(
		'title' => tt('System error occurred'),
		'message' => $message
	);
	db_rollback();
	io_write_log($message, 'system');
	echo(render($data, 'error.tpl', 'error_syslog'));
	die();
}

/// Error by access denied : not logged in User => Login form, else stop process.
/// @param $info : addition information about error.
function error_denied($info=''){
	io_write_log(tt('Invalid access for called page.'), 'denied');
	header('HTTP/1.1 402 Access Denied');
	if(authority_is_guest()){
		if(!function_exists('syswidget_login')){
			require(INCLUDE_PATH.'tool/syswidget.tool.inc');
		}
		$data = array('form' => syswidget_login());
		$message = render($data, 'error_login.tpl', 'error_login');
	}else{
		$uri = SECTION_URI;
		$message  = tt('Invalid access for called page.');
		$message .= sprintf(tt('<p>You should go to <a href="%s">this page</a> for further action.</p>'), $uri);
	}
	
	if(state_is_develop()) $message .= '<br />'.$info;
	
	$data = array(
		'title' => tt('System error occurred'),
		'message' => $message
	);
	db_rollback();
	echo(render($data, 'error.tpl', 'error_syslog'));
	die();
}

/// Database error : write log and stop process.
/// @param $qurey : database query.
/// @param $error : database error message.
function error_db_log($query, $error){
	$message = 'Database error : '.$error.' by query : '.$query.' ';
	if(!state_is_product()){
		error_syslog($message);
	}else{
		db_rollback();
		io_write_log($message, 'system');
		die();
	}
}

/// Error by database connection : stop process.
function error_db_connect(){
	$message = 'Ximple can not connect to database, please check your database setup.';
	if(!state_is_product()){
		error_fatal('DB Connection Error', $message);
	}else{
		io_write_log($message, 'db_connect');
		die();
	}
}

/// Error by page not found : stop process.
/// @param $info : addition information about error.
function error_not_found($info=''){
	header("HTTP/1.0 404 Not Found");
	$message = 'The requested page can not be found on the server.';
	if(state_is_develop())	$message .= '<br />'.$info;
	$data = array(
		'title' => 'Page not found',
		'message' => $message,
	);
	if(gg('search_id')) search_drop_index();
	echo(render($data, 'error.tpl', 'error_not_found'));
	db_rollback();
	io_write_log($message, 'not_found');
	die();
}

?>
