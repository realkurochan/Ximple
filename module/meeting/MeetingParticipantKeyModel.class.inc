<?php
class MeetingParticipantKeyModel extends ContentModel{
	public $tableName = 'meeting_key';
	function MeetingParticipantKeyModel(){
		$this->participant = $this->owner();
		$this->meeting = $this->hidden(null, 'int');
		$this->secure_key = $this->hidden(null, 'char');
		$this->status = $this->hidden(null, 'int');
	}
}
?>
