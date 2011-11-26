<?php

class FormMetaData{
	public $id = '';
	public $description = '';
	public $title;
	public $mode;
	public $configForm;
	public $additionalForm = array();
	public $ajaxFunction = '';
	public $submitLabel;
	public $isCaptcha = false;
	public $isSetTitle = true;
	public $isAjax = false;
	
	function FormMetaData(){
		$this->submitLabel  = tt('submit');
	}
}

?>
