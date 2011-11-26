<?php
require_once(INCLUDE_PATH.'class/column/MultipleFilesColumn.class.inc');

class GalleryPictureColumn extends MultipleFilesColumn{
	function GalleryPictureColumn($galleryID){
		$this->galleryID = $galleryID;
		$this->path = FILES_PATH.'gallery/'.$galleryID.'/';
		MultipleFilesColumn::MultipleFilesColumn('', $this->path, 'picture');
	}
	
	function triggerPostInsert(){
		MultipleFilesColumn::triggerPostInsert();
		$thumbpath = $this->path.'thumb_';
		foreach($this->fileArray as $key => $value){
			image_generate_thumb($value, $this->path.'/'.$value['name'], $thumbpath.$value['name']);
		}
		
		foreach($this->data as $key => $value){
			$this->data[$key] = str_replace('files/gallery/'.$this->galleryID.'/', '', $value);
		}
	}
	
	function triggerDrop(){
	}
}
?>
