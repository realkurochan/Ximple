<?php
class CharColumn extends ModelColumn{
	function CharColumn($label, $help=null){
		$this->setType('char');
		$this->label = $label;
		$this->help = $help;
		$this->isAllowedTranslate = true;
		$this->setFormAttribute('size', 25);
		$this->setFormAttribute('maxlength', 80);
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('name', $this->name);
		$this->setFormAttribute('id', $id);
		$this->setFormAttribute('value', $this->data);
		$render = '<input '.$this->renderFormAttribute().'/>';
		$render.= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	public function renderGridForm($formID){
		$this->setFormAttribute('name', $this->name.'[]');
		$this->setFormAttribute('value', $this->data);
		return '<input '.$this->renderFormAttribute().'/>';
	}
}
?>
