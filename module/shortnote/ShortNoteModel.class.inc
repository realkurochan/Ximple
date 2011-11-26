<?php
class ShortNoteModel extends Model{
	function ShortNoteModel(){
		$this->note = $this->shortText('');
		$this->write_time = $this->now();
		$this->owner = $this->owner();
	}
}
?>
