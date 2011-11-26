<?php
function module_get_info_like(){
	return array(
		'name' => tt('Like'),
		'description' => tt('Let users in your site click like for comments and contents.'),
	);
}

function module_init_like(){
	require_once('Like.class.inc');
	$like = new Like();
	$like->initModule('Like.class.inc');
	
	extension_add('comment_operation', 'like_embed');
	extension_add('content_operation', 'like_embed');
	
	notify_add_subscribe('like', tt('Some body like my comments or contributions.'));
}

function module_drop_like(){
	require_once('Like.class.inc');
	$like = new Like();
	$like->dropModule();
	
	extension_drop('comment_operation', 'like_embed');
	extension_drop('content_operation', 'like_embed');
	
	notify_drop_subscribe('like');
}
?>
