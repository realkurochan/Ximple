<?php
require_once('HiddenColumn.class.inc');
class NowColumn extends HiddenColumn{
	public $isUpdateTime=false;
	public $formatDateBySelect = false;
	public $dateFormat;
	
	function NowColumn($dateFormat=null){
		HiddenColumn::HiddenColumn(NOW, 'char');
		if(!empty($dateFormat)){	
			$this->dateFormat = $dateFormat;
			$this->formatDateBySelect = true;
		}else{
			$this->dateFormat = 'Y-m-d H:i:s';
			$this->formatDateBySelect = false;
		}
	}
	
	public function triggerUpdateForm(){
		if($this->isUpdateTime) $this->data = NOW;
	}
	
	public function triggerSelect(){
		if($this->formatDateBySelect){
			$this->data = date($this->dateFormat, strtotime($this->data));
		}
	}
}
?>
