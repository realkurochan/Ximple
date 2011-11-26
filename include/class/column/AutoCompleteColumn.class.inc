<?php
require_once('CharColumn.class.inc');
class AutoCompleteColumn extends CharColumn{
	private $ajaxMode;
	
	function AutoCompleteColumn($label, $ajaxMode, $help=null){
		CharColumn::CharColumn($label, $help);
		$this->ajaxMode = $ajaxMode;
	}
	
	public function triggerGetJavaScript(){
		head_add_js('include/javascript/autocomplete.column.js');
	}
	
	public function renderForm($formID){
		$this->setFormAttribute('alt', $this->ajaxMode);
		$this->setFormAttribute('class', 'auto_complete');
		return CharColumn::renderForm($formID);
	}
}
?>
