<?php
include('AttributeTypeColumn.class.inc');
include('AttributeValueColumn.class.inc');

class ProductAttributeModel extends Model{
	public $moduleName = 'product';
	public $tableName = 'product_attribute';
	public $dropMode = 'product_drop_attribute';
	
	function ProductAttributeModel($productID=null){
		$this->type = new AttributeTypeColumn();
		$this->value = new AttributeValueColumn();
		$this->product = $this->none($productID, 'int');
		$this->product->isGridForm = false;
		$this->product->isParent = true;
	}
	
	public function triggerSetParent($parentID){
		$this->product->data = $parentID;
	}
}
?>
