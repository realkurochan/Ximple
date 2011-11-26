<?php
require_once('ModelFactory.class.inc');
require_once('TableCreator.class.inc');

class QuickTableCreator{
	private $moduleName;
	private $modelFactory;
	private $modelConnector;
	private $tableCreator;
	
	function QuickTableCreator($model){
		$this->modelFactory = new ModelFactory($model);
		$this->modelConnector = $this->modelFactory->bind();
		$this->tableCreator = new TableCreator($this->modelConnector, $model->tableName);
	}
	
	public function create(){
		$this->tableCreator->create();
	}
}

?>
