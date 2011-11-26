<?php
execute_add_class('gallery', 'Gallery');
include_once(MODULE_PATH.'content/Content.class.inc');
include_once('GalleryModel.class.inc');

class Gallery extends Content{
	private $galleryPicture;
	
	function Gallery($isEmbed=false){
		$this->tableName = 'gallery';
		$this->moduleName = 'gallery';
		$this->moduleTitle = tt('Gallery');
		$this->addAction('gallery_picture_insert', 'insertPicture');
		$this->addAction('gallery_picture_drop', 'dropPicture');
		$this->init(new GalleryModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		if($this->isSingleEntry()) return $this->renderSingleGallery();
		else return $this->renderMultipleGalleries();
	}
	
	
	public function createDirectory(){
		$directory = FILES_PATH.'gallery/';
		if(!is_dir($directory)) mkdir($directory);
		$directory.= $this->id;
		if(!is_dir($directory)) mkdir($directory);
	}
	
	private function renderSingleGallery(){
		$this->id = mid();
		$this->selectByID($this->column);
		$this->translateDataByID();
		if($this->isReadable()){
			$this->prepareRender();
			head_add_css('gallery.css');
			require_once('GalleryPicture.class.inc');
			$this->galleryPicture = new GalleryPicture($this->id);
			$this->dataByID['picture'] = $this->getPictures($this->id);
			$this->getPictureForm();
			$this->getAppearanceConfiguration();
			$render = render($this->dataByID, 'gallery.tpl', 'gallery');
			$render.= $this->comment->render();
			$this->setDocumentData();
			return $render;
		}else{
			$this->accessDeniedEntry();
		}
	}
	
	private function renderMultipleGalleries(){
		head_add_css('gallery.css');
		$column = $this->modelConnector->getColumnNameExcepted(array('content'));
		$this->selectFirstPage($column, cc('gallery_entries_number'));
		$this->translateData();
		foreach($this->data as $key => $value){
			$this->data[$key]['picture'] = $this->getPictures($key);
		}
		$this->getCommentNumber();
		$this->getDataEditable();
		$this->setPathMutiplePage();
		$renderArray = array(
			'title' => $this->getListTitle(),
			'description' => $this->getCategoryDescription(),
			'data' => $this->data
		);
		$render  = render($renderArray, 'gallery_multiple.tpl', 'gallery_multiple');
		$render .= $this->pager->render('gallery'.$this->getPageMode());
		return $render;
	}
	
	private function prepareRender(){
		$this->handlePublishingStatus();
		$this->initComment();
		$this->insertTracker('read');
		$this->addMetaData();
		$this->dataByID['editable'] = $this->isEditable($this->dataByID['writer']['id']);
	}
	
	private function getPictureForm(){
		if($this->isCallable('update')){
			$this->dataByID['picture_form'] = $this->galleryPicture->renderInsertForm();
		}else{
			$this->dataByID['picture_form'] = '';
		}
	}
	
	private function getPictures($galleryID){
		$clause = 'WHERE gallery='.$galleryID;
		return db_select_single_column('gallery_picture', 'picture', $clause);
	}
	
	private function getAppearanceConfiguration(){
		$appearance = cc('gallery_appearance');
		if(!is_array($appearance)) $appearance = array();
		$this->dataByID['show_avatar'] = in_array('avatar', $appearance);
		$this->dataByID['show_category'] = in_array('category_icon', $appearance);
		$this->dataByID['show_sibling'] = in_array('sibling', $appearance);
	}
	
	private function getListTitle(){
		$tag = gg('tag');
		if($tag) $title = sprintf("%s %s", tt('Gallery List tagged by'), $tag);
		else $title = tt('Gallery List');
		return $title;
	}
	
	private function getCategoryDescription(){
		$tag = gg('tag');
		$description = '';
		if($tag){
			$galleryEntries = array_values($this->data);
			if($galleryEntries[0]['category']['category'] == $tag){
				$description =  $galleryEntries[0]['category']['description'];
			}
		}
		return $description;
	}
}
?>
