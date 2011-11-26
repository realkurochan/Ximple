<?php
if(!authority_group('theme_admin')) error_denied();

execute_add_function('theme_select');
execute_add_function('theme_save');
execute_add_function('theme_reset');
execute_add_function('theme_clear_custom');
execute_add_function('theme_image');
execute_add_function('theme_css');
execute_add_function('theme_edit_css');
execute_add_function('theme_save_css');
execute_add_function('theme_config');
execute_add_function('theme_config_save');

define('THEME_PAGE_LENGTH', 6);
define('THEME_ROW_LENGTH', 2);
include_once(INCLUDE_PATH.'class/PagerRender.class.inc');

function theme_select(){
	$title = tt('Manage Theme');
	document_set_path($title);
	document_set_title($title);
	$theme = theme_get_information();
	$render = render(array('data' => $theme), 'theme.tpl', 'theme');
	$pager = new PagerRender(THEME_PAGE_LENGTH, count($theme));
	$render.= $pager->render('theme_select');
	head_add_css('theme.css');
	return $render;
}

function theme_save(){
	$all_themes = theme_get_information();
	theme_drop_additional_config($all_themes[cc('theme')]['additional_config']);
	$theme_name = gg('theme');
	theme_set_config($theme_name);
	execute_set_db();
	document_set_redirect('theme_select/page/'.pid().'#selected_theme');
	notify_add_info(tt('The selected theme has been successfully initialized.'));
}

function theme_reset(){
	$theme_name = cc('theme');
	theme_set_config($theme_name);
	execute_set_db();
	document_set_redirect('theme_select/page/'.pid().'#selected_theme');
	notify_add_info(tt('Your theme has been successfully reset.'));
}

function theme_clear_custom(){
	io_clear_directory(FILES_PATH.'customtheme/'.SECTION.'/');
	execute_set_db();
	document_set_redirect('theme_select/page/'.pid().'#selected_theme');
	notify_add_info(tt('Your custom theme directories have been successfully cleared.'));
	if(IS_APC){
		apc_clear_cache();
		apc_clear_cache('user');
	}
}

function theme_image(){
	if(theme_check_custom_directory('image')){
		include_once(MODULE_PATH.'file/file_manage.module.inc');
		include_once(MODULE_PATH.'file/file.tool.inc');
		$dir = gg('dir');
		if(!$dir) $dir = '';
		$root_path = file_get_mode_path('theme_image');
		$title = tt('Theme custom image');
		document_set_title($title);
		document_set_path(tt('Theme'), 'theme');
		document_set_path($title);
		return file_manage($root_path, $dir, 'theme_image', $title);
	}
}

function theme_css(){
	$css_path = theme_check_custom_directory('css');
	if($css_path){
		$css_list = io_get_directory_entry($css_path);
		sort($css_list);
		$title = tt('Custom CSS');
		document_set_path(tt('Theme'), 'theme');
		document_set_path($title);
		document_set_title($title);
		return render(array('data' => $css_list), 'theme_css.tpl', 'theme_css');
	}
}

function theme_edit_css(){
	$css_path = theme_check_custom_directory('css');
	if($css_path){
		$file_name = str_replace('_', '.', gg('file'));
		$css = io_read_file($css_path.'/'.$file_name);
		$title = tt('Edit Custom CSS : '.$file_name);
		document_set_path(tt('Theme'), 'theme');
		document_set_path(tt('Custom CSS'), 'theme_css');
		document_set_path($title);
		document_set_title($title);
		return render(array('data' => $css, 'file_name' => $file_name), 'theme_css_edit.tpl', 'theme_css_edit');
	}
}

function theme_save_css(){
	$css_path = theme_check_custom_directory('css');
	if($css_path){
		io_write_file($css_path.'/'.pp('file'), pp('value'));
		execute_set_db();
		document_set_redirect('theme_css');
		notify_add_info(tt('Your CSS file has been successfully saved.'));
	}
}

function theme_config(){
	if(theme_check_custom_directory('config')){
		document_set_path(tt('Theme'), 'theme');
		return theme_execute_external('theme_execute_config_form');
	}
}

function theme_config_save(){
	if(theme_check_custom_directory('config')){
		execute_set_db();
		document_set_redirect('theme_config');
		theme_execute_external('theme_execute_config_save');
	}
}

function theme_execute_external($function){
	if(is_dir(THEME_PATH.cc('theme').'/')) $theme_dir = THEME_PATH.cc('theme').'/';
	else $theme_dir = SECTION_PATH.'private/theme/'.cc('theme').'/';
	
	include_once('theme.config.inc');
	if(function_exists($function)){
		return $function();
	}else{
		notify_add_error(tt('The selected theme can not be configured.'));
		execute_set_db();
		document_set_redirect('theme_select');
	}
}

function theme_set_additional_config($config){
	foreach($config as $key => $value){
		config_set($key, $value);
	}
}

function theme_drop_additional_config($config){
	foreach($config as $key => $value){
		config_drop($key, $value);
	}
}

function theme_check_custom_directory($type){
	$custom_path = FILES_PATH.'customtheme/'.SECTION.'/'.cc('theme').'/';
	if(!is_dir($custom_path)){
		theme_set_config(cc('theme'));
		notify_add_info(tt('Your custom theme directory does not exist. The directory has been created.'));
		execute_set_db();
		document_set_redirect('theme_'.$type);
	}
	return $custom_path.$type;
}

