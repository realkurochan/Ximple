<?php
require_once('CharColumn.class.inc');
class EmailColumn extends CharColumn{
	function EmailColumn($label, $help=null){
		CharColumn::CharColumn($label, $help);
		$this->isNotNull = true;
	}
	
	public function triggerGetJavaScript(){
		head_add_js_var('emailNotOk', tt('Your Email address is not conformed.'));
		head_add_js('include/javascript/email.column.js');
	}
	
	public function renderForm($formID){
		$this->setFormAttribute('class', 'email_field');
		return CharColumn::renderForm($formID);
	}
}
?>
