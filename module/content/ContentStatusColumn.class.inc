<?php
require_once(INCLUDE_PATH.'class/column/RadioColumn.class.inc');

class ContentStatusColumn extends RadioColumn{
	private $moduleName;
	private $status;
	
	public $isTriggerSelect = false;
	
	function ContentStatusColumn($label, $moduleName){
		RadioColumn::SelectiveColumn($label, array());
		$this->setType('int');
		$this->moduleName = $moduleName;
	}
	
	public function setStatus($status){
		$this->status = $status;
		$this->defaultData = cc($this->moduleName.'_'.$this->status.'_default');
	}
	
	public function setOption($option){
		$this->option = $option;
	}
	
	public function triggerInsertForm(){
		$this->initSelect();
	}
	
	public function triggerUpdateForm(){
		$this->initSelect();
	}
	
	public function triggerSelect(){
		if($this->isTriggerSelect) $this->data = $this->option[$this->data];
	}
	
	private function initSelect(){
		$adminGroup = $this->moduleName.'_admin';
		$editorGroup = $this->moduleName.'_editor';
		if(!(authority_group($adminGroup) or authority_group($editorGroup))){
			$config = cc($this->moduleName.'_'.$this->status);
			$option = array();
			foreach($config as $key){
				$option[$key] = $this->option[$key];
			}
			$this->option = $option;
		}
	}
}
?>
