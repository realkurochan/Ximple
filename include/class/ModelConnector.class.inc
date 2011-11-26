<?php

class ModelConnector{
	private $id;
	private $moduleName;
	private $parentID;
	private $modelObject;
	private $model;
	private $cacheName = array();
	private $cacheAttribute = array();
	
	function ModelConnector($model, $modelObject){
		$this->model = $model;
		$this->modelObject = $modelObject;
	}
	
	public function hasSection(){
		return $this->modelObject->hasSection();
	}
	
	public function setID($id){
		$this->id = $id;
		$this->triggerGetID($id);
	}
	
	public function getID(){
		return $this->id;
	}
	
	public function setModuleName($moduleName){
		$this->moduleName = $moduleName;
	}
	
	public function getModuleName(){
		return $this->moduleName;
	}
	
	public function setAsOverlayGridForm(){
		$this->modelObject->isOverlayGridForm = true;
		foreach($this->model as $key => $modelColumn){
			$modelColumn->isOverlayGridForm = true;
		}
	}
	
	public function setParentID($parentID){
		$this->parentID = $parentID;
	}
	
	public function getColumn($col){
		if(isset($this->model[$col])){
			return $this->model[$col];
		}
	}
	
	public function getParentColumnName(){
		$column = $this->getColumnNameByBoolean('isParent');
		if(count($column)) return $column[0];
		else return null;
	}
	
	public function getColumnName(){
		$col = array('id');
		foreach($this->model as $key => $value){
			if($value->isDB or $value->isLink) $col[] = $key;
		}
		return $col;
	}
	
	public function getColumnNameExcepted($excepted){
		$col = $this->getColumnName();
		foreach($col as $key => $value){
			if(in_array($value, $excepted)) unset($col[$key]);
		}
		return $col;
	}
	
	public function getFormColumn(){
		$model = $this->model;
		foreach($model as $key => $value){
			if(!$value->isForm){
				unset($model[$key]);
			}
		}
		return $model;
	}
	
	public function getTranslateFormColumn(){
		$model = $this->model;
		foreach($model as $key => $value){
			if(!$value->isTranslate){
				unset($model[$key]);
			}
		}
		return $model;
	}
	
	public function setAsConfigForm($col){
		$this->model[$col]->isForm = false;
	}
	
	public function getColumnAttribute($attrName){
		$attr = array();
		foreach($this->model as $key => $value){
			$attr[$key] = $value->$attrName;
		}
		return $attr;
	}
	
	public function resetData(){
		$model = $this->model;
		foreach($model as $key => $value){
			if(!isset($model[$key]->defaultData)){
				$model[$key]->data = '';
			}else{
				$model[$key]->data = $model[$key]->defaultData;
			}
		}
		return $model;
	}
	
	public function addModel($name, $model){
		$this->model[$name] = $model;
		$this->model[$name]->name = $name;
	}
	
	public function addModelAfter($reference, $name, $model){
		if(isset($this->model[$reference])){
			if(!isset($this->model[$name])){
				$model->name = $name;
				$resultModel = array();
				foreach($this->model as $key => $value){
					$resultModel[$key] = $value;
					if($key == $reference){
						$resultModel[$name] = $model;
					}
				}
				$this->model = $resultModel;
			}
		}elseif(state_is_develop()){
			$message = sprintf(tt('%s does not exist (ModelConnector::addModelAfter())'), $reference);
			notify_add_error($message);
		}
	}
	
	public function addModelBefore($reference, $name, $model){
		if(isset($this->model[$reference])){
			if(!isset($this->model[$name])){
				$model->name = $name;
				$resultModel = array();
				foreach($this->model as $key => $value){
					if($key == $reference){
						$resultModel[$name] = $model;
					}
					$resultModel[$key] = $value;
				}
				$this->model = $resultModel;
			}
		}elseif(state_is_develop()){
			$message = sprintf(tt('%s does not exist (ModelConnector::addModelBefore())'), $reference);
			notify_add_error($message);
		}
	}
	
	public function setData($data){
		$model = $this->model;
		foreach($model as $key => $value){
			if(isset($data[$key])){
				$model[$key]->data = $data[$key];
			}
		}
		return $model;
	}
	
	public function setDataDB($data){
		$model = $this->model;
		foreach($model as $key => $value){
			if(isset($data[$key])){
				$model[$key]->dataDB = $data[$key];
			}
		}
		return $model;
	}
	
	public function getData(){
		$data = array();
		foreach($this->model as $key => $value){
			$data[$key] = $value->data;
		}
		return $data;
	}
	
	public function setAttribute($columnName, $attributeName, $attributeValue){
		$this->model[$columnName]->$attributeName = $attributeValue;
	}
	
