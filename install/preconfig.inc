<?php
function preconfig_init(){
	include(INCLUDE_PATH.'tool/main.tool.inc');
	include(INCLUDE_PATH.'tool/extension.tool.inc');
	include(INCLUDE_PATH.'tool/execute.tool.inc');
	include(INCLUDE_PATH.'tool/translate.tool.inc');
	include(INCLUDE_PATH.'tool/notify.tool.inc');
	include(INCLUDE_PATH.'tool/document.tool.inc');
	include(INCLUDE_PATH.'tool/count.tool.inc');
	include(INCLUDE_PATH.'tool/widget.tool.inc');
	include(MODULE_PATH.'module/module.module.inc');
	define('MODE_PAGE', 0);
}
	
function preconfig_blog(){
	preconfig_init();
	module_prepare();
	include(MODULE_PATH.'blog/module.init.inc');
	module_init_blog();
	config_set('index_mode', 'blog');
	config_append('feed_mode', 'blog');
	config_save();
	module_post();
}

function preconfig_personal(){
}

function preconfig_community(){
}

function preconfig_document(){
}

function preconfig_SME(){
}

?>
