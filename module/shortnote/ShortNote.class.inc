<?php
execute_add_class('shortnote', 'ShortNote');
include('ShortNoteModel.class.inc');

class ShortNote extends Control{
	function ShortNote(){
		$this->moduleTitle = 'Short Note';
		$this->moduleName = 'shortnote';
		$this->tableName = 'note';
		$this->ownerColumn = 'owner';
		$this->addAccessMethod('user', 'index');
		$this->addAccessMethod('user', 'write');
		$this->addAccessMethod('user', 'insert');
		$this->insertRedirection = 'shortnote#top_note';
		$this->updateRedirection = 'shortnote#note_'.mid();
		$this->init(new ShortNoteModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		head_add_css('note.css');
		$this->select($this->column, 'WHERE owner='.USER_ID.' ORDER BY id DESC');
		return render(array('data' => $this->data), 'note.tpl', 'shortnote');
	}
}
?>
