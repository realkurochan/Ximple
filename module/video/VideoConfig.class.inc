<?php
execute_add_extended_class('video', 'config', 'VideoConfig');
include_once(MODULE_PATH.'content/ContentConfig.class.inc');
include('VideoConfigModel.class.inc');

class VideoConfig extends ContentConfig{
	function VideoConfig(){
		$this->moduleName = 'video';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('Video Configuration');
		$this->setAdmin('video_admin');
		$this->init(new VideoConfigModel());
		$this->setMode('video');
	}
	
	public function index(){
		document_set_path(tt('Video'), 'video');
		$this->groupConfigForm();
		$this->appearanceFormGroup->append('video_width');
		$this->appearanceFormGroup->append('video_height');
		$this->appearanceFormGroup->append('video_icon_size');
		$this->appearanceFormGroup->append('video_entries_number');
		$this->formMeta->mode = 'video_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
}
?>
