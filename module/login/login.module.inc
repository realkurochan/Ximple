<?php
execute_add_function('login');
execute_add_function('login_ajax');
execute_add_function('login_submit');

function login(){
	document_set_title(tt('Login'));
	document_set_path(tt('Login'));
	if(authority_is_guest()) return login_render();
	else return login_render_logout();
}

function login_ajax(){
	execute_set_ajax();
	if(authority_is_guest()) return login_render();
	else return login_render_logout();
}

function login_submit(){
	execute_as_db();
	document_set_redirect('');
}

function login_render(){
	$render_array = array(
		'forget_password' => cc('forget_password'),
		'registable' => cc('registration_enable'),
		'text' => cc('registration_login_page'),
	);
	head_add_css('form.css');
	head_add_js('include/external/sha1.js');
	return render($render_array, 'login_page.tpl', 'login_page');
}

function login_render_logout(){
	head_add_css('form.css');
	head_add_js('include/external/sha1.js');
	return render(array(), 'logout_page.tpl', 'logout_page');
}
?>
