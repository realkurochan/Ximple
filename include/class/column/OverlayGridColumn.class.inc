<?php
class OverlayGridColumn extends ModelColumn{
	public $childModuleName;
	public $childTableName;
	public $parentColumn;
	
	private $id;
	private $modelConnector;
	
	function OverlayGridColumn($label, $childModel, $help=null){
		$this->setType('array');
		$this->label = $label;
		
		$modelFactory = new ModelFactory($childModel);
		$this->modelConnector = $modelFactory->bind();
		$this->modelConnector->triggerGetJavaScript();
		
		$this->childModuleName = $childModel->fullModuleName;
		$this->childTableName = $childModel->tableName;
		$this->parentColumn = $this->modelConnector->getParentColumnName();
		
		$this->help = $help;
		$this->isMultipleInsert = true;
		$this->memberColumn = array();
		$this->isDB = false;
		$this->isLink = true;
	}
	
	public function setChildModule(){
	
	}
	
	public function triggerGetJavaScript(){
		head_add_js('include/javascript/overlaygrid.column.js');
	}
	
	public function triggerPostDBInsert(){
		if($this->isLink){
			$table = $this->childTableName;
			$inputName = 'overlay_grid_'.$table;
			$input = pp($inputName);
			if(is_array($input)){
				foreach($input as $key => $value){
					$data = array(
						$this->parentColumn => $this->id,
					);
					db_update($this->childTableName, $data, $value);
				}
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
	}
	
	public function renderForm($formID){
		head_add_css('list.css');
		head_add_js_var('overlayParentID', $this->id);
		$renderArray = array(
			'form_id' => $formID,
			'id' => $this->id,
			'module_name' => $this->childModuleName,
		);
		return render($renderArray, 'overlay_form.tpl', 'overlay_form');
	}
}
?>
