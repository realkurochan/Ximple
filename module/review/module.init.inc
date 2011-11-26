<?php
function module_get_info_review(){
	return array(
		'name' => tt('Review'),
		'description' => tt('Comment writing and reading with rating and opinion point.')
	);
}

function module_init_review(){
	include_once('Review.class.inc');
	include_once('review.tool.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	
	$review = new Review();
	$review->initModule('Review.class.inc');
	
	review_rating_create_table();
	
	module_add_role('review_admin', 'Review Administrator', 'Who can configure, drop Review');
	module_add_role('review_reviewer', 'Review Reviewer', 'Who can write, rate the review and give opinion point.');
	
	module_add_panel('Review', 'review_admin', 'review_list', 'manage all reviews');

	module_add_panel('Review', 'review_reviewer', 'review_reviewee_list', 'list all reviews for selected user');
	module_add_panel('Review', 'review_reviewer', 'review_reviewer_list', 'list all reviews create by user');

	extension_add('additional_profile_link', 'review_add_link');
	notify_add_info(tt('Review has been successfully activated'));
}

function module_drop_review(){
	include_once('Review.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	$review = new Review();
	$review->dropModule();
	module_drop_role('review_admin');
	module_drop_role('review_reviewer');
	module_drop_panel('Review');
	extension_drop('additional_profile_link', 'review_add_link');
	notify_add_info(tt('Review has been successfully deactivated'));
}
?>
