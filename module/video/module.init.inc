<?php
function module_get_info_video(){
	$describe = tt('With Video Module, you can provide video streaming on your website. ');
	$describe.= tt('Be careful with the bandwidth. Vedio streaming is very bandwidth hungry.');
	return array(
		'name' => tt('Video'),
		'description' => $describe,
	);
}

function module_init_video(){
	include_once('Video.class.inc');
	include_once('VideoConfig.class.inc');
	
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$video = new Video();
	$video->initModule('Video.class.inc');
	
	$videoConfig = new VideoConfig();
	$videoConfig->initModule('VideoConfig.class.inc');
	
	$flash = new FlashWidgetManager('video_last_post');
	$flash->setMode('video');
	$flash->setName(tt('Last posted Videos'));
	$flash->setTable('video');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2) ORDER BY publish_time DESC');
	$flash->submit();
	
	$flash = new FlashWidgetManager('video_last_commented');
	$flash->setMode('video');
	$flash->setName(tt('Last commented Videos'));
	$flash->setTable('video');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2 AND comment_time > 0) ORDER BY comment_time DESC');
	$flash->submit();
	
	config_set('video_entries_number', 5);
	config_set('video_width', 480);
	config_set('video_height', 390);
	config_set('video_icon_size', 64);
	$video_directoy = FILES_PATH.'video';
	if(!is_dir($video_directoy)) mkdir($video_directoy);
}

function module_drop_video(){
	include_once('Video.class.inc');
	include_once('VideoConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$video = new Video();
	$video->dropModule();
	$videoConfig = new VideoConfig();
	$videoConfig->dropModule();
	
	notify_add_info(tt('Video has been successfully deactivated'));
	config_drop('video_entries_number');
	config_drop('video_width');
	config_drop('video_width');
}
?>
