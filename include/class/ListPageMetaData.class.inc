<?php
class ListPageMetaData{
	public $operation = array();
	public $title;
	public $mode = MODE;
	public $page = MODE_PAGE;
	public $description;
	public $writeMode;
	public $writeLabel;
	public $defaultOrder;
	public $defaultOrderDesc = 'id';
	public $groupBy;
	public $column;
	
	private $addtionalParameter = array();
	
	public function addOperation($mode, $label, $icon, $additionParameter=null){
		$this->operation[$mode]['label'] = $label;
		$this->operation[$mode]['addition'] = $additionParameter;
		if(is_file(ROOT_PATH.'files/icon/'.$icon)){
			$this->operation[$mode]['icon'] = 'files/icon/'.$icon;
		}elseif(is_file(ROOT_PATH.'module/'.MODULE.'/theme/'.$icon)){
			$this->operation[$mode]['icon'] = 'module/'.MODULE.'/theme/'.$icon;
		}
	}
	
	public function addParameter($name, $value){
		$this->addtionalParameter[$name] = $value;
	}
	
	public function getParameter(){
		return $this->addtionalParameter;
	}
}
?>
