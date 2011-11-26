<?php
execute_add_extended_class('forum', 'config', 'ForumConfig');
include_once(MODULE_PATH.'content/ContentConfig.class.inc');
include_once('ForumConfigModel.class.inc');

class ForumConfig extends ContentConfig{
	function ForumConfig(){
		$this->moduleName = 'forum';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('Forum Configuration');
		$this->setAdmin('forum_admin');
		$this->init(new ForumConfigModel());
		$this->setMode('forum');
	}
	
	public function index(){
		document_set_path(tt('Forum'), 'forum');
		$this->groupConfigForm();
		$this->formMeta->mode = 'forum_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
	
	public function save(){
		foreach(pp() as $key => $value){
			$entryKey = str_replace('forum_', 'forum_entry_', $key);
			config_set($entryKey, $value);
		}
		$levelNumber = pp('forum_user_level_number');
		$levelName = pp('forum_user_level_name');
		$level = array();
		foreach($levelNumber as $key => $value){
			if(strlen($levelName[$key]) and ((int) $value) > 0){
				$level[$value] = $levelName[$key];
			}
		}
		ksort($level);
		config_set('forum_user_level', $level);
		ContentConfig::save();
	}
	
	protected function groupConfigForm(){
		ContentConfig::groupConfigForm();
		$userLevel = $this->getNewConfigForm(tt('User Levels'));
		$userLevel->append('forum_user_level');
	}
}
?>
