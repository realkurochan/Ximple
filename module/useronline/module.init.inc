<?php
function module_get_info_useronline(){
	return array(
		'name' => tt('UserOnline'),
		'description' => tt('Show user status, who is now online.')
	);
}

function module_init_useronline(){
	require_once(INCLUDE_PATH.'tool/widget.tool.inc');
	module_add('useronline', 'useronline.module.inc', 'UserOnline');
	widget_add(tt('Who is online (count)'), 'useronline_widget_cont');
	widget_add(tt('Who is online (list)'), 'useronline_widget_user_list');
	extension_add('main_init', 'useronline_set_access');
}

function module_drop_useronline(){
	require_once(INCLUDE_PATH.'tool/widget.tool.inc');
	module_drop('useronline');
	widget_drop('useronline_widget_count');
	widget_drop('useronline_widget_user_list');
	extension_drop('main_init', 'useronline_set_access');
}
?>
