<?php
require_once(INCLUDE_PATH.'class/column/HiddenColumn.class.inc');

class WidgetPositionColumn extends HiddenColumn{
	function WidgetPositionColumn(){
		HiddenColumn::HiddenColumn('disabled', 'char');
	}
	
	function triggerInsertForm(){
		$position = gg('position');
		if(isset($position) and array_key_exists($position, cc('widget_position'))){
			$this->defaultData = $position;
		}else{
			$this->defaultData = 'disabled';
		}
	}
}
?>
