<?php
include('AttributeFormTypeColumn.class.inc');

class ProductAttributeTypeModel extends Model{
	public $moduleName = 'product';
	public $tableName = 'product_attribute_type';
	public $dropMode = 'product_type_drop_atrribute_type';
	
	function ProductAttributeTypeModel($typeID=null){
		$this->name = $this->title('attribute type');
		$this->form_type = new AttributeFormTypeColumn();
		$this->product_type = $this->none($typeID, 'int');
		$this->product_type->isGridForm = false;
		$this->product_type->isParent = true;
	}
	
	public function triggerSetParent($parentID){
		$this->product_type->data = $parentID;
	}
}
?>
