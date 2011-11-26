<?php
require_once('CharColumn.class.inc');
class ColorColumn extends CharColumn{
	function ColorColumn($label, $help=null){
		CharColumn::CharColumn($label, $help);
		$this->defaultData = "#5555ee";
		$this->data = "#5555ee";
	}
	
	public function renderForm($formID){
		$this->setFormAttribute('class', 'color_picker');
		$id = $formID.'_'.$this->name;
		$render  = '<div id="picker_'.$id.'"></div>';
		$render .= CharColumn::renderForm($formID);
		$render .= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	public function renderGridForm($formID){
		$this->initJS();
		$this->setFormAttribute('class', 'grid_color_picker');
		return CharColumn::renderGridForm($formID);
	}
	
	public function triggerGetJavaScript(){
		head_add_css('include/external/farbtastic/farbtastic.css');
		head_add_js('include/external/farbtastic/farbtastic.js');
		head_add_js('include/javascript/color.column.js');
		head_add_js_var('selectColorLabel', tt('Select Color'));
	}
}
?>
