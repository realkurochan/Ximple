<?php
function module_get_info_faq(){
	return array(
		'name' => tt('FAQ'),
		'description' => tt('Frequency Asked Question. An introduction to Ximple can be discover here.')
	);
}

function module_init_faq(){
	include_once('FAQ.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	if(!is_dir('files/faq/')) mkdir('files/faq/');
	
	$faq = new FAQ();
	$faq->initModule('FAQ.class.inc');
	
	$admin = new RoleManager('faq_admin');
	$admin->setName('FAQ Administrator');
	$admin->setDescription('Who can write, edit, drop and configure Faq');
	$admin->submit();
	
	$contributor = new RoleManager('faq_contributor');
	$contributor->setName('FAQ Contributor');
	$contributor->setDescription('Who can write and edit own question.');
	$contributor->submit();
	
	$panel = new PanelManager('FAQ');
	$panel->appendControl($faq);
	$panel->append('category/module/faq', 'FAQ category');
	$panel->append('faq_arrange', 'rearrage FAQ');
	$panel->append('faq_list', 'list all questions');
	$panel->append('faq_contributor_list', 'list my questions');
	$panel->append('faq_write', 'write new question');
	$panel->appendRole($admin);
	$panel->appendRole($contributor);
	$panel->submit();
	
	widget_add_navigation('faq', tt('FAQ'));
	notify_add_info(tt('FAQ has been successfully activated'));
}

function module_drop_faq(){
	include_once('FAQ.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	$faq = new FAQ();
	$faq->dropModule();
	
	$admin = new RoleManager('faq_admin');
	$admin->drop();
	
	$contributor = new RoleManager('faq_contributor');
	$contributor->drop();
	
	$panel = new PanelManager('FAQ');
	$panel->drop();
	
	widget_drop_navigation('faq');
	notify_add_info(tt('FAQ has been successfully deactivated'));
}
?>
