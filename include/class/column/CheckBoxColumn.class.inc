<?php
require_once('SelectiveColumn.class.inc');
class CheckBoxColumn extends SelectiveColumn{
	function CheckBoxColumn($label, $option, $help=null){
		SelectiveColumn::SelectiveColumn($label, $option, $help);
		$this->setType('array');
	}
	
	public function triggerGetJavaScript(){
		head_add_js('include/javascript/checkbox.column.js');
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->checkSortable();
		$this->divisionClass = 'checkbox_list';
		$this->setFormAttribute('type', 'checkbox');
		$this->setFormAttribute('name', $this->name.'[]');
		$this->setFormAttribute('class', 'checkbox_'.$id);
		$render = SelectiveColumn::renderRawsForm($formID);
		if(count($this->option) > 1){
			$render .= '<p><a href="javascript:checkAllCheckBox(\''.$id.'\')">'.tt('check all').'</a> | ';
			$render .= '<a href="javascript:checkNoneCheckBox(\''.$id.'\')">'.tt('check none').'</a></p>';
		}
		return $render;
	}
	
	protected function getCheck($data){
		if(is_array($this->data) and in_array($data, $this->data)){
			$this->setFormAttribute('checked', 'checked');
		}else{
			$this->setFormAttribute('checked', '');
		}
	}
	
}
?>
