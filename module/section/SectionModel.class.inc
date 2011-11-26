<?php
class SectionModel extends Model{
	function SectionModel(){
		$this->name = $this->title(tt('section name'));
		$this->name->help = tt('Section name must be longer than 4 characters and unique');
		$this->name->listMode = 'section_redirect';
		$this->uri = $this->title(tt('section URI'));
		$this->uri->isList = false;
		$this->uri->help = tt('For example : http://mydomain.com/. The last character must be /.');
		$this->path = $this->title(tt('section path'));
		$this->path->help  = tt('Section path must be longer than 4 characters, unique and consists of a-z, A-Z, 0-9, _, -. ');
		if(MODE_ID == 1){
			$this->path->isNotNull = false;
			$this->path->form = 'hidden';
			$this->path->label = null;
			$this->path->help = null;
		}
	}
}
?>
