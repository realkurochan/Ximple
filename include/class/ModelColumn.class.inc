<?php
abstract class ModelColumn{
	private $formAttribute = array();
	private $typeMap = array(
		'int' => 'integer',
		'real' => 'float',
		'char' => 'string',
		'shorttext' => 'string',
		'text' => 'string',
		'date' => 'string',
		'timestamp' => 'string',
		'array' => 'array',
	);
	
	protected $isAllowedTranslate = false;
	
	public $type;
	public $name;
	public $label;
	public $help;
	public $data;
	public $dataDB;
	public $defaultData;
	public $listMode;
	public $tableName;
	
	public $isDB = true;
	public $isLink = false;
	public $isRepresentative = false;
	public $isForm = true;
	public $isOverlayGridForm = false;
	public $isConfigFormAllowed = true;
	public $isGridForm = true;
	public $isFormLabel = true;
	public $isNotNull = false;
	public $isList = false;
	public $isReachable = false;
	public $isOrderable = true;
	public $isTranslate = false;
	public $isMultipleInsert = false;
	public $isParent = false;
	public $isUser = false;
	public $isSerialize = false;
	
	public abstract function renderForm($formID);
	
	public function enableTranslate(){
		if($this->isAllowedTranslate){
			$this->isTranslate = true;
		}elseif(state_is_develop()){
			$pattern = tt('%s is not allowed to be translated.');
			notify_add_error(sprintf($pattern, $this->name));
		}
	}
	
	public function check(){
	}
	
	public function setFormAttribute($name, $value){
		$this->formAttribute[$name] = $value;
	}
	
	public function unsetFormAttribute($name){
		unset($this->formAttribute[$name]);
	}
	
	public function resetFormAttribute(){
		$this->formAttribute = array();
	}
	
	public function getPHPType(){
		return $this->typeMap[$this->type];
	}
	
	public function setType($type){
		$allowedType = array_keys($this->typeMap);
		if(in_array($type, $allowedType)){
			$this->type = $type;
		}elseif(state_is_develop()){
			notify_add_error('unkown type : '.$type.' ('.$this->name.')');
		}
	}
	
	public function getType(){
		return $type;
	}
	
	public function triggerStripTag(){
		if(is_string($this->data)){
			$this->data = input_strip_tags($this->data, cc('allowed_tags'));
		}
	}
	
	public function triggerCheckNull(){
		if($this->isNotNull and $this->data === null){
			error_syslog(sprintf(tt('Data in column %s is not allowed be empty.'), $this->name));
		}
	}
	
	public function triggerSetTableName($tableName){
		$this->tableName = $tableName;
	}
	
	protected function renderFormAttribute(){
		$render = '';
		foreach($this->formAttribute as $key => $value){
			if(strlen($value)) $render .= $key.'="'.$value.'" ';
		}
		return $render;
	}
}
?>
