<?php
/// @file
/// This file provides functions for configuration management in XimpleCMS.

/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Configuration concept in XimpleCMS
/// XimpleCMS uses serialization to store configuration values in files.
/// The saved files are in files/config.
/// Due to the serialization and KISS concept in XimpleCMS,
/// you should not store many configuration values in the system configuration.
/// For modules with heavy weight configuration (NOT RECOMMENDED),
/// it should have own configuration system.

/// Temporary change of configuration value.
/// @param $key : reference key of configuration.
/// @param $value : value of configuration.
function config_change($key, $value){
	$GLOBALS['tmp_config'][$key] = $value;
}

/// Permanent change of configuration value (use with configsave).
/// @param $key : reference key of configuration.
/// @param $value : value of configuration.
function config_set($key, $value){
	$GLOBALS['config'][$key] = $value;
}

/// Append a configuration value in to the configuration, which is an array.
/// @param $key : reference key of configuration.
/// @param $value : value of configuration.
function config_append($key, $value){
	$GLOBALS['config'][$key][] = $value;
}

/// Append a configuration value in to the configuration, which is an associative array.
/// @param $name : reference key of configuration.
/// @param $key : associative key of configuration.
/// @param $value : value of configuration.
function config_append_assc($name, $key, $value){
	$GLOBALS['config'][$name][$key] = $value;
}

/// Drop a configuration value.
/// @param $key : reference key of configuration.
function config_drop($key){
	unset($GLOBALS['config'][$key]);
}

/// Check existence of a configuration value.
/// @param $name : reference key of configuration.
/// @param $value : value of configuration.
function config_has_value($name, $value){
	$config = cc($name);
	if(!is_array($config)) $config = array();
	return in_array($value, $config);
}


/// Drop a configuration value, which have the same value as the given value.
/// @param $key : reference key of configuration.
/// @param $value : value of configuration.
function config_drop_by_value($key, $value){
	if(isset($GLOBALS['config'][$key]) and is_array($GLOBALS['config'][$key])){
		$conf = $GLOBALS['config'][$key];
		foreach($conf as $index => $set_value){	
			if($set_value == $value){
				unset($GLOBALS['config'][$key][$index]);
			}
		}
	}
}

/// Drop a configuration value contained in an associative array.
/// @param $name : reference key of configuration.
/// @param $key : associative key of configuration.
function config_drop_assc($name, $key){
	unset($GLOBALS['config'][$name][$key]);
}

/// Save entire configuration to file.
function config_save(){
	$config_dir = FILES_PATH.'config/';
	if(!is_dir($config_dir)) mkdir($config_dir);
	io_serialize($GLOBALS['config'], $config_dir.'config'.SECTION);
	cache_clear_section();
	if(IS_APC){
		apc_clear_cache();
		apc_clear_cache('user');
	}
}

/// Default module requirement of XimpleCMS 
/// Modules in this array are required module for XimpleCMS.
/// @return Array of default module requirement.
function config_default_require(){
	return array(
		'widget' => 'Widget.class.inc',
		'menu' => 'Menu.class.inc',
		'module' => 'module.module.inc',
		'file' => 'file.module.inc',
		'shortnote' => 'ShortNote.class.inc',
		'message' => 'Message.class.inc',
		'captcha' => 'captcha.module.inc',
		'about' => 'About.class.inc',
		'config' => 'SectionConfig.class.inc',
		'theme' => 'theme.module.inc',
		'user' => 'User.class.inc',
		'group' => 'Group.class.inc',
		'profiletype' => 'ProfileType.class.inc',
		'profile' => 'Profile.class.inc',
		'registration' => 'registration.module.inc',
		'account' => 'Account.class.inc',
		'section' => 'Section.class.inc',
		'syslog' => 'syslog.module.inc',
		'category' => 'Category.class.inc',
		'comment' => 'Comment.class.inc',
		'freetag' => 'freetag.module.inc',
		'tracker' => 'tracker.module.inc',
		'flashwidget' => 'flashwidget.module.inc',
		'imageloader' => 'imageloader.module.inc',
		'search' => 'search.module.inc',
		'login' => 'login.module.inc',
		'bench' => 'bench.module.inc',
		'extension' => 'extension.module.inc',
		'forgetpassword' => array(
			'#primary' => 'ForgetPassword.class.inc',
			'reset' => 'ResetPassword.class.inc',
		),
		'locale' => array(
			'#primary' => 'Locale.class.inc',
			'module' => 'LocaleModule.class.inc',
		),
	);
}

