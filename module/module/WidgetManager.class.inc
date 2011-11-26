<?php
require_once(INCLUDE_PATH.'tool/widget.tool.inc');

class WidgetManager{
	private $title;
	private $mode;
	private $position = 'disabled';
	private $formMode;
	private $saveMode;
	private $sortNumber;
	
	function WidgetManager($title, $mode){
		$this->title = $title;
		$this->mode = $mode;
	}
	
	public function setPosition($position){
		$this->position = $position;
	}
	
	public function setFormMode($formMode){
		$this->formMode = $formMode;
	}
	
	public function setSaveMode($saveMode){
		$this->saveMode = $saveMode;
	}
	
	public function setSortNumber($sortNumber){
		$this->sortNumber = $sortNumber;
	}
	
	public function submit(){
		widget_add($this->title, $this->mode, $this->position, $this->formMode, $this->saveMode, $this->sortNumber);
	}
	
	public function drop(){
		widget_drop($this->mode);
	}
}
?>
