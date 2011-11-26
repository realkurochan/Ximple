<?php
function install_check_database(){
	if(DB_TYPE=='mysql')      list($next_enable_1, $result) = install_check_mysql();
	elseif(DB_TYPE=='pgsql')  list($next_enable_1, $result) = install_check_pgsql();
	elseif(DB_TYPE=='sqlite') list($next_enable_1, $result) = install_check_sqlite();
	else{
		$message = tt('Invalid DB_TYPE! Ximple 0.3 supports only MySQL(mysql), PosgreSQL(pgsql), SQLite(sqlite).');
		$result .= '<b>DB_TYPE :</b> <b class="invalid">'.$message.'</b><br />';
		$next_enable = 'disabled="disabled"';
	}
	return array($next_enable_1, $result);
}

/// Render Step of Installation.
function install_step_render(){
	if(isset($_GET['mode']))$get_step = str_replace('step', '',$_GET['mode']);
	else $get_step = 0;
	$step_label = install_get_step_label();
	
	if(!isset($_SESSION['step'])) $_SESSION['step'] = 0;
	if($get_step > $_SESSION['step']) $_SESSION['step'] = $get_step;
	$step = $_SESSION['step'];
	
	$render = '<table><tr>';
	for($i=0; $i<=$step; $i++){
		if($i == $get_step){
			$render .= '
			<td align="center" valign="top" class="selected">
				<h1>'.$i.'</h1>'.$step_label[$i].'
			</td>';
		}else{
			$render .= '<td class="enable_step" align="center" valign="top">
				<a href="?mode=step'.$i.'"><h1>'.$i.'</h1>'.$step_label[$i].'
			</td>';
		}
	}
	
	for($i=$step+1; $i<=5; $i++){
		$render .= '<td class="unable_step" align="center" valign="top">
			<h1>'.$i.'</h1>'.$step_label[$i].'
		</td>';
	}
	$render .= '</tr></table>';
	return $render;
}

function install_set_lang(){
	if(isset($_GET['lang'])){
		define('LANGUAGE', $_GET['lang']);
		$_SESSION['lang'] = $_GET['lang'];
	}elseif(isset($_SESSION['lange'])){
		define('LANGUAGE', $_SESSION['lang']);
	}else{
		define('LANGUAGE', 'en');
	}
}

/// Render selection for interface language selection.
function install_language_selection(){
	$lang = array(
		'en' => 'English',
		'th' => 'ภาษาไทย',
	);
	
	$option = '';
	foreach($lang as $key=>$value){
		$option .= '<option value="'.$key.'">'.$value.'</option>';
	}
	
	if(isset($_GET['mode'])) $selection = '<input name="mode" type="hidden" value="'.$_GET['mode'].'"/>';
	else $selection = '';
	$selection .= '
	<select name="lang" onchange="submit()">
		<option value="'.LANGUAGE.'">'.$lang[LANGUAGE].'</option>
		'.$option.'
	</select>
	';
	return $selection;
}

