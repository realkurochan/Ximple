<?php
function module_get_info_develcard(){
	return array(
		'name' => tt('DevelCard'),
		'description' => tt('Development card for software improvments')
	);
}

function module_init_develcard(){
	module_develcard_require();
	$panel = new PanelManager('DevelCard');
	module_develcard_init_control($panel);
	module_develcard_init_role($panel);
	module_develcard_init_panel($panel);
	module_add('develcard', 'develcard.widget.module.inc', tt('DevelCard'), 'widget');
	module_develcard_add_widget();
	notify_add_subscribe('develcard', tt('someone posts in DevelCard', true));
	notify_add_subscribe('develcard_comment', tt('someone comments on DevelCard', true));
	module_develcard_init_config();
}

function module_drop_develcard(){
	module_develcard_require();
	module_develcard_drop_control();
	module_develcard_drop_role();
	module_develcard_drop_panel();
	notify_drop_subscribe('develcard');
	notify_drop_subscribe('develcard_comment');
}

function module_develcard_require(){
	include_once('DevelCard.class.inc');
	include_once('DevelCardConfig.class.inc');
	include_once('ResolverModel.class.inc');
	include_once(INCLUDE_PATH.'tool/widget.tool.inc');
	include_once(INCLUDE_PATH.'class/QuickTableCreator.class.inc');
	include_once(MODULE_PATH.'flashwidget/FlashWidgetManager.class.inc');
}

function module_develcard_init_control($panel){
	$develcard = new DevelCard();
	$develcard->initModule('DevelCard.class.inc');
	
	$develcardConfig = new DevelCardConfig();
	$develcardConfig->initModule('DevelCardConfig.class.inc');
	
	$resolverCreator = new QuickTableCreator(new ResolverModel());
	$resolverCreator->create();
	$panel->appendControl($develcard);
}

function module_develcard_drop_control(){
	$develcard = new DevelCard();
	$develcard->dropModule();
	
	$develcardConfig = new DevelCardConfig();
	$develcardConfig->dropModule();
}

function module_develcard_init_role($panel){
	$admin = new RoleManager('develcard_admin');
	$admin->setName('DevelCard Administrator');
	$admin->setDescription('Who can configure, edit and drop DevelCard.');
	$admin->submit();
	
	$moderator = new RoleManager('develcard_moderator');
	$moderator->setName('DevelCard Moderator');
	$moderator->setDescription('Who can edit and drop DevelCard.');
	$moderator->submit();
	
	$resolver = new RoleManager('develcard_resolver');
	$resolver->setName('DevelCard Resolver');
	$resolver->setDescription('Who must resolve problems mentioned in DevelCard.');
	$resolver->submit();
	
	$reporter = new RoleManager('develcard_reporter');
	$reporter->setName('DevelCard Reporter');
	$reporter->setDescription('Who can DevelCard Report.');
	$reporter->submit();
	
	$panel->appendRole($admin);
	$panel->appendRole($moderator);
	$panel->appendRole($resolver);
	$panel->appendRole($reporter);
}

function module_develcard_drop_role(){
	$admin = new RoleManager('develcard_admin');
	$admin->drop();
	
	$moderator = new RoleManager('develcard_moderator');
	$moderator->drop();
	
	$reporter = new RoleManager('develcard_reporter');
	$reporter->drop();
}

function module_develcard_init_panel($panel){
	$panel->append('develcard_config', 'configure DevelCard');
	$panel->append('develcard_personal', 'my Develcard');
	$panel->append('develcard_personal_list', 'list of personals');
	$panel->append('develcard_write', 'submit new DevelCard');
	$panel->submit();
}

function module_develcard_drop_panel(){
	$panel = new PanelManager('DevelCard');
	$panel->drop();
}

function module_develcard_add_widget(){
	$flash = new FlashWidgetManager('develcard_recent_created');
	$flash->setMode('develcard');
	$flash->setName(tt('Recently created DevelCard'));
	$flash->setTable('develcard');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND status=0) ORDER BY comment_time DESC');
	$flash->submit();
	
	$flash = new FlashWidgetManager('develcard_recent_progress');
	$flash->setMode('develcard');
	$flash->setName(tt('Recent DevelCard in progress'));
	$flash->setTable('develcard');
	$flash->setColumn('topic');
	$flash->setClause('WHERE (section='.SECTION.' AND status=1) ORDER BY comment_time DESC');
	$flash->submit();
	
	$widget = new WidgetManager(tt('My DevelCard in progress'), 'develcard_widget_resolver');
	$widget->setFormMode('develcard_widget_resolver_config');
	$widget->setSaveMode('develcard_widget_resolver_save');
	$widget->submit();
	
	$widget = new WidgetManager(tt('DevelCard of current User in progress'), 'develcard_widget_personal');
	$widget->setFormMode('develcard_widget_personal_config');
	$widget->setSaveMode('develcard_widget_personal_save');
	$widget->submit();
}

function module_develcard_init_config(){
	if(!is_array(cc('develcard_type'))){
		config_set('develcard_access', 2);
		config_set('develcard_pixel_per_point', 12);
		config_set('develcard_load_color', '#ffffff');
		config_set('develcard_load_bg_color', '#aa00aa');
		config_set('develcard_point_color', '#ffffff');
		config_set('develcard_point_bg_color', '#00aaaa');
	
		config_set('develcard_list_per_page', 20);
		config_set('develcard_hight_color', '#ff0000');
		config_set('develcard_medium_color', '#ffff00');
		config_set('develcard_low_color', '#00ff00');
		config_set('develcard_no_effect_color', '#0000ff');
		config_set('develcard_type', develcard_get_default_type_option());
		config_set('develcard_comment_write_label', tt('Please, write your opinion'));
		config_set('develcard_comment_page_length', 10);
	
		config_set('develcard_widget_resolver_number', 5);
		config_set('develcard_widget_personal_number', 5);
	}
}
?>
