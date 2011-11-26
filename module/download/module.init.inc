<?php
function module_get_info_download(){
	$describe = tt('With Download-Module, you can provide files for downloading on your website. ');
	$describe.= tt('The files are categorized with the configurations for access permission.');
	return array(
		'name' => tt('Download'),
		'description' => $describe,
	);
}

function module_init_download(){
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once('Download.class.inc');
	$download = new Download();
	$download->initModule('Download.class.inc');
	
	$admin = new RoleManager('download_admin');
	$admin->setName('Download Administrator');
	$admin->setDescription('Who can provide new files for downloading, edit and delete them.');
	$admin->submit();

	$panel = new PanelManager('Download');
	$panel->appendControl($download);
	$panel->append('download_write', 'upload new file');
	$panel->append('download', 'list & manage  download');
	$panel->append('category/module/download', 'download category');
	$panel->appendRole($admin);
	$panel->submit();
	
	module_add_cache('download');
	notify_add_info(tt('Download has been successfully activated'));
	widget_add_navigation('download', tt('Download'));
	category_init('download');
	
	$dowload_dir = FILES_PATH.'download/';
	if(!is_dir($dowload_dir)) mkdir($dowload_dir);
}

function module_drop_download(){
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once('Download.class.inc');
	$download = new Download();
	$download->dropModule();
	
	$admin = new RoleManager('download_admin');
	$admin->drop();
	
	$panel = new PanelManager('Download');
	$panel->drop();
	
	module_drop_cache('download');
	notify_add_info(tt('Download has been successfully deactivated'));
	widget_drop_navigation('download');
}
?>
