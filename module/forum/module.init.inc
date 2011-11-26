<?php
function module_get_info_forum(){
	$describe = tt('Forum module for Ximple. You can use forum module for online discussion in your community.');
	return array(
		'name' => tt('Forum'),
		'description' => $describe,
	);
}

function module_init_forum(){
	include_once('Forum.class.inc');
	include_once('ForumEntry.class.inc');
	include_once('ForumConfig.class.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$forumEntry = new ForumEntry();
	$forumEntry->initModule('ForumEntry.class.inc');
	
	$forum = new Forum();
	$forum->initModule('Forum.class.inc');
	
	$forumConfig = new ForumConfig();
	$forumConfig->initModule('ForumConfig.class.inc');
	
	
	list($admin, $editor, $contributor) = content_add_role('forum_entry', tt('Forum Entry'));
	
	$panel = new PanelManager('Forum Entry');
	$panel->drop();
	
	$panel = new PanelManager('Forum');
	$panel->drop();
	$panel->appendRole($admin);
	$panel->appendRole($editor);
	$panel->appendRole($contributor);
	
	$panel->appendControl($forum);
	$panel->append('forum_config', 'configure Forum');
	$panel->append('forum_manage', 'sort and manage Forum');
	$panel->submit();
	
	$panel->appendControl($forumEntry);
	$panel->append('category/module/forum', 'Forum category');
	$panel->append('forum_entry_write', 'write new Forum entry');
	$panel->append('forum_entry_contributor_list', 'list my Forum entry');
	$panel->append('forum_entry_editor_list', 'list Forum entry in Section');
	$panel->submit();
	
	$flash = new FlashWidgetManager('forum_entry_last_post');
	$flash->setMode('forum_entry');
	$flash->setName(tt('Last posted Forum Entries'));
	$flash->setTable('forum_entry');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2) ORDER BY id DESC');
	$flash->submit();
	
	$flash->setReference('forum_entry_last_commented');
	$flash->setName(tt('Last commented Forum Entries'));
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2 AND comment_time > 0) ORDER BY comment_time DESC');
	$flash->submit();
	
	$flash = new FlashWidgetManager('forum_list');
	$flash->setMode('forum_entry_list/forum');
	$flash->setName(tt('List of Forums'));
	$flash->setTable('forum');
	$flash->setColumn('name');
	$flash->setClause('WHERE section='.SECTION.' AND category != 0 ORDER BY position');
	$flash->submit();
	
	$widget = new WidgetManager(tt('Advanced Last commented Forum Entries'), 'forum_entry_last_commented');
	$widget->setPosition('disabled');
	$widget->setFormMode('forum_entry_last_commented_config');
	$widget->setSaveMode('forum_entry_last_commented_config_save');
	$widget->submit();
	
	menu_drop('forum_entry');
	menu_add('forum', tt('Forum'));
}

function module_drop_forum(){
	include_once('Forum.class.inc');
	include_once('ForumEntry.class.inc');
	include_once('ForumConfig.class.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$forumEntry = new ForumEntry();
	$forumEntry->dropModule();
	
	$forum = new Forum();
	$forum->dropModule();
	
	$forumConfig = new ForumConfig();
	$forumConfig->dropModule();
	
	$panel = new PanelManager('Forum');
	$panel->drop();
	
	$widget = new WidgetManager(tt('Advanced Last commented Forum Entries'), 'forum_entry_last_commented');
	$widget->drop();
	
	content_drop_module('forum_entry', tt('Forum Entry'));
	menu_drop('forum');
	notify_add_info(tt('Forum has been successfully deactivated'));
}
?>
