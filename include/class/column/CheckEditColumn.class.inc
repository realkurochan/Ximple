<?php
require_once('SelectiveColumn.class.inc');
class CheckEditColumn extends SelectiveColumn{
	private $editable;
	
	function CheckEditColumn($label, $option, $help=null){
		SelectiveColumn::SelectiveColumn($label, $option, $help);
		$this->setType('array');
	}
	
	public function triggerGetJavaScript(){
		head_add_js('include/javascript/checkedit.column.js');
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->checkSortable();
		$render = '<div class="checkedit_list">';
		$render.= '<div class="'.$this->listClass.'" id="'.$id.'">';
		$i = 0;
		foreach($this->option as $key => $value){
			$render .= '<div class="sortable_item">'.$this->imageTag;
			
			$this->resetFormAttribute();
			$this->getCheck($key);
			$this->setCheckBoxAttribute($id.$i);
			$render .= '<input '.$this->renderFormAttribute().'/>';
			
			$this->resetFormAttribute();
			$this->setInputAttribute($id.$i, $key, $value);
			$render .= '<input '.$this->renderFormAttribute().'/>';
			
			$render .= '</div>';
			$i++;
		}
		$render .= '</div>';
		$render .= '</div>';
		$render .= '<span id="info_'.$id.'"></span>';
		$render .= '<p style="clear:both;">';
		return $render;
	}
	
	protected function getCheck($data){
		if(is_array($this->data) and in_array($data, $this->data)){
			$this->setFormAttribute('checked', 'checked');
			$this->editable = '';
		}else{
			$this->setFormAttribute('checked', '');
			$this->editable = 'disabled';
		}
	}
	
	private function setCheckBoxAttribute($relation){
		$this->setFormAttribute('type', 'checkbox');
		$this->setFormAttribute('name', 'check_'.$this->name.'[]');
		$this->setFormAttribute('class', 'checkedit');
		$this->setFormAttribute('rel', $relation);
		$this->setFormAttribute('style', 'border:none;');
	}
	
	private function setInputAttribute($id, $data, $label){
		$this->setFormAttribute('id', $id);
		$this->setFormAttribute('name', $this->name.'['.$data.']');
		$this->setFormAttribute('disabled', $this->editable);
		$this->setFormAttribute('value', $label);
		$this->setFormAttribute('size', 25);
	}
}
?>