/// Check MySQL connection.
/// @return Result of connection checking.
function install_check_mysql(){
	$page = '<b>DB_TYPE :</b> <b class="valid">OK</b><br />';
	$invalid = tt('Invalid !!!');
	$result = array('<b>DB_HOST :</b> <b class="valid">OK</b><br />
	<b>DB_USER :</b> <b class="valid">OK</b><br />
	<b>DB_PASS :</b> <b class="valid">OK</b><br />', true);
	
	@ $dbh = mysql_pconnect(DB_HOST,DB_USER,DB_PASS) or 
	$result = array('<b>DB_HOST :</b> <b class="invalid">'.$invalid.'</b><br />
	<b>DB_USER :</b> <b class="invalid">'.$invalid.'</b><br />
	<b>DB_PASS :</b> <b class="invalid">'.$invalid.'</b><br />', false);
	$page = $result[0];
	$next_enable = $result[1];
	
	$result = array('<b>DB_NAME :</b> <b class="valid">OK</b><br />', true);
	@ mysql_select_db(DB_NAME,$dbh) or $result = array('
	<b>DB_NAME :</b> <b class="invalid">'.$invalid.'</b><br />', false);
	$page .= $result[0];
	$next_enable = $result[1];
	
	return array($next_enable, $page);
}

/// Check PostgreSQL connection.
/// @return Result of connection checking.
function install_check_pgsql(){
	$page .= '<b>DB_TYPE :</b> <b class="valid">OK</b><br />';
		
	$result = array('<b>DB_HOST :</b> <b class="valid">OK</b><br />
	<b>DB_USER :</b> <b class="valid">OK</b><br />
	<b>DB_PASS :</b> <b class="valid">OK</b><br />', true);
	
	@ $dbh = pg_connect('host='.DB_HOST.' user='.DB_USER.' password='.DB_PASS.' dbname='.DB_NAME) or
	$result = array('<b>DB_HOST :</b> <b class="invalid">Invalid !!!</b><br />
	<b>DB_USER :</b> <b class="invalid">Invalid !!!</b><br />
	<b>DB_PASS :</b> <b class="invalid">Invalid !!!</b><br />', false);
	$page .= $result[0];
	$next_enable = $result[1];
	
	return array($next_enable, $page);
}

/// Check SQLite connection.
/// @return Result of connection checking.
function install_check_sqlite(){
	$result = array('<b>DB_TYPE :</b> <b class="valid">OK</b><br />', true);
	
	@ $dbh = sqlite_open(FILES_PATH.'sqlite.data', 0766, $sqliteerror) or
	$result = array(
		'<b>DB_TYPE :</b> <b class="invalid">Trouble with SQLite connection ('.$sqliteerror.'). You should change permission of '.ROOT_PATH.'files to 0757</b><br />',
		false
	);
	$page = $result[0];
	$next_enable = $result[1];
	return array($next_enable, $page);
}

/// Check ROOT_PATH.
/// @return Result of ROOT_PATH checking.
function install_check_path(){
	$valid_path = file_exists(ROOT_PATH.'include/tool/main.tool.inc');
	if($valid_path){
		$page ='<b>ROOT_PATH :</b> <b class="valid">OK</b><br />';
		$next_enable = true;
	}else{
		$root_path = dirname(dirname($_SERVER['SCRIPT_FILENAME']));
		$info = '';
		if(substr($root_path, -1) != "/"){
			$info = tt('The last character of ROOT_PATH must be "/".');
		}
		$page ='<b>ROOT_PATH :</b> <b class="invalid">Invalid !!! Please try to use '.$root_path.'/. '.$info.'</b><br />';
		$next_enable = false;
	}
	
	return array($next_enable, $page);
}

/// Check ROOT_URI.
/// @return Result of ROOT_URI checking.
function install_check_uri(){
	$uri = $_SERVER['HTTP_HOST'].dirname(dirname($_SERVER['PHP_SELF']));
	if(substr($uri,-1) == "/"){
		$uri = substr($uri,0,-1);
	}
	$root_uri = 'http://'.$uri.'/';
	if($root_uri == ROOT_URI){
		$page ='<b>ROOT_URI :</b> <b class="valid">OK</b><br />';
		$next_enable = true;
	}else{
		$root_uri2 = 'http://www.'.$uri.'/';
		if($root_uri == ROOT_URI){
			$page ='<b>ROOT_URI :</b> <b class="valid">OK</b><br />';
			$next_enable = true;
		}else{
			$info = '';
			if(substr(ROOT_URI, -1) != "/"){
				$info = tt('The last character of ROOT_URI must be "/".');
			}
			$page ='<b>ROOT_URI :</b> <b class="invalid">Invalid !!! Please try to use '.$root_uri.' or '.$root_uri2.'. '.$info.'</b><br />';
			$next_enable = false;
		}
	}
	return array($next_enable, $page);
}

/// Check DEFAULT_LANGUAGE.
/// @return Result of DEFAULT_LANGUAGE checking.
function install_check_lang(){
	$default = DEFAULT_LANGUAGE;
	if(in_array($default, array('th', 'en'))){
		$page ='<b>DEFAULT_LANGUAGE :</b> <b class="valid">OK</b><br />';
		$next_enable = true;
	}else{
		$message = tt('Invalid !!! Ximple 0.3 supports English(en) and Thai(th)');
		$page ='<b>DEFAULT_LANGUAGE :</b> <b class="invalid">'.$message.'</b>';
		$next_enable = false;
	}
	return array($next_enable, $page);
}

function install_directory_list(){
	return array(
		'files/about', 'files/account', 'files/avatar', 'files/cache', 'files/category',
		'files/config', 'files/count', 'files/customtheme', 'files/dbcache', 'files/log', 
		'files/member', 'files/misc', 'files/notify', 'files/precache', 'files/registration', 
		'files/state', 'files/tracker', 'files/xml',
	);
}

function install_error_setup_not_exist(){
	$root_uri = 'http://'.$_SERVER['HTTP_HOST'].dirname(dirname($_SERVER['PHP_SELF'])).'/';
	define('ROOT_URI', $root_uri);
	
	$setup_file = dirname(dirname(__FILE__));
	$message = sprintf('Please create file : %s', $setup_file.'/include/setup.php');
	$body  = '<h3 style="color:red;">'.$message.'</h3>';
	$message = sprintf('You can use %s as example', $setup_file.'/include/setup.example.php');
	$body .= '<h3 style="color:red;">'.$message.'</h3>';
	$message = 'You can not pass this step, before you create setup-file.';
	$body .= '<h3 style="color:red;">'.$message.'</h3>';
	$body .= '<h3 style="color:green;"><a href="'.$root_uri.'install">Please, try again.</a></h3>';
	$agree = false;
	
	$title = tt('File not found : setup.php');
	$select_langauge = install_language_selection();
	$step = install_step_render();
	include('template/install.tpl');
	die();
}

/// Create tables in database.
function install_create_db_table(){
	install_db_init();
	if(!db_exist_table('users')){
		$filename = ROOT_PATH.'install/database/'.DB_TYPE.'_create_table.sql';
		$handle = fopen ($filename, "r");
		$contents = fread($handle,filesize($filename));
		fclose ($handle);
		$sqls = split(';',$contents);
		foreach($sqls as $sql){
			if(strlen($sql) > 15){
				db_modify('ximple_create_table', $sql);
			}
		}
	}
	install_init_authority();
	install_init_js_var_user_insert();
}

function install_include(){
	include(INCLUDE_PATH.'tool/cache.tool.inc');
	include(INCLUDE_PATH.'tool/error.tool.inc');
	include(INCLUDE_PATH.'tool/io.tool.inc');
	include(INCLUDE_PATH.'tool/render.tool.inc');
	include(INCLUDE_PATH.'tool/config.tool.inc');
	include(INCLUDE_PATH.'tool/input.tool.inc');
	include(INCLUDE_PATH.'database/'.DB_TYPE.'.inc');
}

function install_init_authority(){
	include(INCLUDE_PATH.'tool/authority.tool.inc');
	authority();
}

function install_init_js_var_user_insert(){
	include(INCLUDE_PATH.'tool/head.tool.inc');
	head_add_js_var('pleaseFill', tt('Please, fill the required field : '));
	head_add_js_var('formNotCorrect', tt('Any field in the form is not correctly filled. Please correct it and try again.'));
	head_add_js_var('nameTooShort', tt('Login name is too short.'));
	head_add_js_var('nameNotConformed', tt('Login name is not conformed.'));
	head_add_js_var('nameExists', tt('Login name exists.'));
	head_add_js_var('emailExists', tt('Email address exists.'));
	head_add_js_var('passwdNotChange', tt('Password is empty, hence password is not changed.'));
	head_add_js_var('passwdTooShort', tt('Password is too short.'));
	head_add_js_var('passwdVeryStrong', tt('Password strength is very strong.'));
	head_add_js_var('passwdStrong', tt('Password strength is strong.'));
	head_add_js_var('passwdMedium', tt('Password strength is medium.'));
	head_add_js_var('passwdWeak', tt('Password strength is weak.'));
	head_add_js_var('passwdMismatch', tt('Password is mismathed.'));
	head_add_js_var('emailNotOk', tt('Your Email address is not conformed.'));
	head_add_js_var('insertUser', true);
	head_add_js_var('isInstalled', false);
}

function install_get_step_label(){
	return array(
		tt('License agreement'),
		tt('Setup validation'),
		tt('Permission validation'),
		tt('Root-Login information'),
		tt('Configuration'),
		tt('Happy Ending')
	);
}

function install_define_constant(){
	if(!defined('SECTION')) define('SECTION', 1);
	define('SECTION_URI', ROOT_URI);
	define('MODULE', 'install');
	define('Q', '?/');
	define('NOW', date('Y-m-d H:i:s'));
	define('TODAY', date('d F Y'));
}

function install_render_next_step($next_step, $next_enable, $try){
	ob_start();
	require('template/agree.tpl');
	$render = ob_get_contents();
	ob_end_clean();
	return $render;
}

function install_login(){
	$_SESSION['login_name'] = addslashes((string)pp('login_name'));
	$_SESSION['login_name_hash'] = sha1(pp('login_name'));
	$_SESSION['login_salt'] = sha1(rand());
	$_SESSION['login_password'] = sha1(pp('prepassword').ss('login_salt'));
}

function install_insert_user(){
	install_db_init();
	$user = db_select('users', array('id'));
	if(count($user) == 0){
		$insert_data = array(
			'login_name' => pp('login_name'),
			'login_name_hash' => sha1(pp('login_name')),
			'email' => pp('email'),
			'email_hash' => sha1(pp('email')),
			'password' => pp('prepassword'),
		);
		db_insert('users', $insert_data);
	}
}

/// Render form for minimal section configuration.
/// @return Form for minimal section configuration.
function install_section_config_page(){
	$preconf = install_preconfig();
	ob_start();
	require('template/sectionconfig.tpl');
	$render = ob_get_contents();
	ob_end_clean();
	return $render;
}

function install_db_init(){
	install_include();
	$GLOBALS['config'] = config_check_default(array());
	db_init();
}

/// Update user information (by resend user information)
function update_user(){
	// Insert user into database.
	if(count(db_array('SELECT id FROM users LIMIT 1')) == 0){
		$q = "UPDATE users SET login_name='%s', password='%s', email='%s' WHERE id=1";
		$q = sprintf($q, $_POST['login_name'], $_POST['password'], $_POST['email']);
		db_query($q);
	}
}


/// Get information array of system-preconfiguration.
/// @return Information array of system-preconfiguration.
function install_preconfig(){
	$preconfig = array(
		'blog' => array(
			'label' => tt('Bloging System'),
			'description' => tt('Minimal configuration with blog module.'),
		),
		/* In planning
		'personal' => array(
			'label' => tt('Personal Website'),
			'description' => tt('This pre-configuration allows you to create your personal website with blogging system and image gallery.'),
		),
		'community' => array(
			'label' => tt('Community Website'),
			'description' => tt('With community pre-configuration, your website initially contains forum, news, mailing-list and FAQ system.'),
		),
		'SME' => array(
			'label' => tt('SME Website'),
			'description' => tt('Website for Small and Medium Enterprise or other institution. This pre-configuration allows them to provide news and information about themselves.'),
		),
		'magazin' => array(
			'label' => tt('Online Magazin'),
			'description' => tt(''),
		),
		'portal' => array(
			'label' => tt('Web Portal'),
			'description' => tt(''),
		),
		'enterprise' => array(
			'label' => tt('Enterprise Website'),
			'description' => tt(''),
		),
		'shop' => array(
			'label' => tt('Online Shop'),
			'description' => tt(''),
		),
		'social' => array(
			'label' => tt('Social Network'),
			'description' => tt(''),
		),
		'knowledge' => array(
			'label' => tt('Knowledge Management System'),
			'description' => tt(''),
		),
		'wiki' => array(
			'label' => tt('Wiki-Based Knowledge Management'),
			'description' => tt(''),
		),
		'bookmark' => array(
			'label' => tt('Social Bookmarking'),
			'description' => tt(''),
		),
		*/
	);
	ksort($preconfig);
	return $preconfig;
}

function install_init_section(){
	install_db_init();
	$section_init = (count(db_select('section', array('id'))) == 0);
	if(strlen(pp('section_name').pp('section_slogan')) and $section_init){
		$insert_array = array('name' => pp('section_name'));
		db_insert('section', $insert_array);
		config_set('section_name', pp('section_name'));
		config_set('section_slogan', pp('section_slogan'));
		config_save();
	}
}

/// Render finishing message with minimal text-guideline.
function install_finish_message(){
	$m = tt('%sHome Page%s of your website can be found here.');
	$item  = '<li>'.sprintf($m, '<a href="'.ROOT_URI.'"><b>', '</b></a>').'</li>';
	$m = tt('Now you can configurate your system on %sSection-Configuration%s.');
	$item .= '<li>'.sprintf($m, '<a href="'.ROOT_URI.'?/config"><b>', '</b></a>').'</li>';
	$m = tt('If you want to extend your system with modules, you can go to %sLoad-Module%s to activate modules.');
	$item .= '<li>'.sprintf($m, '<a href="'.ROOT_URI.'?/module_manage"><b>', '</b></a>').'</li>';
	$m = tt('You can use %sTheme Manage%s to customize looks and feels of your site.').'</li>';
	$item .= '<li>'.sprintf($m, '<a href="'.ROOT_URI.'?/theme_select"><b>', '</b></a>');
	$remove   = tt('After this step, please remove directory "install" for security of your website.');
	$thank    = tt('Have fun, %sXimple Team%s');
	$thank    = sprintf($thank, '<a href="http://www.ximplesoft.com/">', '</a>');
	$message  = '<ul>'.$item.'</ul>';
	$message .= '<p style="color:red;">'.$remove.'</p>';
	$message .= '<p>'.$thank.'</p>';
	return $message;
}


function install_set_path(){
	define('FILES_PATH', ROOT_PATH.'files/');
	define('INCLUDE_PATH', ROOT_PATH.'include/');
	define('MODULE_PATH', ROOT_PATH.'module/');
	define('THEME_PATH', ROOT_PATH.'theme/');
}
?>
