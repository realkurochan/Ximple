<?php
class ShowItemColumn extends CheckBoxColumn{
	function ShowItemColumn(){
		CheckBoxColumn::CheckBoxColumn(tt('shown item by rendered directory'), array());
		$this->isSortable = true;
	}
	
	function triggerUpdateForm(){
		$this->getSelection();
	}
	
	function getSelection(){
		$option = directory_show_item_option();
		if(is_array($this->data)){
			foreach($this->data as $key){
				$this->option[$key] = $option[$key];
			}
		}
		
		foreach($option as $key => $value){
			if(!isset($this->option[$key])) $this->option[$key] = $value;
		}
	}
}
?>
