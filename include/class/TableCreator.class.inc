<?php
class TableCreator{
	private $tableName;
	private $modelConnector;
	
	function TableCreator($modelConnector, $tableName){
		$this->tableName = $tableName;
		$this->modelConnector = $modelConnector;
	}
	
	public function create(){
		if(!db_exist_table($this->tableName)){
			$query = $this->getSQL();
			db_create_table($this->tableName, $query);
		}
	}
	
	private function getSQL(){
		$query = array();
		$columnName = $this->modelConnector->getColumnNameByBoolean('isDB');
		$type = $this->modelConnector->getColumnAttribute('type');
		$isNotNull = $this->modelConnector->getColumnAttribute('isNotNull');
		$default = $this->modelConnector->getColumnAttribute('defaultData');
		foreach($columnName as $name){
			$q = $name.' '.db_get_type($type[$name]);
			if($isNotNull[$name]) $q .= ' NOT NULL ';
			if(!empty($default[$name]) and !in_array($type[$name], array('text', 'shorttext', 'array'))){
				$q .= " DEFAULT '".$default[$name]."' ";
			}
			$query[] = $q;
		}
		return $query;
	}
}
?>