/// Default module name of XimpleCMS.
/// @return Array of default module.
function config_default_module_name(){
	return array(
		'about' => 'About Us',
	);
}


/// Get default panel array.
/// @return Default panel array.
function config_default_panel(){
	return array(
		'::: My Account :::' => array(
			'my files' => 'file_manage_user',
			'my message' => 'message',
			'my profile' => 'profile',
			'my short note' => 'shortnote',
			'my tracker' => 'tracker',
			'configure account' => 'account'
		),
		'Comments' => array( 
			'my comments' => 'comment_list',
			'on my contribution' => 'comment_contributor_list',
			'in section' => 'comment_section_list',
		)
	);
}

/// Get array of default panel access role.
/// @return Array of default panel access role.
function config_default_panel_access(){
	$panel = array(
		'Widget & Menu' => array(  
			'widget_admin' => array(  
				'widget' => 'widget',
			),
			'menu_admin' => array(  
				'menu' => 'menu'
			),
		),
		'Users & Groups' => array(  
			'user_admin' => array(  
				'user' => 'user',
				'registration' => 'registration_config',
			),
			'group_admin' => array(  
				'group & role' => 'group'
			),
			'profile_admin' => array(  
				'profile type' => 'profiletype'
			)
		),
		'System Admin' => array(  
			'file_admin' => array(  
				'manage file' => 'file_manage_admin'
			),
			'module_admin' => array(  
				'manage module' => 'module_manage',
				'manage extension' => 'extension'
			),
			'config_admin' => array(  
				'configure section' => 'config/'.SECTION,
			),
			'syslog_admin' => array(  
				'system log' => 'syslog_render'
			),
		),
		'Themes' => array(  
			'theme_admin' => array(
				'select theme' => 'theme_select',
				'configure theme' => 'theme_config',
				'custom css' => 'theme_css',
				'custom image' => 'theme_image'
			)
		),
	);
	if(SECTION == 1){
		$panel['System Admin']['section_admin']['manage section'] = 'section_manage';
	}
	if(SECTION == 1 and state_is_develop()){
		$panel['Locale']['locale_admin'] = array(
			'get locale array' => 'locale',
			'initial locale' => 'locale_module',
		);
	}
	return $panel;
}

/// Get array of default access description.
/// @return array of default access description.
function config_default_role(){
	$default = array(
		'user_admin' => array(
			'name' => 'User Manager',
			'description' => 'Who can manage users in Section.'),
		'comment_admin' => array(
			'name' => 'Comment Manager',
			'description' => 'Who can delete and edit all comments in Section.'),
		'group_admin' => array(  
			'name' => 'Group Manager',
			'description' => 'Who can manage groups and roles in Section.'),
		'file_admin' => array(  
			'name' => 'File Manager',
			'description' => 'Who can manage System-Files in Section'),
		'profile_admin' => array(  
			'name' => 'Profiler',
			'description' => 'Who can manager Profile-Type and User-Profile of all users in Section'),
		'module_admin' => array(  
			'name' => 'Module Loader',
			'description' => 'Who can manage modules in Section.'),
		'theme_admin' => array(  
			'name' => 'Designer',
			'description' => 'Who can select and configure theme in Section.'),
		'config_admin' => array(  
			'name' => 'Configurator',
			'description' => 'Who can configure important parameter of Section.'),
		'section_admin' => array(  
			'name' => 'Section Manager',
			'description' => 'Who can add and delete Sections.'),
		'widget_admin' => array(  
			'name' => 'Widget Manager',
			'description' => 'Who can manage Widgets in Section.'),
		'menu_admin' => array(  
			'name' => 'Menu Manager',
			'description' => 'Who can manage Menus in Section.'),
		'locale_admin' => array(  
			'name' => 'Locale Manager',
			'description' => 'Who can use tools for text translation (Section must be set to development state).'),
		'syslog_admin' => array(  
			'name' => 'System Watcher',
			'description' => 'Who can access Syslog-management in Section.')
	);
	return $default;
}

