<?php
class DataModifier{
	private $id;
	private $table;
	private $modelConnector;
	private $data;
	private $columnDB;
	private $fileColumn;
	private $noneColumn;
	private $multipleInsertColumn;

	function DataModifier($table, $modelConnector){
		$this->table = $table;
		$this->modelConnector = $modelConnector;
	}
	
	public function getID(){
		return $this->id;
	}
	
	public function prepareInsert(){
		$this->modelConnector->triggerPreInsert();
		$this->columnDB = $this->modelConnector->getColumnNameByBoolean('isDB');
		$this->prepare();
		$this->modelConnector->triggerPostInsert();
		$this->modelConnector->triggerCheckNull();
	}
	
	public function insert($isCaptcha){
		$retult = false;
		$this->data = $this->modelConnector->getData();
		if(!$isCaptcha or $this->checkCaptcha()){
			$this->getMultipleInsertColumn();
			if(count($this->multipleInsertColumn)){
				$retult = $this->insertMultipleRows();
			}else{
				$retult = $this->insertSingleRow();
			}
			$this->modelConnector->triggerPostDBInsert();
			extension_execute('insert', $this->id);
		}
		return $retult;
	}
	
	public function prepareUpdate($data){
		$this->modelConnector->setDataDB($data);
		$this->checkID();
		$this->modelConnector->setID($this->id);
		$this->modelConnector->triggerPreUpdate();
		$this->columnDB = $this->modelConnector->getColumnNameByBoolean('isDB');
		$this->prepare();
		$this->modelConnector->triggerPostUpdate();
		$this->modelConnector->triggerCheckNull();
	}
	
	public function update($isCaptcha){
		$retult = false;
		if(!$isCaptcha or $this->checkCaptcha()){
			$this->getMultipleInsertColumn();
			if(count($this->multipleInsertColumn)){
				$retult = $this->updateMultipleRows();
			}else{
				$retult = $this->updateSingleRow();
			}
			$this->modelConnector->triggerPostDBUpdate();
			extension_execute('update', $this->id);
		}
		return $retult;
	}
	
	public function prepareDrop(){
		$this->columnDB = $this->modelConnector->getColumnNameByBoolean('isDB');
		$this->checkID();
		$this->modelConnector->setID($this->id);
		$this->modelConnector->triggerDrop();
	}
	
	public function drop(){
		db_drop($this->table, $this->id);
		$this->modelConnector->setID($this->id);
		$this->modelConnector->triggerPostDBDrop();
		extension_execute('drop', $this->id);
	}
	
	public function checkCaptcha(){
		if(authority_is_guest()){
			include_once(INCLUDE_PATH.'tool/captcha.tool.inc');
			$gentime = pp('captcha_gentime_captcha');
			$parity = pp('captcha_code_captcha');
			$code = pp('captcha');
			if(!captcha_check($gentime, $code, $parity)){
				notify_add_error(tt('Your given code from the image was wrong. Data is not inserted.'));
				return false;
			}
		}
		return true;
	}
	
	private function prepare(){
		$this->getData();
		$this->modelConnector->setData($this->data);
		$this->modelConnector->triggerSerialize();
		$this->modelConnector->triggerStripTag();
		$this->data = $this->modelConnector->getData();
	}
	
	private function getData(){
		$type = $this->modelConnector->getPHPType();
		foreach($this->columnDB as $name){
			$data = pp($name);
			settype($data, $type[$name]);
			$this->data[$name] = $data;
		}
	}
	
	private function checkID(){
		$this->id = (int) gg('id');
		if(empty($this->id)){
			error_syslog(tt('Data can not be modified without any given ID.'));
		}
	}
	
	private function getDataFromConnector(){
		$this->data = $this->modelConnector->getData();
		foreach($this->data as $key => $value){
			if(!in_array($key, $this->columnDB)) unset($this->data[$key]);
		}
	}
	
	private function getMultipleInsertColumn(){
		$this->multipleInsertColumn = $this->modelConnector->getColumnNameByBoolean('isMultipleInsert');
		$this->multipleInsertColumn = array_intersect($this->multipleInsertColumn, $this->columnDB);
	}
	
	private function insertSingleRow(){
		$this->getDataFromConnector();
		$this->id = db_insert($this->table, $this->data);
		$this->modelConnector->setID($this->id);
		return true;
	}
	
	private function insertMultipleRows(){
		$this->getDataFromConnector();
		$this->formatMultipleData();
		$this->id = array();
		foreach($this->data as $key => $value){
			$this->modelConnector->setData($value);
			$this->modelConnector->triggerCheckNull();
			$this->id[] = db_insert($this->table, $value);
		}
		return true;
	}
	
	private function updateSingleRow(){
		$this->getDataFromConnector();
		db_update($this->table, $this->data, $this->id);
		return true;
	}
	
	private function updateMultipleRows(){
		$this->getDataFromConnector();
		$this->formatMultipleData();
		if(!is_array($this->id)) $this->id = array();
		foreach($this->data as $key => $value){
			$this->modelConnector->setData($value);
			$this->modelConnector->triggerCheckNull();
			$this->id[] = $key;
			db_update($this->table, $value, $key);
		}
		return true;
	}
	
	private function formatMultipleData(){
		$formattedData = array();
		$dataKey = array_keys($this->data[$this->multipleInsertColumn[0]]);
		foreach($dataKey as $i){
			foreach($this->data as $key => $value){
				if(is_array($value)){
					$formattedData[$i][$key] = $value[$i];
				}else{
					$formattedData[$i][$key] = $value;
				}
			}
		}
		$this->data = $formattedData;
	}
}
?>
