<?php
class ShowPageColumn extends CheckBoxColumn{
	function ShowPageColumn(){
		CheckBoxColumn::CheckBoxColumn(tt('select and sort pages shown in list'), array());
		$this->isSortable = true;
	}
	
	function triggerUpdateForm(){
		$this->getSelection();
	}
	
	function getSelection(){
		$selection = array();
		$clause = 'WHERE (parent='.mid().") ";
		$item = db_select_single_column('page', 'topic', $clause);
		foreach($item as $id => $topic){
			$mode = 'page_edit/'.$id.'/parent/'.mid();
			$render  = render_icon($mode, 'files/icon/edit.png', '');
			$mode = 'page_drop/'.$id;
			$render .= render_icon($mode, 'files/icon/drop.png', '');
			$render .= '<a href="'.SECTION_URI.Q.'page/'.$id.'">'.$topic.'</a>';
			$selection[$id] = $render;
		}
		
		if(is_array($this->data)){
			foreach($this->data as $id){
				if(isset($selection[$id])) $this->option[$id] = $selection[$id];
			}
		}
		
		foreach($selection as $key => $value){
			if(!isset($this->option[$key])) $this->option[$key] = $value;
		}
	}
}
?>
