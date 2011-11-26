<?php
class ProfileTypeModel extends Model{
	function ProfileTypeModel(){
		$this->name = $this->title(tt('type name'));
		$this->description = $this->shortText(tt('description'));
		$this->description->isList = true;
		$this->description->isReachable = true;
		$this->allowed_group = $this->group(tt('allowed group'));
		$this->allowed_group->isExceptedGuest = true;
		$this->section = $this->section();
	}
}
?>
