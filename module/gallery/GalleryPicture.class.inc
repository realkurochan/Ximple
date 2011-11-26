<?php
execute_add_extended_class('gallery', 'picture', 'GalleryPicture');
require_once('GalleryPictureModel.class.inc');

class GalleryPicture extends Control{
	private $galleryID;
	
	function GalleryPicture($galleryID=null){
		if(empty($galleryID)) $galleryID = pp('gallery');
		$this->galleryID = $galleryID;
		$this->tableName = 'gallery_picture';
		$this->moduleName = 'gallery';
		$this->moduleExtension = 'picture';
		$this->moduleTitle = tt('Gallery Picture');
		$this->setAdmin('gallery_admin');
		$this->init(new GalleryPictureModel($galleryID));
		$this->formMeta->id = 'gallery_picture';
		$this->formMeta->title = tt('Insert Picture');
		$this->formMeta->mode = 'gallery_picture_insert';
		$this->formMeta->isSetTitle = false;
	}
	
	public function insert(){
		require_once('Gallery.class.inc');
		$gallery = new Gallery();
		if($gallery->isCallable('update')){
			$gallery->id = pp('gallery');
			$gallery->createDirectory();
			$this->allowAccess();
			Control::insert();
			execute_as_db();
			document_set_redirect('gallery/'.$gallery->id);
		}
	}
	
	public function drop(){
		$this->id = (int) gg('id');
		$this->galleryID = (int) gg('gallery');
		$this->selectByID($this->column, true);
		$path = FILES_PATH.'gallery/'.$this->galleryID.'/';
		unlink($path.$this->dataByID['picture']);
		unlink($path.'thumb_'.$this->dataByID['picture']);
		Control::drop();
		document_set_redirect('gallery/'.$this->galleryID);
	}
	
	public function getPictures(){
		$this->select($this->column, 'WHERE gallery='.((int) $this->galelryID));
	}
	
	public function allowAccess(){
		$this->setOwner(USER_ID);
	}
}
?>
