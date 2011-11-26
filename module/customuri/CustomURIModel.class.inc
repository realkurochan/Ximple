<?php
class CustomURIModel extends Model{
	function CustomURIModel(){
		$this->page_name = $this->title(tt('page name'));
		
		$help = tt('e.g. blog/111, gallery/456/Party.html');
		$this->original_URI = $this->title(tt('original URI'), $help);
		$this->original_URI->isList = false;
		
		$help = tt('e.g. Vacation-in-New-York.html, Party.html');
		$this->mapped_URI = $this->title(tt('mapped URI'), $help);
		$this->mapped_URI->isList = false;
		
		$this->section = $this->section();
	}
}
?>
