<?php
require_once('CharColumn.class.inc');
class HiddenColumn extends CharColumn{
	function HiddenColumn($default, $type){
		$this->isConfigFormAllowed = false;
		$this->setType($type);
		$this->defaultData = $default;
		$this->data = $default;
		$this->setFormAttribute('type', 'hidden');
		$this->isFormLabel = false;
	}
}
?>
