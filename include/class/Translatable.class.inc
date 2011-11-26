<?php
include_once('Authority.class.inc');

abstract class Translatable extends Authority{
	public $moduleName;
	public $moduleExtension;
	public $moduleTitle;
	public $fullModuleName;
	
	protected $modelConnector;
	protected $configForm = array();
	protected $formMeta;
	
	private $translateFormRender;
	
	public function renderTranslateForm(){
		execute_set_ajax();
		require_once(INCLUDE_PATH.'tool/country.tool.inc');
		$country = country();
		$this->formMeta->id = $this->fullModuleName;
		$this->formMeta->title = sprintf(tt('Translation : %s'), $country[gg('language')]);
		$this->formMeta->mode = $this->fullModuleName.'_translate';
		$this->prepareTranslateForm();
		$translation = translate_get_all_data($this->fullModuleName, gg('module_id'), gg('language'));
		$render = $this->translateFormRender->renderTranslateForm($translation);
		return $render;
	}
	
	public function setTranslate(){
		execute_set_ajax();
		translate_set_data(pp());
		notify_add_info(tt('Your translated content has been successfully updated.'));
	}
	
	protected function init($modelObj){
		$this->checkAtribute($modelObj);
		if(strlen($this->moduleExtension) == 0) $this->fullModuleName = $this->moduleName;
		else $this->fullModuleName = $this->moduleName.'_'.$this->moduleExtension;
	}
	
	protected function translateData(){
		foreach($this->data as $key => $value){
			$this->data[$key] = translate_content($value, $this->fullModuleName, $value['id']);
		}
		if($this->id and isset($this->data[$this->id])) $this->dataByID = $this->data[$this->id];
	}
	
	protected function translateDataByID(){
		$this->dataByID = translate_content($this->dataByID, $this->fullModuleName, $this->id);
	}
	
	private function prepareTranslateForm(){
		if(!$this->translateFormRender){
			include_once('FormRender.class.inc');
			include_once('TranslateFormRender.class.inc');
			$this->translateFormRender = new TranslateFormRender($this->modelConnector, $this->formMeta, $this->fullModuleName);
		}
		$this->formMeta->configForm = $this->configForm;
		if($this->formMeta->isSetTitle){
			document_set_title($this->formMeta->title);
		}
	}
	
	private function checkAtribute($modelObj){
		if(state_is_develop()){
			$message = array();
			$pattern = tt('Atrribute mismatched Model & Control (%s) : %s vs %s.');
			$attribute = array(	
				'moduleName',
				'moduleExtension',
				'fullModuleName',
				'tableName',
			);
			foreach($attribute as $key => $value){
				if(strlen($modelObj->$value)){
					if(strlen($this->$value) == 0){
						$this->$value = $modelObj->$value;
					}elseif($this->$value != $modelObj->$value){
						$message[] = sprintf($pattern, $value, $this->$value, $modelObj->$value);
					}
				}
			}
			foreach($message as $key => $value){
				notify_add_error($value);
			}
		}
	}
}
?>
