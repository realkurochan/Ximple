<?php
class CommandDocumentModel extends Model{
	function CommandDocumentModel(){
		$this->owner = $this->owner();
		$this->sort_number = $this->hidden(0, 'int');
		
		$this->command = $this->title(tt('คำสั่ง'));
		$this->command->isList = false;
		
		$this->at = $this->title(tt('ที่'));
		$this->at->isList = false;
		
		$this->subject = $this->title(tt('เรื่อง'));
		$this->subject->listMode = 'documentgenerator_commanddocument';
		
		$this->detail = $this->plain(tt('ข้อความ'));
		$this->detail->isList = false;
		
		$this->since = $this->title(tt('ตั้งแต่'));
		$this->since->isList = false;
		
		$this->month = $this->title(tt('เดือน'));
		$this->month->isList = false;
		
		$this->year = $this->title(tt('ปี'));
		$this->year->isList = false;
		
		$this->rank = $this->title(tt('ยศ'));
		$this->rank->isList = false;
		
		$this->name = $this->title(tt('ชื่อ'));
		$this->name->isList = false;
		
		$this->surname = $this->title(tt('สกุล'));
		$this->surname->isList = false;
		
		$this->position = $this->title(tt('ตำแหน่ง'));
		$this->position->isList = false;
		
		$this->category = $this->category('commanddocument');
		
		$option = array(
			0 => tt('draft'),
			1 => tt('publish'),
		);
		$this->status = $this->select(tt('status'), $option);
		
		$this->section = $this->section();
	}
}
?>
