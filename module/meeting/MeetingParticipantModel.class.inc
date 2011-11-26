<?php
class MeetingParticipantModel extends ContentModel{
	public $tableName = 'meeting_participant';
	function MeetingParticipantModel(){
		$this->participant = $this->owner();
		$this->meeting = $this->none(null, 'int');
		$this->status = $this->none(null, 'int');
	}
}
?>
