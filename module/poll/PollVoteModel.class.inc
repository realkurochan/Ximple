<?php
class PollVoteModel extends Model{
	public $tableName = 'poll_vote';
	
	function PollVoteModel(){
		$this->poll = $this->none(null, 'int');
		$this->voter = $this->owner();
		$this->voter_ip = $this->none(null, 'char');
	}
}
?>
