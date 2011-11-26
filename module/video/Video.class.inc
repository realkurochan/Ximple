<?php
execute_add_class('video', 'Video');
include_once(MODULE_PATH.'content/Content.class.inc');
include('VideoModel.class.inc');

class Video extends Content{
	function Video(){
		$this->tableName = 'video';
		$this->moduleName = 'video';
		$this->moduleTitle = tt('Video');
		$this->init(new VideoModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		if($this->isSingleEntry()) return $this->renderSingleVideo();
		else return $this->renderMultipleVideos();
	}
	
	public function renderSingleVideo(){
		$this->id = mid();
		$this->selectByID($this->column);
		if($this->isReadable()){
			$this->prepareRender();
			$render = render($this->dataByID, 'video.tpl', 'video');
			$render.= $this->comment->render();
			head_add_css('video.css');
			$this->setDocumentData();
			return $render;
		}else{
			$this->accessDeniedEntry();
		}
	}
	
	private function renderMultipleVideos(){
		head_add_css('video.css');
		$this->selectFirstPage($this->column, cc('video_entries_number'));
		$this->getCommentNumber();
		$this->getDataEditable();
		$this->setPathMutiplePage();
		$render = render(array('data' => $this->data), 'video_multiple.tpl', 'video_multiple');
		$render.= $this->pager->render('video'.$this->getPageMode());
		return $render;
	}
	
	public function insert(){
		$file = ff('file');
		if($file['size'] == 0 or $file['type'] == 'video/x-flv'){
			Content::insert();
			$this->selectByID($this->column);
			$size = filesize(ROOT_PATH.$this->dataByID['file']);
			db_update($this->tableName, array('size' => $size), $this->id);
		}else{
			notify_add_error(tt('Video support only .flv file.'));
		}
	}
	
	public function update(){
		$file = ff('file');
		if($file['size'] == 0 or $file['type'] == 'video/x-flv'){
			Content::update();
			$this->selectByID($this->column);
			$size = filesize(ROOT_PATH.$this->dataByID['file']);
			db_update($this->tableName, array('size' => $size), $this->id);
		}else{
			notify_add_error(tt('Video support only .flv file.'));
		}
	}
	
	private function prepareRender(){
		$this->handlePublishingStatus();
		$this->initComment();
		$this->insertTracker('read');
		$this->addMetaData();
		$this->dataByID['editable'] = $this->isEditable($this->dataByID['writer']['id']);
		$operation = $this->getExtendedOperation();
		$this->dataByID['addition_operation'] = $operation[$this->id];
	}
}

?>
