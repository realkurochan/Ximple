<?php
include_once('seminar.tool.inc');
class SeminarMapModel extends Model {
	function SeminarMapModel(){
		$this->seminar = $this->hidden('0', 'int');
		$this->attendant = $this->hidden('0', 'int');
	}
}
?>
