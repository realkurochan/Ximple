<?php
include_once('product.tool.inc');
include('ProductAttributeTypeModel.class.inc');

class ProductTypeModel extends Model{
	function ProductTypeModel(){
		$this->name = $this->title(tt('name'));
		$this->parent = $this->select(tt('parent type'), product_get_type());
		
		$directory = 'files/product/'.SECTION.'/typeicon/';
		$size = cc('product_type_icon_size');
		$this->icon = $this->imageLoader(tt('icon'), $directory, $size);
		
		$this->description = $this->shortText(tt('description'));
		$this->configuration = $this->hidden(null, 'text');
		
		$this->attribute_type = $this->grid('attribute type');
		$this->attribute_type->setModel(new ProductAttributeTypeModel());
	}
}
?>
