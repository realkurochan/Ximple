<?php
function module_get_info_guestbook(){
	return array(
		'name' => tt('Guestbook'),
		'description' => tt('Let your visitors sign and write something on your web site.')
	);
}

function module_init_guestbook(){
	include_once('Guestbook.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	$guestbook = new Guestbook();
	$guestbook->initModule('Guestbook.class.inc');
	
	$admin = new RoleManager('guestbook_admin');
	$admin->setName('Guestbook Administrator');
	$admin->setDescription('Who can edit and drop guestbook.');
	$admin->submit();
	
	module_add_cache('guestbook');
	widget_add_navigation('guestbook', tt('Guestbook'));
	notify_add_info(tt('Guestbook has been successfully activated'));
	$p = tt('You can now assess Guestbook with <a href="%s">this link</a>. Add this link somewhere to Widget or Menu.');
	notify_add_tip(sprintf($p, SECTION_URI.Q.'guestbook'));
}

function module_drop_guestbook(){
	include_once('Guestbook.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	$guestbook = new Guestbook();
	$guestbook->dropModule();
	
	$admin = new RoleManager('guestbook_admin');
	$admin->drop();
	
	module_drop_cache('guestbook');
	widget_drop_navigation('guestbook');
	notify_add_info(tt('Guestbook has been successfully deactivated'));
}
?>
