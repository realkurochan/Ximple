<?php
class GuestbookModel extends Model{
	function GuestbookModel(){
		$this->writer = $this->title(tt('your name'));
		$this->email = $this->email(tt('your email (not show)'));
		$this->email->isList = true;
		$this->write_time = $this->now(cc('time_format'));
		$this->guestbook = $this->shortText(tt('your opinion'));
		$this->section = $this->section();
	}
}
?>
