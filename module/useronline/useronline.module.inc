<?php
execute_add_function('useronline');
execute_add_function('useronline_widget_cont');
execute_add_function('useronline_widget_user_list');
execute_add_function('useronline_set_access');
execute_add_function('useronline_profile_test');

function useronline(){
	document_set_path(tt('Who is online'));
	$user_info = useronline_get_array_info();
	$render_array = array('user' => $user_info);
	return render($render_array, 'useronline.tpl', 'useronline');
}


function useronline_widget_cont(){
	$count = count(useronline_check_user());
	$content = sprintf(tt('%d people are online.'), $count);
	$content = '<a href="'.SECTION_URI.'useronline">'.$content.'</a>';
	return $content;
}

function useronline_widget_user_list(){
	$user_info = useronline_get_array_info();
	$render_array = array('user' => $user_info);
	return render($render_array, 'useronline_widget.tpl', 'useronline_widget');
}

function useronline_set_access(){
	if(USER_ID > 0) count_set('useronline', USER_ID, time());
}

function useronline_get_array_info(){
	$check = useronline_check_user();
	$info = array();
	foreach($check as $key => $id){
		$info[] = userinfo_get_info_by_id($id);
	}
	return $info;
}

function useronline_check_user(){
	$access_time = count_get_all('useronline');
	$now = time() - 300;
	$online = array();
	foreach($access_time as $key => $value){
		if($value > $now) $online[] = $key;
	}
	return $online;
}

?>
