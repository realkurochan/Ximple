<?php
class PollChoiseModel extends Model{
	public $moduleName = 'poll';
	public $tableName = 'poll_choise';
	public $dropMode = 'poll_choise_drop';
	
	function PollChoiseModel($pollID=null){
		$this->choise = $this->char(tt('choise'));
		$this->voted_number = $this->hidden(0, 'int');
		$this->voted_number->defaultData = 0;
		$this->poll = $this->none($pollID, 'int');
		$this->poll->isGridForm = false;
		$this->poll->isParent = true;
	}
	
	public function triggerSetParent($parentID){
		$this->poll->data = $parentID;
	}
}
?>
