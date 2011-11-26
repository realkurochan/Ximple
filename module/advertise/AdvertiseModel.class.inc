<?php
class AdvertiseModel extends Model{
	function AdvertiseModel(){
		$this->name = $this->title('name');
		$this->name->listMode = 'advertise';
		$this->uri = $this->title('URI');
		$this->uri->isList = false;
		$this->address = $this->title(tt('address'));
		$this->banner = $this->image(tt('banner'), 'files/advertise/');
		$option = cc('advertise_widget_position');
		if(!is_array($option)) $option = array();
		$this->position = $this->select(tt('position'), $option);
		$this->description = $this->shortText(tt('advertising desciption'));
		$this->customer = $this->shortText(tt('customer description'));
		$this->section = $this->section();
	}
}
?>
