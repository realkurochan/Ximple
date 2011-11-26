<?php
class ModelFactory{
	private $model = array();
	private $modelObject;
	private $isConvert = false;
	
	function ModelFactory($modelObject){
		$this->modelObject = $modelObject;
	}
	
	public function getModelArray(){
		$this->convertToArray();
		return $this->model;
	}
	
	public function bind(){
		$this->convertToArray();
		$connector = new ModelConnector($this->model, $this->modelObject);
		$connector->triggerSetTableName($this->modelObject->tableName);
		return $connector;
	}
	
	private function convertToArray(){
		if(!$this->isConvert){
			foreach($this->modelObject as $key => $value){
				$beforeColumnName = $this->modelObject->getExtendedBefore($key);
				if(is_array($beforeColumnName)){
					foreach($beforeColumnName as $column){
						$this->model[$column] = $this->modelObject->$column;
						$this->model[$column]->name = $column;
					}
				}
				if(is_a($value, 'ModelColumn') and empty($this->model[$key])){
					$this->model[$key] = $value;
					$this->model[$key]->name = $key;
				}
				$afterColumnName = $this->modelObject->getExtendedAfter($key);
				if(is_array($afterColumnName)){
					foreach($afterColumnName as $column){
						$this->model[$column] = $this->modelObject->$column;
						$this->model[$column]->name = $column;
					}
				}
			}
			$this->check();
			$this->isConvert = true;
		}
	}
	
	private function check(){
		foreach($this->model as $name => $col){
			$col->check();
		}
	}
}
?>
