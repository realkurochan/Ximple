<?php
class TimeIntervalColumn extends ModelColumn{
	public $isDay = true;
	public $isHour = true;
	public $isMinute = true;
	public $isSecond = true;
	
	private $renderArray = array();
	private $helpArray = array();
	
	function TimeIntervalColumn($label, $help=null){
		$this->setType('int');
		$this->label = $label;
		$this->help = $help;
		$this->defaultData = 0;
	}
	
	public function triggerGetJavaScript(){
		head_add_css('include/external/jquery-spinbox/jquery.spinbox.css');
		head_add_js('include/external/jquery-spinbox/jquery.spinbox.js');
		head_add_js('include/javascript/timeinterval.column.js');
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		if(empty($this->data)) $this->data = $this->defaultData;
		list($day, $hour, $minute, $second) = $this->processTimeInterval($this->data);
		$this->renderDay($id, $day); 
		$this->renderHour($id, $hour); 
		$this->renderMinute($id, $minute);
		$this->renderSecond($id, $second);
		$this->help .= '('.join(' : ', $this->helpArray).')';
		$render  = join(' : ', $this->renderArray);
		$render .= $this->renderHidden($id, $day); 
		$render .= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	private function renderHidden($id){
		$this->setFormAttribute('id', $id);
		$this->setFormAttribute('name', $this->name);
		$this->setFormAttribute('class', 'timeinterval_form');
		$this->setFormAttribute('value', $this->data);
		$this->setFormAttribute('type', 'hidden');
		return '<input '.$this->renderFormAttribute().'/>';
	}
	
	private function renderDay($id, $day){
		if($this->isDay){
			$this->setFormAttribute('id', $id.'_day');
			$this->setFormAttribute('size', 3);
			$this->setFormAttribute('name', $this->name.'_day');
			$this->setFormAttribute('class', 'timeinterval_day_form');
			$this->setFormAttribute('value', $day);
			$this->helpArray[] = tt('days');
			$this->renderArray[] = '<input '.$this->renderFormAttribute().'/>';
		}
	}
	
	private function renderHour($id, $hour){
		if($this->isHour){
			$this->setFormAttribute('id', $id.'_hour');
			$this->setFormAttribute('size', 3);
			$this->setFormAttribute('name', $this->name.'_hour');
			$this->setFormAttribute('class', 'timeinterval_hour_form');
			$this->setFormAttribute('value', $hour);
			$this->helpArray[] = tt('hours');
			$this->renderArray[] = '<input '.$this->renderFormAttribute().'/>';
		}
	}
	
	private function renderMinute($id, $minute){
		if($this->isMinute){
			$this->setFormAttribute('id', $id.'_minute');
			$this->setFormAttribute('size', 3);
			$this->setFormAttribute('name', $this->name.'_minute');
			$this->setFormAttribute('class', 'timeinterval_minute_form');
			$this->setFormAttribute('value', $minute);
			$this->helpArray[] = tt('minutes');
			$this->renderArray[] = '<input '.$this->renderFormAttribute().'/>';
		}
	}
	
	private function renderSecond($id, $second){
		if($this->isSecond){
			$this->setFormAttribute('id', $id.'_second');
			$this->setFormAttribute('size', 3);
			$this->setFormAttribute('name', $this->name.'_second');
			$this->setFormAttribute('class', 'timeinterval_second_form');
			$this->setFormAttribute('value', $second);
			$this->helpArray[] = tt('seconds');
			$this->renderArray[] = '<input '.$this->renderFormAttribute().'/>';
		}
	}
	
	private function processTimeInterval($second){
		$day = date('z', $second);
		$hour = date('H', $second%86400);
		$minute = date('i', $second%3600);
		$second = date('s', $second%60);
		return array($day, $hour, $minute, $second);
	}
}
?>