function theme_get_information(){
	list($theme, $init_file) = theme_get_init_file(THEME_PATH);
	
	$private_path = SECTION_PATH.'private/theme/';
	if(is_dir($private_path)){
		list($theme_private, $init_file_private) = theme_get_init_file($private_path);
		$theme = array_merge($theme_private, $theme);
		$init_file = array_merge($init_file_private, $init_file);
	}
	
	theme_include_init_file($init_file);
	$theme = theme_check_function($theme);
	$theme = theme_check_status($theme);
	$theme = theme_get_description($theme);
	return $theme;
}

function theme_get_init_file($dir_name){
	$init_array = array();
	$theme_array = array();
	$dir = dir($dir_name);
	while (false !== ($entry = $dir->read())) {
		if($entry != '.' and $entry != '..'){
			$init_file = $dir_name.$entry.'/theme.init.inc';
			$config_file = $dir_name.$entry.'/theme.config.inc';
			$boot_file = $dir_name.$entry.'/theme.boot.inc';
			if(is_file($init_file) and is_file($config_file) and is_file($boot_file)){
				$init_array[] = $init_file;
				$theme_array[] = $entry;
			}
		}
	}
	return array($theme_array, $init_array);
}

function theme_include_init_file($init_file){
	foreach($init_file as $file){
		if(state_is_develop()){
			include_once($file);
		}else{
			@include_once($file);
		}
	}
}

function theme_check_function($theme){
	$theme_out = array();
	foreach($theme as $name){
		if(function_exists('theme_get_info_'.$name)){
			$theme_out[] = $name;
		}
	}
	return $theme_out;
}

function theme_check_status($theme){
	$theme_out = array();
	$selected_theme = cc('theme');
	foreach($theme as $name){
		$theme_out[$name]['current'] = $name == $selected_theme;
	}
	return $theme_out;
}

function theme_get_description($theme){
	foreach($theme as $name => $value){
		$function_name = 'theme_get_info_'.$name;
		$theme_info = $function_name();
		if(theme_check_info($theme_info)){
			$theme[$name] = array_merge($theme[$name], $theme_info);
			$preview = 'theme/'.$name.'/'.$theme[$name]['preview'];
			if(is_file(ROOT_PATH.$preview)) $theme[$name]['preview'] = ROOT_URI.$preview;
			else $theme[$name]['preview'] = SECTION_URI.'private/'.$preview;
		}else{
			unset($theme[$name]);
			if(state_is_develop()){
				notify_add_error('Information of theme : '.$name.' is not complete.');
			}
		}
	}
	return $theme;
}

function theme_check_info(&$theme_info){
	if(!isset($theme_info['additional_config']) or !is_array($theme_info['additional_config'])){
		$theme_info['additional_config'] = array();
	}
	$info = array('name', 'description', 'widget_position', 'widget_primary_position', 'menu_include_widget', 'preview');
	$complete = count(array_intersect($info, array_keys($theme_info))) == count($info);
	$complete = ($complete and isset($theme_info['widget_position']));
	$complete = ($complete and count($theme_info['widget_position']));
	$check_primary_widget = array_key_exists($theme_info['widget_primary_position'], $theme_info['widget_position']);
	$complete = ($complete and $check_primary_widget);
	if(!$check_primary_widget and state_is_develop()){
		notify_add_error(tt('The given primary widget position does not exist.'));
	}
	return $complete;
}

function theme_copy_custom($theme){
	$custom_dir = FILES_PATH.'customtheme/';
	if(!is_dir($custom_dir)) mkdir($custom_dir);
	$custom_dir .= SECTION.'/';
	if(!is_dir($custom_dir)) mkdir($custom_dir);
	$custom_dir .= $theme.'/';
	if(!is_dir($custom_dir)) mkdir($custom_dir);
	if(!is_dir($custom_dir.'css')) mkdir($custom_dir.'css');
	if(!is_dir($custom_dir.'image')) mkdir($custom_dir.'image');
	$source = THEME_PATH.'default/';
	io_copy_directory($source.'css', $custom_dir.'css');
	
	if(is_dir(THEME_PATH.$theme.'/')) $source = THEME_PATH.$theme.'/';
	else $source = SECTION_PATH.'private/theme/'.$theme.'/';
	
	io_copy_directory($source.'css', $custom_dir.'css');
	io_copy_directory($source.'image', $custom_dir.'image');
}

function theme_set_config($theme_name){
	$all_themes = theme_get_information();
	$theme_config = $all_themes[$theme_name];
	config_set('theme', $theme_name);
	$position = array_merge($theme_config['widget_position'], array('disabled' => 'Disabled'));
	config_set('widget_position', $position);
	config_set('widget_primary_position', $theme_config['widget_primary_position']);
	config_set('menu_include_widget', $theme_config['menu_include_widget']);
	$custom_img = FILES_PATH.'customtheme/'.SECTION.'/'.$theme_name.'/image/';
	config_append_assc('file_mode_path', 'theme_image', $custom_img);
	theme_set_additional_config($theme_config['additional_config']);
	theme_copy_custom($theme_name);
	config_save();
	theme_reset_widget_position($theme_config['widget_primary_position']);
}

function theme_reset_widget_position($primary_position){
	require_once(INCLUDE_PATH.'tool/syswidget.tool.inc');
	db_update_many('widget', array('position' => 'disabled'), 'section='.SECTION);
	$list = syswidget_get_list();
	$list = $list['primary'];
	$clause = "(call_mode='".join("' OR call_mode='", $list)."')";
	$clause.= 'AND section='.SECTION;
	db_update_many('widget', array('position' => $primary_position), $clause);
}

?>
