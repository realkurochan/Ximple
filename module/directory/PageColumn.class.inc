<?php
require_once(INCLUDE_PATH.'class/column/HiddenColumn.class.inc');
class PageColumn extends HiddenColumn{
	function PageColumn(){
		HiddenColumn::HiddenColumn(0, 'int');
	}
	
	public function triggerInsertForm(){
		$this->defaultData = 0;
	}
	
	public function triggerUpdateForm(){
		$this->label = tt('index page');
		$this->form = 'select';
		$this->option = db_select_single_column('page', 'topic', 'WHERE parent='.mid());
	}
}
?>
