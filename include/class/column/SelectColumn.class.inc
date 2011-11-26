<?php
require_once('SelectiveColumn.class.inc');
class SelectColumn extends SelectiveColumn{
	function SelectColumn($label, $option, $help=null){
		SelectiveColumn::SelectiveColumn($label, $option, $help);
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('name', $this->name);
		$render = $this->renderSelectForm($formID);
		$render .= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	public function renderGridForm($formID){
		$this->setFormAttribute('name', $this->name.'[]');
		return $this->renderSelectForm($formID);
	}
	
	private function renderSelectForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('id', $id);
		$render = '<select '.$this->renderFormAttribute().'>';
		foreach($this->option as $key => $value){
			$selected = $this->data == $key ? 'selected="selected"' : '';
			$render .= '<option value="'.$key.'" '.$selected.'>'.$value.'</option>';
		}
		$render .= '</select>';
		return $render;
	}
}
?>
