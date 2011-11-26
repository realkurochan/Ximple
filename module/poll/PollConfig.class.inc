<?php
execute_add_extended_class('poll', 'config', 'PollConfig');
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('PollConfigModel.class.inc');

class PollConfig extends ConfigControl{
	function PollConfig(){
		$this->moduleName = 'poll';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('Poll Configuration');
		$this->setAdmin('poll_admin');
		$this->init(new PollConfigModel());
	}
	
	public function index(){
		document_set_path(tt('Poll'), 'poll');
		$this->formMeta->mode = 'poll_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
}
?>
