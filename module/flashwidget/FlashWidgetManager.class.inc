<?php
require_once('flashwidget.module.inc');

class FlashWidgetManager{
	private $reference;
	private $mode;
	private $name;
	private $table;
	private $column;
	private $clause;
	
	function FlashWidgetManager($reference){
		$this->reference = $reference;
	}
	
	public function setReference($reference){
		$this->reference = $reference;
	}
	
	public function setMode($mode){
		$this->mode = $mode;
	}
	
	public function setName($name){
		$this->name = $name;
	}
	
	public function setTable($table){
		$this->table = $table;
	}
	
	public function setColumn($column){
		$this->column = $column;
	}
	
	public function setClause($clause){
		$this->clause = $clause;
	}
	
	public function submit(){
		flashwidget_add($this->reference, $this->mode, $this->name, $this->table, $this->column, $this->clause);
	}
	
	public function drop(){
		flashwidget_drop($this->reference);
	}
}
?>
