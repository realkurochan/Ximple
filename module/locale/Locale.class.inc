<?php
execute_add_class('locale', 'Locale');

require_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
require_once('LocaleModel.class.inc');
require_once('locale.tool.inc');

if(!state_is_develop()) error_denied();

class Locale extends ConfigControl{
	function Locale(){
		$this->moduleName = 'locale';
		$this->moduleTitle = tt('Get Locale Array');
		$this->setAdmin('locale_admin');
		$this->init(new LocaleModel());
	}
	
	public function index(){
		document_set_path(tt('Locale'), 'locale');
		document_set_path(tt('Get Locale Array'));
		$this->formMeta->mode = 'locale';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
	
	protected function getConfig(){
		return array(
			'directory' => pp('directory'),
			'language' => pp('language'),
			'locale' => locale_get(pp('directory'), pp('language')),
		);
	}
}
?>
