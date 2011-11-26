<?php
authority_group('group_admin');
execute_add_function('group_role');
execute_add_function('group_role_save');

function group_role(){
	$gid = (int)gg('id');
	if($gid == 2){
		execute_set_db();
		document_set_redirect('group');
		notify_add_error(tt('The selected group is not allowed to be edited.'));
		return null;
	}
	if(empty($gid)) error_denied();
	return group_render($gid);
}

function group_role_save(){
	$gid = (int)pp('id');
	execute_set_db();
	document_set_redirect('group');
	group_update_map($gid);
	group_set_config($gid);
}

function group_render($gid){
	$name = db_select_single_column('groups', 'name', 'WHERE id='.$gid);
	$name = $name[$gid];
	$title = tt('Edit Group-Permission').' : '.$name;
	document_set_title($title);
	document_set_path(tt('User-Group'), 'group');
	document_set_path($title);
	head_add_css('list.css');
	head_add_css('form.css');
	head_add_css('role.css');
	head_add_js('include/javascript/form.js');
	head_add_js('include/javascript/autocomplete.column.js');
	$renderArray = array(
		'gid'  => $gid,
		'name' => $name,
		'role' => group_get_role($gid),
		'user' => group_get_user($gid),
	);
	return render($renderArray, 'role.tpl', 'role');
}

function group_update_map($gid){
	$map = group_get_map($gid);
	$drop = group_get_drop_array();
	$insert = group_get_users_insert_array();
	foreach($drop as $key => $value){
		if(!in_array($value, $insert) and in_array($value, $map)){
			db_drop_many('map', 'users='.$value.' AND section='.SECTION.' AND groups='.$gid);
		}
	}
	$insert_array = array('groups' => $gid, 'section' => SECTION);
	foreach($insert as $key => $value){
		if(!in_array($value, $map)){
			$insert_array['users'] = $value;
			db_insert('map', $insert_array);
		}
	}
}

function group_set_config($gid){
	$role = pp('role');
	group_set_role($gid, $role);
	group_drop_role($gid, $role);
	config_save();
}

function group_set_role($gid, $role){
	if(is_array($role)){
		foreach($role as $key => $value){
			$group = cc($value);
			if(is_array($group) and !in_array($gid, $group)){
				config_append($value, $gid);
			}else{
				config_set($value, array($gid));
			}
		}
	}
}

function group_drop_role($gid, $role){
	list($system_role, $module_role) = group_init_role();
	$section_role = array_merge($system_role, $module_role);
	foreach($section_role as $key => $value){
		if(is_array($role) and !in_array($key, $role)){
			config_drop_by_value($key, $gid);
		}
	}
}

function group_get_map($gid){
	$map_select = db_select('map', array('users'), 'WHERE groups='.$gid.' AND section='.SECTION);
	$map = array();
	foreach($map_select as $key=>$value){
		$map[] = $value['users'];
	}
	return $map;
}


function group_get_drop_array(){
	$drop = array();
	$splitted = split(',', pp('drop_user'));
	foreach($splitted as $key=>$value){
		$uid = (int)$value;
		if(!empty($uid)) $drop[] = $uid;
	}
	return $drop;
}

function group_get_users_insert_array(){
	$splitted = split(',', addslashes(pp('insert_user')));
	$name_list = array();
	foreach($splitted as $name){
		if(substr($name, 0, 1) == ' ') $name = substr($name, 1);
		if(substr($name, -1) == ' ') $name = substr($name, 0, -1);
		$name_list[] = $name;
	}
	if(count($name_list)){
		$clause = "WHERE login_name='".join("' OR login_name='", $name_list)."'";
		$insert = db_select_single_column('users', 'id', $clause);
	}else{
		$insert = array();
	}
	return $insert;
}

function group_get_role($gid){
	list($system_role, $module_role) = group_init_role();
	$role = array_merge($system_role, $module_role);
	foreach($role as $key => $value){
		$role[$key]['is_system'] = isset($system_role[$key]);
		$role[$key]['is_role'] = (is_array(cc($key)) and in_array($gid, cc($key)));
	}
	return $role;
}

function group_get_user($gid){
	$map = db_select('map', array('users'), 'WHERE groups='.$gid.' AND section='.SECTION);
	$user_info = userinfo_get_info($map, 'users');
	$user = array();
	foreach($user_info as $key=>$value){
		$user[$value['users']['id']] = $value['users'];
	}
	return $user;
}

function group_init_role(){
	$system_role = config_default_role();
	$module_role = io_unserialize(FILES_PATH.'config/role'.SECTION);
	ksort($module_role);
	return array($system_role, $module_role);
}

?>
