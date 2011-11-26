<?php
function module_get_info_contact(){
	$describe = tt('Let your visitors send email to the you with out knowing your email address. ');
	$describe.= tt('Moreover, you can show them your contact information on Contact-Page.');
	return array(
		'name' => tt('Contact'),
		'description' => $describe,
	);
}

function module_init_contact(){
	include_once('Contact.class.inc');
	include_once('ContactConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	
	$contact = new Contact();
	$contact->initModule('Contact.class.inc');
	
	$contactConfig = new ContactConfig();
	$contactConfig->initModule('ContactConfig.class.inc');
	
	$admin = new RoleManager('contact_admin');
	$admin->setName('Contact Administrator');
	$admin->setDescription('Who can insert, edit and drop Contact.');
	$admin->submit();
	
	$panel = new PanelManager('Contact');
	$panel->appendControl($contact);
	$panel->append('contact_write', 'create new Contact');
	$panel->append('contact', 'list & manage  Contact');
	$panel->append('contact_config', 'config Contact');
	$panel->appendRole($admin);
	$panel->submit();
	
	notify_add_info(tt('Contact has been successfully activated'));
	$contact_data = db_select_single_column('contact', 'name', 'WHERE section='.SECTION);
	foreach($contact_data as $key => $value){
		widget_add_navigation('contact/'.$key, $value);
	}
	$contact_dir = FILES_PATH.'contact';
	if(!is_dir($contact_dir)) mkdir($contact_dir);
}

function module_drop_contact(){
	include_once('Contact.class.inc');
	include_once('ContactConfig.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	
	$contact = new Contact();
	$contact->dropModule();
	
	$contactConfig = new ContactConfig();
	$contactConfig->dropModule();
	
	$admin = new RoleManager('contact_admin');
	$admin->drop();
	
	$panel = new PanelManager('Contact');
	$panel->drop();
	
	module_drop_cache('contact');
	notify_add_info(tt('Contact has been successfully deactivated'));
	$navigation = cc('navigation');
	foreach($navigation as $key => $value){
		if(substr($key,0, 7) == 'contact') widget_drop_navigation($key);
	}
}
?>
