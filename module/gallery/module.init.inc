<?php
function module_get_info_gallery(){
	$describe = tt('Gallery allows you to manage your fotos like another content type (tags, category, work-flow & co.). ');
	$describe.= tt('Thumnails of your fotos will be automatically generated.');
	return array(
		'name' => tt('Gallery'),
		'description' => $describe,
	);
}

function module_init_gallery(){
	include_once('Gallery.class.inc');
	include_once('GalleryPicture.class.inc');
	include_once('GalleryConfig.class.inc');
	
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$gallery = new Gallery();
	$gallery->initModule('Gallery.class.inc');
	
	$galleryPicture = new GalleryPicture();
	$galleryPicture->initModule('GalleryPicture.class.inc');
	
	$galleryConfig = new GalleryConfig();
	$galleryConfig->initModule('GalleryConfig.class.inc');
	
	$flash = new FlashWidgetManager('gallery_last_post');
	$flash->setMode('gallery');
	$flash->setName(tt('Last posted Galleries'));
	$flash->setTable('gallery');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2) ORDER BY publish_time DESC');
	$flash->submit();
	
	$flash = new FlashWidgetManager('gallery_last_commented');
	$flash->setMode('gallery');
	$flash->setName(tt('Last commented Galleries'));
	$flash->setTable('gallery');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2 AND comment_time > 0) ORDER BY comment_time DESC');
	$flash->submit();
	
	config_set('gallery_entries_number', 5);
}

function module_drop_gallery(){
	include_once('Gallery.class.inc');
	include_once('GalleryPicture.class.inc');
	include_once('GalleryConfig.class.inc');
	
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$gallery = new Gallery();
	$gallery->dropModule();
	
	$galleryPicture = new GalleryPicture();
	$galleryPicture->dropModule();
	
	$galleryConfig = new GalleryConfig();
	$galleryConfig->dropModule();
	
	notify_add_info(tt('Gallery has been successfully deactivated'));
	config_drop('gallery_entries_number');
}
?>
