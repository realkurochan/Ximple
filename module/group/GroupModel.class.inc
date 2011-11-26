<?php
class GroupModel extends Model{
	function GroupModel(){
		$this->name = $this->title(tt('name'));
		$this->description = $this->plain(tt('description'));
		$this->description->isList = true;
		$this->description->isReachable = true;
		$this->section = $this->section();
	}
}
?>
