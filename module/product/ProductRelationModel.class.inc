<?php
require_once('RelatedProductColumn.class.inc');

class RelatedProductModel extends Model{
	function RelatedProductModel($sourceProductID=null){
		$this->source_product = $this->hidden($sourceProductID, 'int');
		$this->related_product = new RelatedProductColumn();
		$this->weighting = $this->spin(tt('weighting'), 1, 10);
		$this->click = $this->none(0, 'int');
	}
}
?>
