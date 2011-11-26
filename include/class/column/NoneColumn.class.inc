<?php
class NoneColumn extends ModelColumn{
	function NoneColumn($default=null, $type='char'){
		$this->isConfigFormAllowed = false;
		$this->setType($type);
		$this->defaultData = $default;
		$this->data = $default;
		$this->isGridForm = false;
		$this->isForm = false;
	}
	
	function renderForm($formID){
		return null;
	}
}
?>
