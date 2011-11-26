<?php
class ShowDirectoryColumn extends CheckBoxColumn{
	function ShowDirectoryColumn(){
		CheckBoxColumn::CheckBoxColumn(tt('select and sort sub-directories shown in list'), array());
		$this->isSortable = true;
	}
	
	function triggerUpdateForm(){
		$this->getSelection();
	}
	
	function getSelection(){
		$selection = array();
		$clause = 'WHERE (parent='.mid().") ";
		$item = db_select_single_column('directory', 'name', $clause);
		foreach($item as $id => $topic){
			$mode = 'directory_edit/'.$id.'/parent/'.mid();
			$render  = render_icon($mode, 'files/icon/edit.png', '');
			$mode = 'directory_drop/'.$id;
			$render .= render_icon($mode, 'files/icon/drop.png', '');
			$render .= '<a href="'.SECTION_URI.Q.'directory/'.$id.'">'.$topic.'</a>';
			$selection[$id] = $render;
		}
		
		if(is_array($this->data)){
			foreach($this->data as $id){
				$this->option[$id] = $selection[$id];
			}
		}
		
		foreach($selection as $key => $value){
			if(!isset($this->option[$key])) $this->option[$key] = $value;
		}
	}
}
?>
