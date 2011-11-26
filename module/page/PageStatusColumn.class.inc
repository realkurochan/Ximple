<?php
require_once(INCLUDE_PATH.'class/column/RadioColumn.class.inc');

class PageStatusColumn extends RadioColumn{
	private $status;
	public $configuration;
	public $isTriggerSelect = false;
	
	function PageStatusColumn($label, $status){
		RadioColumn::RadioColumn($label, array());
		$this->setType('int');
		$this->status = $status;
	}
	
	public function triggerInsertForm(){
		$this->defaultData = $this->configuration[$this->status.'_default'];
		$this->initSelect();
	}
	
	public function triggerUpdateForm(){
		$this->initSelect();
	}
	
	public function triggerSelect(){
		if($this->isTriggerSelect){
			$functionName = 'content_get_'.$this->status;
			$option = $functionName();
			$this->data = $option[$this->data];
		}
	}
	
	private function initSelect(){
		$functionName = 'content_get_'.$this->status;
		$option = $functionName();
		if(!(authority_group('page_admin') or authority_group('page_editor'))){
			$enabled = $this->configuration[$this->status];
			foreach($enabled as $key){
				$this->option[$key] = $option[$key];
			}
		}else{
			$this->option = $option;
		}
	}
}

?>
