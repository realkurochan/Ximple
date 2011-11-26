<?php
class ProductFileModel extends Model{
	public $moduleName = 'product';
	public $tableName = 'product_file';
	public $dropMode = 'product_drop_file';
	
	function PeopleFileModel($productID=null){
		$this->short_description = $this->char(tt('short description'));
		$this->file = $this->file(tt('file'), 'files/product/'.SECTION.'/');
		$this->type = $this->hidden('', 'char');
		$this->product = $this->none($productID, 'int');
		$this->product->isGridForm = false;
		$this->product->isParent = true;
	}
	
	public function triggerSetParent($parentID){
		$this->product->data = $parentID;
	}
	
	public function triggerPostInsert(){
		$this->getType();
	}
	
	public function triggerPostUpdate(){
		$this->getType();
	}
	
	private function getType(){
		$this->type->data = array();
		if(is_array($this->file->data)){
			foreach($this->file->data as $key => $value){
				if(is_file($value)){
					$this->type->data[$key] = mime_content_type($value);
				}
			}
		}
	}
}
?>
