<?php
include_once('product.tool.inc');
include_once(MODULE_PATH.'freetag/FreeTagColumn.class.inc');
include('ProductTypeColumn.class.inc');

class ProductModel extends Model{
	function ProductModel(){
		$this->name = $this->title(tt('product name'));
		$this->type = new ProductTypeColumn();
		$this->brand = $this->autoComplete(tt('brand'), 'product_get_brand');
		$this->description = $this->text(tt('description & features'));
		$this->freetag = new FreeTagColumn('product');
		$this->price = $this->spin(tt('price'), 0, 4294967296);
		$this->currency = $this->select(tt('currency'), product_get_currency());
		$this->rating = $this->none(0, 'int');
		
		$this->product_file = $this->grid(tt('image & file'));
		$this->product_file->setModel(new ProductFileModel());
		
		$this->attribute = $this->grid(tt('attributes'));
		$this->attribute->setModel(new ProductAttributeModel());
	}
}
?>
