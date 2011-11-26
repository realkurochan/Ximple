<?php
require_once('CharColumn.class.inc');
class FileColumn extends CharColumn{
	public $isImage = false;
	public $path;
	
	protected $fileArray = array();
	
	private $id;
	
	function FileColumn($label, $path, $help=null){
		CharColumn::CharColumn($label, $help);
		$this->path = ROOT_PATH.$path;
		$this->isOrderable = false;
		$this->setFormAttribute('size', 25);
		$this->setFormAttribute('type', 'file');
	}
	
	public function triggerUpdateForm(){
		$this->data = null;
	}
	
	public function triggerGetID($id){
		$this->id = $id;
	}
	
	public function triggerPostInsert(){
		if($this->isMultipleInsert) $this->multipleInsert();
		else $this->singleInsert();
	}
	
	public function triggerPostUpdate(){
		if($this->isMultipleInsert) $this->multipleUpdate();
		else $this->singleUpdate();
	}
	
	public function triggerDrop(){
		$this->getData();
		if(is_file(ROOT_PATH.$this->dataDB)) unlink(ROOT_PATH.$this->dataDB);
	}
	
	protected function getData(){
		$clause = 'WHERE id='.$this->id;
		$data = db_select_single_column($this->tableName, $this->name, $clause);
		$this->dataDB = $data[$this->id];
	}
	
	private function singleInsert(){
		$file = ff($this->name);
		if($file['size']){
			$this->data = io_upload($file, $this->path, $this->isImage);
		}
	}
	
	private function multipleInsert(){
		$this->fileArray = input_reorder_multiple_files($this->name);
		foreach($this->fileArray as $key => $value){
			if(substr($key,0,2) != 'id'){
				if($value['size']){
					$this->data[] = io_upload($value, $this->path, $this->isImage);
				}else{
					unset($this->fileArray[$key]);
					$this->data[] = null;
				}
			}
		}
	}
	
	private function singleUpdate(){
		$file = ff($this->name);
		$this->getData();
		if($file['size']){
			if(is_file(ROOT_PATH.$this->dataDB)) unlink(ROOT_PATH.$this->dataDB);
			$this->data = io_upload($file, $this->path, $this->isImage);
		}else{
			$this->data = $this->dataDB;
		}
	}
	
	private function multipleUpdate(){
		$this->getMultipleData();
		$this->data = $this->dataDB;
		foreach($this->fileArray as $key => $value){
			if($value['size']){
				if(is_file(ROOT_PATH.$this->dataDB[$key])) unlink(ROOT_PATH.$this->dataDB[$key]);
				$this->data[$key] = io_upload($value, $this->path, $this->isImage);
			}else{
				unset($this->fileArray[$key]);
			}
		}
	}
	
	private function getMultipleData(){
		$fileArray = input_reorder_multiple_files($this->name);
		$this->fileArray = array();
		$idArray = array();
		foreach($fileArray as $id => $file){
			if(substr($id, 0, 2) == 'id'){
				$id = substr($id,3);
				$idArray[] = (int)$id;
				$this->fileArray[(int)$id] = $file;
			}
		}
		if(count($idArray)){
			$clause = 'WHERE id='.join(' OR id=', $idArray);
			$this->dataDB = db_select_single_column($this->tableName, $this->name, $clause);
		}
	}
}
?>
