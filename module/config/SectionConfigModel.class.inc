<?php
require_once(INCLUDE_PATH.'class/column/MultipleFilesColumn.class.inc');
require_once(INCLUDE_PATH.'tool/country.tool.inc');

class SectionConfigModel extends Model{
	function SectionConfigModel(){
		$this->section_name = $this->title(tt('section name'));
		$this->section_name->enableTranslate();
		$this->section_slogan = $this->char(tt('section solgan'));
		$this->section_slogan->enableTranslate();
		$this->home_label = $this->title(tt('home lable'));
		$this->home_label->enableTranslate();
		$this->index_mode = $this->select('fronpage module', cc('module_name'));
		$this->icon = $this->image(tt('section icon (16x16)'), 'files/misc/');
		
		$help = tt('Currency is not automatically converted to given label. ');
		$help.= tt('To avoid calculation error, you should only one currency.');
		$this->currency_label = $this->title(tt('curreny label'), $help);
		
		if(function_exists('apache_get_modules')) $module = apache_get_modules();
		else $module = null;
		if($module==null or in_array('mod_rewrite', $module)){
			$this->friendly_url_enable = $this->enable(tt('enable friendly URI'));
		}else{
			$this->friendly_url_enable = $this->hidden();
			$this->friendly_url_enable->type = 'int';
			$this->friendly_url_enable->defaultData = 0;
		}
		
		$option = array(
			'develop' => tt('development'),
			'product' => tt('production'),
			'maintain' => tt('maintenance'),
			'test' => tt('test'),
		);
		$this->state = $this->radio(tt('section state'), $option);
		
		$this->registration_enable = $this->enable(('Enable registration for new users'));
		$this->forget_password = $this->enable(('Enable password recovery'));
		$this->avatar_resizable = $this->enable(('Enable resizing avatar'));
		$this->avatar_size = $this->spin(('Avatar size'), 32, 1024);
		$this->avatar_big_size = $this->spin(('Big avatar size'), 64, 1024);
		
		$help = tt('Add supported language for contents in Section.');
		$this->supported_language = $this->grid(tt('Supported language'), $help);
		$this->supported_language->isEmptyForm = true;
		$language = Model::select(tt('country'), country());
		$language->data = cc('supported_language');
		$this->supported_language->append('supported_language', $language);
		
		$option = timezone_identifiers_list();
		$option = array_combine($option, $option);
		$this->time_zone = $this->select(tt('time zone'), $option);
		
		$formats = array('d F Y', 'd M Y', 'd F y', 'd M y');
		$option = array();
		foreach($formats as $format){
			$option[$format] = date($format);
		}
		$this->time_format = $this->select(tt('time format'), $option);
		
		$cacheOptions = array(
			3 => tt('agressive process cache (side-effects)'),
			2 => tt('page cache (recommended)'),
			1 => tt('partly process cache'),
			0 => tt('database cache only'),
		);
		$this->cache_mode = $this->radio(tt('cache mode'), $cacheOptions);
		$this->cache_time = $this->timeInterval(tt('cache time'));
		$this->cache_time->isSecond = false;
		
		$this->section_description = $this->plain(tt('description'));
		$this->section_keywords = $this->plain(tt('keywords'));
		
		$this->meta_data = $this->plain(tt('HTML header meta-data'));
		$this->allowed_tags = $this->plain(tt('allowed HTML-TAG'));
		
		$targetOption = cc('module_name');
		$targetOption['#notify'] = tt('Notification');
		$targetOption['#external'] = tt('External link');
		$this->link_target = $this->checkBox(tt('open page in new window by'), $targetOption);
		
		$availableFeed = cc('feed_available_mode');
		if(!is_array($availableFeed)) $availableFeed = array();
		$this->feed_mode = $this->checkbox(tt('modules enabled feed'), $availableFeed);
		$this->feed_type = $this->checkbox(tt('feed type'), array('atom' => 'Atom', 'rss' => 'RSS'));
		$this->feed_number = $this->spin(tt('number of feed entries'), 5, 50);
		$this->feed_comment = $this->enable(tt('enable comment feed'));
		$this->feed_comment_explicit = $this->enable(tt('explicit comment feed'));
		
		$this->antispam_salt = $this->title(tt('antispam salt'));
		$this->antispam_time = $this->timeInterval(tt('antispam time'));
		$this->antispam_time->isDay = false;
		$this->antispam_time->isSecond = false;
		
		$this->syslog_enable = $this->enable(tt('system logging enable'));
		
		$this->syslog_time = $this->timeInterval(tt('system logging expiration'));
		$this->syslog_time->isMinute = false;
		$this->syslog_time->isSecond = false;
		
		$this->tracker_enable = $this->enable(tt('user tracker enable'));
		
		$this->tracker_time = $this->timeInterval(tt('user tracker expiration'));
		$this->tracker_time->isMinute = false;
		$this->tracker_time->isSecond = false;
		
		$this->notify_interval = $this->timeInterval(tt('notification refresh interval'));
		$this->notify_interval->isDay = false;
		$this->notify_interval->isHour = false;
		
		$this->notify_default_subscription = $this->checkbox(tt('default subscription'), notify_get_subscribable_module());
		
		$path = FILES_PATH.'smiley/'.SECTION.'/';
		$this->smiley = new MultipleFilesColumn(tt('smiley'), $path, 'smiley');
		$pattern = tt('You can also use <a href="%s">File Manage</a> to manage smiley in Section');
		$link = SECTION_URI.'?mode=file_manage_admin&dir=smiley/'.SECTION.'/';
		$this->smiley->help = sprintf($pattern, $link);
	}
}
?>
