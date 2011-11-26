<?php
require_once(INCLUDE_PATH.'class/column/SelectColumn.class.inc');

class MenuWidgetColumn extends SelectColumn{
	function MenuWidgetColumn(){
		SelectColumn::SelectColumn(tt('includes Widget'), array());
		$this->setType('int');
		$p = tt('Use <a href="%s">widget interface</a> for Widget configuration.');
		$this->help = sprintf($p, SECTION_URI.Q.'widget');
	}
	
	public function renderForm($formID){
		if(cc('menu_include_widget')){
			return SelectColumn::renderForm($formID);
		}else{
			return $this->renderAsHidden($formID);
		}
	}
	
	public function triggerInsertForm(){
		$this->getWidget();
	}
	
	public function triggerUpdateForm(){
		$this->getWidget();
	}
	
	private function getWidget(){
		if(cc('menu_include_widget')){
			$this->option = db_select_single_column('widget', 'title', 'WHERE section='.SECTION, true);
			$this->option[0] = cc('None');
			ksort($this->option);
		}
	}
	
	private function renderAsHidden($formID){
		require_once(INCLUDE_PATH.'class/column/HiddenColumn.class.inc');
		$hidden = new HiddenColumn($this->data, 'int');
		return $hidden->renderForm($formID);
	}
}
?>
