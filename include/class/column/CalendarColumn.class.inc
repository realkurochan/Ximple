<?php
require_once('CharColumn.class.inc');
class CalendarColumn extends CharColumn{
	public $formatDateBySelect = true;
	public $dateFormat;
	public $emptyStart = false;
	
	function CalendarColumn($label, $help=null){
		CharColumn::CharColumn($label, $help);
		$this->defaultData = NOW;
		$this->dateFormat = cc('time_format');
	}
	
	public function triggerGetJavaScript(){
		head_add_js('include/javascript/calendar.column.js');
	}
	
	public function triggerSelect(){
		if($this->formatDateBySelect){
			$this->data = date($this->dateFormat, strtotime($this->data));
		}
	}
	
	public function renderForm($formID){
		$time = strtotime($this->data);
		if($this->emptyStart or $time < 31536000){
			$this->data = '';
		}elseif($this->data){
			$this->data = date('d F Y', strtotime($this->data));
		}else{
			$this->data = date('d F Y');
		}
		$this->setFormAttribute('class', 'calendar_form');
		return CharColumn::renderForm($formID);
	}
	
	public function triggerPostInsert(){
		$this->data = date('Y-m-d', strtotime($this->data)).' '.date('H:i:s');
	}
	
	public function triggerPostUpdate(){
		$this->data = date('Y-m-d', strtotime($this->data)).' '.date('H:i:s');
	}
}
?>
