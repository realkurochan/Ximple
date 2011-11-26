<?php

include_once(INCLUDE_PATH.'class/Model.class.inc');

class FlexiFunConfigModel extends Model{
	function FlexiFunConfigModel(){
		$path = FILES_PATH.'customtheme/'.SECTION.'/'.cc('theme').'/image';
		$layoutOption = array(
			'fluid' => 'Fluid width',
			'fixed' => 'Fexed width',
			'fluid_head' => 'Fixed width with fluid head'
		);
		$this->layout = $this->select(tt('layout'), $layoutOption);
		
		$columnOption = array(
			2 => '2 Columns',
			3 => '3 Columns',
		);
		$this->column = $this->select(tt('columns number'), $columnOption);
		
		$this->body_background_color = $this->color(tt('body background color'));
		$this->body_background_image = $this->file('body background image', $path);
		$this->content_background_color = $this->color(tt('content background color'));
		$this->border_color = $this->color(tt('content border color'));
		
	}
}

?>
