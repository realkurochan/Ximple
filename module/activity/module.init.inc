<?php
function module_get_info_activity(){
	$describe = tt('Calendar of Activities in this the Section.');
	return array(
		'name' => tt('Activity'),
		'description' => $describe,
	);
}

function module_init_activity(){
	include_once('Activity.class.inc');
	include_once('ActivityConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$activity = new Activity();
	$activity->initModule('Activity.class.inc');
	
	$activityConfig = new ActivityConfig();
	$activityConfig->initModule('ActivityConfig.class.inc');
	
	$flash = new FlashWidgetManager('activity_last_post');
	$flash->setMode('activity');
	$flash->setName(tt('Last posted Activities'));
	$flash->setTable('activity');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2) ORDER BY publish_time DESC');
	$flash->submit();
	
	$flash = new FlashWidgetManager('activity_last_commented');
	$flash->setMode('activity');
	$flash->setName(tt('Last commented Activities'));
	$flash->setTable('activity');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2 AND comment_time > 0) ORDER BY comment_time DESC');
	$flash->submit();
	
	config_set('activity_icon_size', 64);
	config_set('activity_entries_number', 5);
}

function module_drop_activity(){
	include_once('Activity.class.inc');
	include_once('ActivityConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$activity = new Activity();
	$activity->dropModule();
	
	$activityConfig = new ActivityConfig();
	$activityConfig->dropModule();
			
	notify_add_info(tt('Activity has been successfully deactivated'));
	config_drop('activity_entries_number');
}
?>
