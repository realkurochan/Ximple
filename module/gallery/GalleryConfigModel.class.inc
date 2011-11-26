<?php
class GalleryConfigModel extends ContentConfigModel{
	function GalleryConfigModel(){
		ContentConfigModel::ContentConfigModel('gallery');
		$option = array(3 => 3, 5 => 5, 8 => 8, 10 => 10, 15 => 15, 20 => 20);
		$this->gallery_entries_number = $this->select(tt('gallery entries per page'), $option);
		$this->extendBefore('gallery_comment_write_label', 'gallery_entries_number');
	}
}
?>
