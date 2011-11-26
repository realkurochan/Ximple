<?php
function module_get_info_poll(){
	$description = tt('Poll module for XimpleCMS. It can be use as a widget or can be embed in other module.');
	return array(
		'name' => tt('Poll'),
		'description' => $description,
	);
}

function module_init_poll(){
	require_once('Poll.class.inc');
	require_once('PollConfig.class.inc');
	require_once('PollChoiseModel.class.inc');
	require_once('PollVoteModel.class.inc');
	include_once(INCLUDE_PATH.'class/QuickTableCreator.class.inc');
	
	$poll = new Poll();
	$poll->initModule('Poll.class.inc');
	
	$pollConfig = new PollConfig();
	$pollConfig->initModule('PollConfig.class.inc');
	
	$pollChoise = new QuickTableCreator(new PollChoiseModel());
	$pollChoise->create();
	
	$pollVote = new QuickTableCreator(new PollVoteModel());
	$pollVote->create();
	
	$admin = new RoleManager('poll_admin');
	$admin->setName('Poll Administrator');
	$admin->setDescription('Who can insert, edit and drop Poll.');
	$admin->submit();
	
	$voter = new RoleManager('poll_voter');
	$voter->setName('Poll Voter');
	$voter->setDescription('Who can vote in restricted Poll.');
	$voter->submit();
	
	$panel = new PanelManager('Poll');
	$panel->appendControl($poll);
	$panel->append('poll_write', 'create new Poll');
	$panel->append('poll_list', 'list & manage  Poll');
	$panel->append('poll_config', 'configure Poll');
	$panel->appendRole($admin);
	$panel->submit();
	
	$widget = new WidgetManager(tt('Poll'), 'poll_widget');
	$widget->submit();
	
	config_set('poll_comment_write_label', tt('Please, write your opinion.'));
	config_set('poll_comment_page_length', 10);
	config_set('poll_result_color', '#999');
	extension_add('content_post_select', 'poll_embed');
	extension_add('content_edit_description', 'poll_embed_icon');
}

function module_drop_poll(){
	require_once('Poll.class.inc');
	require_once('PollConfig.class.inc');
	
	$poll = new Poll();
	$poll->dropModule();
	
	$pollConfig = new PollConfig();
	$pollConfig->dropModule();
	
	$admin = new RoleManager('poll_admin');
	$admin->drop();
	
	$voter = new RoleManager('poll_voter');
	$voter->drop();
	
	$panel = new PanelManager('Poll');
	$panel->drop();
	
	$widget = new WidgetManager(tt('Poll'), 'poll_widget');
	$widget->drop();
	
	extension_drop('content_post_select', 'poll_embed');
	extension_drop('content_edit_description', 'poll_embed_icon');
	extension_drop('content_edit_description', 'poll_write_icon');
}
?>
