<?php
require_once('CharColumn.class.inc');
class PasswordColumn extends CharColumn{
	function PasswordColumn($label, $help=null){
		CharColumn::CharColumn($label, $help);
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('type', 'password');
		$this->setFormAttribute('name', $this->name);
		$this->setFormAttribute('id', $id);
		$this->unsetFormAttribute('value');
		$render = '<input '.$this->renderFormAttribute().'/>';
		$render.= '<span id="info_'.$id.'"></span>';
		return $render;
	}
}
?>
