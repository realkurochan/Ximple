<?php
function module_get_info_blog(){
	$describe = tt('Blogging system for Ximple (see <a href="http://en.wikipedia.org/wiki/Blog">Wikipedia</a> for more information about blog).');
	return array(
		'name' => tt('Blog'),
		'description' => $describe,
	);
}

function module_init_blog(){
	include_once('Blog.class.inc');
	include_once('BlogConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$blog = new Blog();
	$blog->initModule('Blog.class.inc');
	
	$blogConfig = new BlogConfig();
	$blogConfig->initModule('BlogConfig.class.inc');
	
	$flash = new FlashWidgetManager('blog_last_post');
	$flash->setMode('blog');
	$flash->setName(tt('Last posted Blogs'));
	$flash->setTable('blog');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2) ORDER BY publish_time DESC');
	$flash->submit();
	
	$flash = new FlashWidgetManager('blog_last_commented');
	$flash->setMode('blog');
	$flash->setName(tt('Last commented Blogs'));
	$flash->setTable('blog');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND publishing_status < 2 AND comment_time > 0) ORDER BY comment_time DESC');
	$flash->submit();
	
	$widget = new WidgetManager(tt('Promoted Blogs'), 'blog_promote_widget');
	$widget->submit();
	
	config_set('blog_entries_number', 5);
	config_set('blog_icon_size', 64);
}

function module_drop_blog(){
	include_once('Blog.class.inc');
	include_once('BlogConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
	
	$blog = new Blog();
	$blog->dropModule();
	
	$blogConfig = new BlogConfig();
	$blogConfig->dropModule();
	
	config_drop('blog_entries_number');
}
?>
