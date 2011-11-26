<?php
class PlainTextColumn extends ModelColumn{
	function PlainTextColumn($label, $help=null){
		$this->setType('shorttext');
		$this->label = $label;
		$this->help = $help;
		$this->isReachable = true;
		$this->isAllowedTranslate = true;
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('name', $this->name);
		$this->setFormAttribute('id', $id);
		$this->setFormAttribute('style', 'width:100%;height:120px;');
		$this->setFormAttribute('rows', 20);
		$this->setFormAttribute('class', 'plaintext');
		$render = '<textarea '.$this->renderFormAttribute().'>'.$this->data.'</textarea>';
		$render.= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	public function renderGridForm($formID){
		$this->setFormAttribute('name', $this->name.'[]');
		$this->setFormAttribute('style', 'width:100%;height:120px;');
		$this->setFormAttribute('rows', 5);
		$render = '<textarea '.$this->renderFormAttribute().'>'.$this->data.'</textarea>';
		return $render;
	}
}
?>
