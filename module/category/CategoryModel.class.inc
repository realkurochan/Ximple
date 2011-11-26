<?php
class CategoryModel extends Model{
	function CategoryModel($mode){
		$this->category = $this->title(tt('category'));
		$this->category->enableTranslate();
		$this->pic = $this->image(tt('picture'), 'files/category/', 150);
		$this->pic->isList = true;
		$this->description = $this->shortText(tt('description'));
		$this->description->isList = true;
		$this->description->isReachable = true;
		$this->description->enableTranslate();
		$this->mode = $this->hidden($mode, 'char');
		$this->section = $this->section();
	}
}
?>
