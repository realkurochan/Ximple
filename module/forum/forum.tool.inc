<?php
function forum_guest_postable($forum_id){
	$forum = forum_get_info($forum_id);
	return $forum['guest_postable'];
}

function forum_check_authority_group(){
	if(authority_group('forum_entry_admin')) return true;
	if(authority_group('forum_entry_editor')) return true;
	return false;
}

function forum_check_readable($forum_id){
	$allowed_forums = forum_get_readable_id();
	return in_array($forum_id, $allowed_forums);
}

function forum_get_last_entry($forum_id){
	$clause = 'WHERE forum='.$forum_id.' AND publishing_status < 2 ORDER BY comment_time DESC LIMIT 1';
	$column = array('id', 'topic', 'writer', 'writer_name', 'write_time');
	$forum_entry = db_select('forum_entry', $column, $clause);
	if(count($forum_entry)){
		$forum_entry = array_values($forum_entry);
		$forum_entry[0]['writer'] = userinfo_get_info_by_id($forum_entry[0]['writer']);
		return $forum_entry[0];
	}else{
		return array();
	}
}

function forum_get_readable_id(){
	return forum_get_allowed_id('readable_group');
}

function forum_get_writable_id(){
	return forum_get_allowed_id('writable_group');
}

function forum_get_publishable_id(){
	return forum_get_allowed_id('publishable_group');
}

function forum_get_allowed_id($permission_column){
	$forum_permission = db_select_single_column('forum', $permission_column, 'WHERE section='.SECTION);
	$allowed_id = array();
	foreach($forum_permission as $key => $value){
		$group = unserialize($value);
		$enable = authority(array(), $group);
		$enable = ($enable or authority_group('forum_entry_admin'));
		$enable = ($enable or authority_group('forum_entry_editor'));
		if($enable){
			$allowed_id[] = $key;
		}
	}
	return $allowed_id;
}

function forum_get_info($forum_id){
	$clause = 'WHERE id='.((int) $forum_id);
	$column = array('category', 'name', 'description');
	$forum = db_select('forum', $column, $clause);
	return $forum[0];
}

function forum_get_category_id($forum_id){
	$forum = forum_get_info($forum_id);
	return $forum['category'];
}

function forum_set_path($forum_id){
	$forum = forum_get_info($forum_id);
	$category_id = $forum['category'];
	$clause = 'WHERE id='.$category_id;
	$category = db_select('category', array('category'), $clause);
	document_set_path($category[0]['category'], 'forum/category/'.$category_id);
	document_set_path($forum['name'], 'forum_entry_list/forum/'.$forum_id);
}

function forum_render_last_entry($data){
	if(count($data)){
		$render = '<p><label>'.tt('last entry').' : </label>';
		$render.= '<a href="'.SECTION_URI.Q.'forum_entry/'.$data['id'].'">'.$data['topic'].'</a> ';
		if($data['writer'] != -1){
			$render.= tt('by').' '.render_profile_link($data['writer']).' ';
		}else{
			$render.= tt('by').' '.$data['writer_name'].' ';
		}
		$render.= '@ '.date(cc('time_format'), strtotime($data['write_time']))	.'</p>';
		return $render;
	}
}

function forum_show_level(){
	return config_has_value('forum_appearance', 'level');
}

function forum_get_publishing_status(){
	$publishing_status = content_get_publishing_status();
	$publishing_status[4] = tt('private');
	return $publishing_status;
}

function forum_render_level($user_id){
	if($user_id != -1 and forum_show_level()){
		$contribution = count_get('contribution', $user_id);
		$level = cc('forum_user_level');
		$i = 1;
		$userLevel = 0;
		$userLevelName = tt('undefined level');
		foreach($level as $key => $value){
			$userLevel = $i;
			$userLevelName = $value;
			if($contribution < $key) break;
			$i++;
		}
		$star = '';
		for($i=0;$i < $userLevel;$i++){
			$star .= '<img src="'.ROOT_URI.'files/icon/star.png" alt="star" style="padding:0;margin:0;"/>';
		}
		$render  = '<p style="text-align:center;">'.$star.'</p>';
		$render .= '<p style="text-align:center;">'.$userLevelName.' ['.$contribution.']'.'</p>';
		return $render;
	}
}

?>
