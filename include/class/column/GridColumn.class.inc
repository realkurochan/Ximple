<?php
require_once(ROOT_PATH.'include/class/ModelFactory.class.inc');
require_once(ROOT_PATH.'include/class/ModelConnector.class.inc');

/// This class is a container for other ModelColumns.
/// It does not represent a column of database table.
class GridColumn extends ModelColumn{
	public $isEmptyForm = false;
	public $isWritable = true;
	public $memberColumn;
	public $dropMode;
	
	private $id = null;
	private $dataModifier = null;
	private $memberDataCount;
	private $modelConnector;
	private $updateData = array();
	private $postData;
	private $childTableName;
	private $hasUpdateData = false;
	
	function GridColumn($label, $help=null){
		$this->setType('array');
		$this->help = $help;
		$this->label = $label;
		$this->isMultipleInsert = true;
		$this->memberColumn = array();
		$this->isDB = false;
	}
	
	public function setModel($modelObj){
		$this->childTableName = $modelObj->tableName;
		$this->dropMode = $modelObj->dropMode;
		$this->isLink = true;
		$this->isDB = false;
		$this->initModelConnector($modelObj);
		$this->mergeModel();
		$this->parentColumn = $this->modelConnector->getParentColumnName();
		return $this;
	}
	
	public function append($name, $memberModel){
		$this->setColumn($name, $memberModel);
	}
	
	public function renderForm($formID){
		$this->getMember();
		if(!$this->isWritable and count($this->data) == 0){
			return null;
		}
		require_once(ROOT_PATH.'include/class/GridFormRender.class.inc');
		$formRender = new GridFormRender();
		$formRender->setGridColumn($this);
		return $formRender->renderForm($formID);
	}
	
	public function triggerSelect(){
		$this->getDBData();
		$this->data = $this->modelConnector->triggerSelect($this->data);
	}
	
	public function triggerPostDBInsert(){
		if($this->isLink){
			$this->initDataModifier();
			$this->dataModifier->prepareInsert();
			$this->modelConnector->triggerSetParent($this->id);
			$this->dataModifier->insert(false);
		}
	}
	
	public function triggerPostDBUpdate(){
		if($this->isLink){
			$this->initDataModifier();
			$this->dataModifier->prepareInsert();
			$this->separateData();
			$this->insertBeforeUpdate();
			
			if($this->hasUpdateData){
				$this->dataModifier->prepareUpdate();
				$this->modelConnector->setData($this->updateData);
				$this->modelConnector->triggerSetParent($this->id);
				$this->dataModifier->update(false);
			}
		}
	}
	
	public function triggerDBDrop(){
		if($this->isLink){
			$clause = 'WHERE '.$this->parentColumn.'='.$this->id;
			db_drop_many($this->childTableName, $clause);
		}
	}
	
	public function triggerGetID($id){
		$this->id = $id;
		if($this->modelConnector) $this->modelConnector->setParentID($id);
	}
	
	private function getMember(){
		if($this->isLink and !empty($this->id)){
			$this->getMemberDBData();
		}else{
			$this->checkMemberData();
			$this->getMemberData();
		}
		return $this->memberColumn;
	}
	
	private function checkMemberData(){
		$dataCount = $this->getDataCount();
		$dataOK = true;
		if(count($dataCount)) $this->checkDataCorrupt($dataCount);
		if($dataOK) $this->memberDataCount = $dataCount[0];
	}
	
	private function getDataCount(){
		$dataCount = array();
		foreach($this->memberColumn as $key => $value){
			if(is_array($value->data)){
				$dataCount[] = count($value->data);
			}else{
				$dataCount[] = null;
			}
		}
		return $dataCount;
	}
	
	private function checkDataCorrupt($dataCount){
		foreach($dataCount as $key => $value){
			if($value != $dataCount[0]){
				notify_add_warning(sprintf(tt('Data for %s is corrupted!'), $this->label));
				$dataOK = false;
				break;
			}
		}
	}
	
	private function getMemberData(){
		$this->data = array();
		for($i=0;$i<$this->memberDataCount;$i++){
			foreach($this->memberColumn as $key => $value){
				if(is_array($value->data)){
					$this->data[$i][$key] = $value->data[$i];
				}
			}
		}
	}
	
	private function getMemberDBData(){
		$this->getDBData();
		foreach($this->data as $id => $data){
			foreach($data as $column => $value){
				if(isset($member[$column])  and $member[$column]->isGridForm){
					$this->memberColumn[$column]->data = $value;
				}
			}
		}
	}
	
	private function getDBData(){
		$clause = 'WHERE '.$this->parentColumn.'='.$this->id;
		$column = $this->modelConnector->getColumnName();
		$member = $this->memberColumn;
		$this->data = db_select($this->childTableName, $column, $clause);
	}
	
	private function initModelConnector($modelObj){
		$modelFactory = new ModelFactory($modelObj);
		$this->modelConnector = $modelFactory->bind();
	}
	
	private function mergeModel(){
		$column = $this->modelConnector->getColumnName();
		foreach($column as $key => $value){
			$columnModel = $this->modelConnector->getColumn($value);
			if(!empty($columnModel)){
				$columnModel->name = $key;
				$this->setColumn($value, $columnModel);
			}
		}
	}
	
	private function setColumn($name, $memberModel){
		$memberModel->isMultipleInsert = true;
		$memberModel->name = $name;
		if($memberModel->isGridForm){
			$memberModel->type = 'array';
		}
		$memberModel->isForm = false;
		$this->memberColumn[$name] = $memberModel;
	}
	
	private function insertBeforeUpdate(){
		if(!empty($this->data)){
			$this->modelConnector->setData($this->data);
			$this->modelConnector->triggerSetParent($this->id);
			$this->dataModifier->insert(false);
		}
	}
	
	private function initDataModifier(){
		if(!$this->dataModifier){
			include_once(ROOT_PATH.'include/class/DataModifier.class.inc');
			$this->dataModifier = new DataModifier($this->childTableName, $this->modelConnector);
		}
	}
	
	private function separateData(){
		$this->postData = $this->modelConnector->getData();
		$this->data = array();
		foreach($this->postData as $name => $data){
			if(is_array($data)){
				$this->setPostData($name, $data);
			}else{
				$this->data[$name] = $data;
				$this->updateData[$name] = $data;
			}
		}
		if(!$this->hasUpdateData) $this->updateData = array(); 
		$this->checkEmpyData();
	}
	
	private function setPostData($name, $data){
		foreach($data as $key => $value){
			if(substr($key,0,2) == 'id'){
				$this->updateData[$name][substr($key, 3)] = $value;
				$this->hasUpdateData = true;
			}else{
				$this->data[$name][] = $value;
			}
		}
	}
	
	private function checkEmpyData(){
		foreach($this->postData as $name => $data){
			if(is_array($data) and !isset($this->data[$name])){
				$this->data = null;
			}
		}
	}
}
?>
