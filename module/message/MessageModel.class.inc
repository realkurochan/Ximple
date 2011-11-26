<?php
class MessageModel extends Model{
	function MessageModel(){
		$this->sender = $this->owner();
		$this->receiver = $this->user(tt('send to'));
		$this->receiver->defaultData = gg('send_to');
		$this->sendback = $this->checkbox('', array('sendback' => 'Send me a copy.'));
		$this->sendback->isDB = false;
		$this->message = $this->shorttext(tt('message'));
		$this->write_time = $this->now();
		$this->status = $this->hidden('new');
		$this->section = $this->section();
	}
}
?>