	public function getAttribute($columnName, $attributeName){
		return $this->model[$columnName]->$attributeName;
	}
	
	public function getColumnNameByBoolean($attrName){
		return $this->getColumnNameByAttribute($attrName, true);
	}
	
	public function getColumnNameByAttribute($attrName, $attr){
		if(isset($this->cacheName[$attrName][$attr])){
			return $this->cacheName[$attrName][$attr];
		}
		
		$column = array();
		foreach($this->model as $key => $value){
			if($value->$attrName == $attr) $column[] = $key;
		}
		$this->cacheName[$attrName][$attr] = $column;
		return $column;
	}
	
	public function getPHPType(){
		$type = array();
		foreach($this->model as $key => $modelColumn){
			$type[$key] = $modelColumn->getPHPType();
		}
		return $type;
	}
	
	public function triggerPreInsert(){
		$this->trigger('PreInsert');
	}
	
	public function triggerPreUpdate(){
		$this->trigger('PreUpdate');
	}
	
	public function triggerPostInsert(){
		$this->trigger('PostInsert');
	}
	
	public function triggerPostUpdate(){
		$this->trigger('PostUpdate');
	}
	
	public function triggerPostDBInsert(){
		$this->trigger('PostDBInsert');
	}
	
	public function triggerPostDBUpdate(){
		$this->trigger('PostDBUpdate');
	}
	
	public function triggerDrop(){
		$this->trigger('Drop');
	}
	
	public function triggerPostDBDrop(){
		$this->trigger('PostDBDrop');
	}
	
	public function triggerInsertForm(){
		$this->trigger('InsertForm');
	}
	
	public function triggerUpdateForm(){
		$this->trigger('UpdateForm');
	}
	
	public function triggerSerialize(){
		$this->trigger('Serialize');
	}
	
	public function triggerUnserialize(){
		$this->trigger('Unserialize');
	}
	
	public function triggerStripTag(){
		$this->trigger('StripTag');
	}
	
	public function triggerCheckNull(){
		$this->trigger('CheckNull');
	}
	
	public function triggerGetJavaScript(){
		$this->trigger('GetJavaScript');
	}
	
	public function triggerSetParent(){
		$this->triggerSingleParameter('SetParent', $this->parentID);
	}
	
	public function triggerGetID($id){
		$this->triggerSingleParameter('GetID', $id);
	}
	
	public function triggerSetTableName($tableName){
		$this->triggerSingleParameter('SetTableName', $tableName);
	}
	
	public function triggerSelect($data, $column=null){
		$triggerColumn = array();
		foreach($this->model as $key => $value){
			$methodName = 'triggerSelect';
			if(method_exists($value, $methodName)) $triggerColumn[] = $key;
		}
		if(is_array($column)){
			$triggerColumn = array_intersect($triggerColumn, $column);
		}
		if(count($triggerColumn)){
			foreach($data as $id => $value){
				$this->triggerGetID($id);
				foreach($triggerColumn as $key => $col){
					if(isset($this->model[$col])){
						$isTrigger = false;
						if(isset($value[$col])){
							$this->model[$col]->data = $value[$col];
							$isTrigger = true;
						}
						$this->model[$col]->triggerSelect();
						if(method_exists($this->model[$col], 'triggerUnserialize')){
							$this->model[$col]->triggerUnserialize();
						}
						if($isTrigger or $this->model[$col]->isLink){
							$data[$id][$col] = $this->model[$col]->data;
						}
					}
				}
			}
		}
		if(method_exists($this->modelObject, 'triggerSelect')){
			$data = $this->modelObject->triggerSelect($data);
		}
		$this->clearCache();
		return $data;
	}
	
	public function checkExist($col){
		return isset($this->model[$col]);
	}
	
	public function hasTranslate(){
		$model = $this->getTranslateFormColumn();
		return (count($model) and is_array(cc('supported_language')));
	}
	
	private function trigger($event){
		$methodName = 'trigger'.$event;
		foreach($this->model as $key => $value){
			if(method_exists($value, $methodName)){
				$value->$methodName();
			}
		}
		if(method_exists($this->modelObject, $methodName)){
			$this->modelObject->$methodName();
		}
		$this->clearCache();
	}
	
	private function triggerSingleParameter($event, $parammeter){
		$methodName = 'trigger'.$event;
		foreach($this->model as $key => $value){
			if(method_exists($value, $methodName)){
				$value->$methodName($parammeter);
			}
		}
		if(method_exists($this->modelObject, $methodName)){
			$this->modelObject->$methodName($parammeter);
		}
		$this->clearCache();
	}
	
	private function clearCache(){
		$this->cacheAttribute = array();
		$this->cacheName = array();
	}
}
?>
