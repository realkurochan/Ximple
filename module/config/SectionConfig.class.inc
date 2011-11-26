<?php
execute_add_class('config', 'SectionConfig');
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('SectionConfigModel.class.inc');

class SectionConfig extends ConfigControl{
	function SectionConfig(){
		$this->moduleName = 'config';
		$this->moduleTitle = tt('Section Configuration');
		$this->addAction('config_clear_db_cache', 'clearDBCache');
		$this->addAction('config_reset', 'reset');
		$this->setAdmin('config_admin');
		$this->init(new SectionConfigModel());
	}
	
	public function save(){
		extension_execute('section_config_save');
		$oldIcon = cc('icon');
		config_set('supported_language', pp('supported_language'));
		ConfigControl::save();
		$this->uploadIcon($oldIcon);
		$this->setState();
		$this->modelConnector->triggerPostInsert();
		$this->setDefaultSubscription();
	}
	
	public function index(){
		$this->formMeta->id = 'config';
		$this->formMeta->description = tt('You can use this form to configure important parameters of the Section. ');
		$p = tt('Or you can <a href="%s"><strong>reset configurations</strong></a> to the default values.');
		$this->formMeta->description.= sprintf($p, SECTION_URI.Q.'config_reset');
		$this->formMeta->mode = 'config_save';
		$this->formMeta->title = $this->moduleTitle;
		
		$general = $this->getNewConfigForm(tt('General Information'));
		$general->setShowByDefault(true);
		$general->append('section_name');
		$general->append('section_slogan');
		$general->append('index_mode');
		$general->append('home_label');
		$general->append('currency_label');
		$general->append('icon');
		$general->append('friendly_url_enable');
		$general->append('state');
		
		$language = $this->getNewConfigForm(tt('Language'));
		$language->append('supported_language');
		
		$user = $this->getNewConfigForm(tt('User & Registration'));
		$user->append('registration_enable');
		$user->append('forget_password');
		$user->append('avatar_resizable');
		$user->append('avatar_size');
		$user->append('avatar_big_size');
		
		$time = $this->getNewConfigForm(tt('Time'));
		$time->append('time_zone');
		$time->append('time_format');
		
		$searchEngine = $this->getNewConfigForm(tt('Search Engine'));
		$searchEngine->append('section_description');
		$searchEngine->append('section_keywords');
		
		$html = $this->getNewConfigForm(tt('HTML'));
		$html->append('meta_data');
		$html->append('allowed_tags');
		$html->append('link_target');
		
		$smiley = $this->getNewConfigForm(tt('Smiley'));
		$smiley->append('smiley');
		
		$cacheDescription  = tt('Database cache is a part of the system. It can not be disabled. ');
		$p = tt('You can, however, <a href="%s"><strong>clear database cache clicking here</strong></a>. ');
		$cacheDescription .= sprintf($p, SECTION_URI.Q.'config_clear_db_cache');
		$cacheDescription .= tt('This can take a long time. ');
		$cacheDescription .= tt('It should be noted that after submitting this configuration, ');
		$cacheDescription .= tt('all caches excepted database cache will be cleared.');
		$cache = $this->getNewConfigForm(tt('Cache'));
		$cache->setDescription($cacheDescription);
		$cache->append('cache_mode');
		$cache->append('cache_time');
		
		$feed = $this->getNewConfigForm(tt('Feed'));
		$feed->append('feed_mode');
		$feed->append('feed_type');
		$feed->append('feed_number');
		$feed->append('feed_comment');
		$feed->append('feed_comment_explicit');
		
		$spam = $this->getNewConfigForm(tt('Anti-Spamming'));
		$spam->append('antispam_salt');
		$spam->append('antispam_time');
		
		$syslog = $this->getNewConfigForm(tt('System Log & Notification'));
		$syslog->append('syslog_enable');
		$syslog->append('syslog_time');
		$syslog->append('tracker_enable');
		$syslog->append('tracker_time');
		$syslog->append('notify_interval');
		$syslog->append('notify_default_subscription');
		
		$this->renderAdditionalConfig();
		
		return $this->render();
	}
	
	public function clearDBCache(){
		cache_clear_db();
		execute_set_db();
		document_set_redirect('config');
		notify_add_info(tt('Database cache has been cleared.'));
	}
	
	public function reset(){
		$configFile = FILES_PATH.'config/config'.SECTION.'.php';
		if(is_file($configFile)) unlink($configFile);
		$panelFile = FILES_PATH.'config/panel'.SECTION.'.php';
		if(is_file($panelFile)) unlink($panelFile);
		$roleFile = FILES_PATH.'config/role'.SECTION.'.php';
		if(is_file($roleFile)) unlink($roleFile);
		execute_set_db();
		document_set_redirect('config');
		notify_add_info(tt('Configuration have been successfully reset.'));
	}
	
	public function getConfigValue($reference){
		return cc($reference);
	}
	
	private function uploadIcon($oldIcon){
		$dir = FILES_PATH.'misc/';
		if(!is_dir($dir)) mkdir($dir);
		$icon = io_upload(ff('icon'), FILES_PATH.'misc/');
		if(strlen($icon)){
			config_set('icon', $icon);
			if(is_file(ROOT_PATH.$oldIcon) and $oldIcon != 'files/misc/ximple.png'){
				unlink(ROOT_PATH.$oldIcon);
			}
		}else{
			config_set('icon', $oldIcon);
		}
		config_save();
	}
	
	private function setState(){
		$dir = FILES_PATH.'state/';
		if(!is_dir($dir)) mkdir($dir);
		io_write_file($dir.'install', "");
		$dir.= SECTION.'/';
		io_clear_directory($dir);
		if(!is_dir($dir)) mkdir($dir);
		io_write_file($dir.pp('state'), "");
	}
	
	private function renderAdditionalConfig(){
		include_once(INCLUDE_PATH.'class/ExtendedConfigFormRender.class.inc');
		$formRender = new ExtendedConfigFormRender($this, 'section_config');
		$formRender->render();
	}
	
	private function setDefaultSubscription(){
		$default = cc('notify_default_subscription');
		$directory = notify_create_subscribe_directory();
		$uid = db_select_single_column('users', 'id', 'WHERE section='.SECTION);
		foreach($default as $key => $value){
			io_serialize($uid, $directory.$value);
		}
	}
}

?>
