<?php
class ProductAttributeOptionModel extends Model{
	public $moduleName = 'product';
	public $tableName = 'product_attribute_option';
	public $dropMode = 'product_type_drop_attribute_option';
	
	function ProductAttributeOptionModel($attributeTypeID=null){
		$this->attribute_type = $this->hidden($attributeTypeID, 'int');
		$this->attribute_key = $this->hidden(null, 'int');
		$this->attribute_value = $thi->char(tt('value'));
	}
}
?>
