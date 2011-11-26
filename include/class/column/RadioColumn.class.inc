<?php
require_once('SelectiveColumn.class.inc');
class RadioColumn extends SelectiveColumn{
	function RadioColumn($label, $option, $help=null){
		SelectiveColumn::SelectiveColumn($label, $option, $help);
	}
	
	public function renderForm($formID){
		$this->listClass = 'radio_list';
		$this->setFormAttribute('type', 'radio');
		$this->setFormAttribute('name', $this->name);
		return SelectiveColumn::renderRawsForm($formID);
	}
	
	protected function getCheck($data){
		if($data != $this->data) $this->setFormAttribute('checked', '');
		else $this->setFormAttribute('checked', 'checked');
	}
}
?>
