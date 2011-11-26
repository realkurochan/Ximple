<?php
require('MenuWidgetColumn.class.inc');
class MenuModel extends Model{
	function MenuModel(){
		$this->title = $this->title(tt('menu title'));
		$this->title->enableTranslate();
		$this->uri = $this->char(tt('URI'), tt('Absolute URI e.g. http://www.wikipedia.org or Ximple internal URI e.g. blog/111.'));
		$this->uri->isNotNull = true;
		$this->widget = new MenuWidgetColumn();
		$this->sort_number = $this->hidden(0, 'int');
		$this->section = $this->section();
	}
}
?>