/// Default configuration of XimpleCMS.
/// @return Array of default configuration.
function config_default(){
	return array(  
		'theme' => 'bluesky',
		'icon' => 'files/misc/ximple.png',
		'section_name' => 'My Website',
		'section_slogan' => '',
		'section_path' => '',
		'section_description' => '',
		'lang' => 'en',
		'home_label' => 'Home',
		'default_user_logo' => 'files/userlogo/default.png',
		'notify_interval' => 30,
		'index_mode' => 'about',
		'start' => '2006-09-19',
		'meta_data' => '',
		'allowed_tags' => '<a><p><img><strong><h3><h4><h5><obj><param><embed><span><em><blockquote><table><tbody><tr><td><ul><ol><li><br><pre><b><u><i><div><s><sub><sup><hr><font><span><style>',
		'cachable_mode' => array( ),
		'cache_mode' => 'txt',
		'cache_time' => '18000',
		'increase_read' => array( ),
		'feed_mode' => array( ),
		'feed_available_mode' => array( ),
		'feed_number' => '10',
		'feed_comment' => '',
		'feed_type' => array (
			0 => 'atom',
			1 => 'rss',
		),
		'feed_number' => '10',
		'feed_comment' => 1,
		'feed_comment_explicit' => 0,
		'antispam_salt' => (string) md5(rand()),
		'antispam_time' => 21600,
		'widget_position' => array(
			'left' => 'Left Sidebar',
			'foot' => 'Foot',
			'disabled' => 'Disabled',
		),
		'widget_primary_position' => 'left',
		'menu_include_widget' => true,
		'navigation' => array(
			'home' => 'Home',
			'about' => 'About',
		),
		'navigation_enabled' => array('home', 'about'),
		'module_require' => array(),
		'module_name' => array(),
		'syslog_enable' => true,
		'syslog_time' => 604800,
		'tracker_enable' => true,
		'tracker_time' => 604800,
		'time_zone' => 'UTC',
		'time_format' => 'd F Y',
		'supported_language' => array(),
	);
}

/// Check required values of given configuration.
/// If the given configuration does not have the required values,
/// the default values will be set.
/// @param $config : Given configuration.
/// @return Configuration with required values.
function config_check_default($config){
	if(IS_APC and $cache = apc_fetch('config_'.SECTION)) return $cache;
	$default = config_default();
	foreach($default as $key=>$value){
		if(!isset($config[$key])){
			$config[$key] = $value;
		}
	}
	$config['module_require'] = array_merge($config['module_require'], config_default_require());
	$config['module_name'] = array_merge($config['module_name'], config_default_module_name());
	if(!(is_dir(THEME_PATH.$config['theme']) or is_dir(SECTION_PATH.'private/theme/'.$config['theme']))){
		$config['theme'] = 'default';
	}
	if(IS_APC) apc_add('config_'.SECTION, $config);
	return $config;
}

/// Translate configuration.
/// @return Translated configuration.
function config_translate(){
	$GLOBALS['untranslated_config'] = $GLOBALS['config'];
	if(IS_APC and $cache = apc_fetch('config_translat_'.SECTION)) return $cache;
	$GLOBALS['config'] = translate_content($GLOBALS['config'], 'config', SECTION);
	if(IS_APC) apc_add('config_translat_'.SECTION, $GLOBALS['config']);
}
?>
