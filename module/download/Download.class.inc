<?php
execute_add_class('download', 'Download');
include('DownloadModel.class.inc');

class Download extends Control{
	function Download(){
		$this->tableName = 'download';
		$this->moduleName = 'download';
		$this->moduleTitle = tt('Download');
		$this->setAdmin('download_admin');
		$this->addAction('download_get', 'get');
		$this->addAccessMethod('guest', 'index');
		$this->init(new DownloadModel());
	}
	
	public function index(){
		if(MODE_ID == 0){
			document_set_path($this->moduleTitle, $this->moduleName);
			document_set_title($this->moduleTitle);
			return $this->fileList();
		}else{
			return $this->getFile();
		}
	}
	
	public function write(){
		$this->formMeta->id = $this->moduleName;
		$this->formMeta->title = tt('Upload new file');
		$this->formMeta->mode = 'download_insert';
		return $this->renderInsertForm();
	}
	
	public function insert(){
		Control::insert();
		$this->renameFile();
		$category = $this->dataByID['category']['category'];
		search_add_source('download', $this->id, $this->dataByID['name'], $category, '', $this->dataByID['description']);
	}
	
	public function update(){
		Control::update();
		$this->renameFile();
		$category = $this->dataByID['category']['category'];
		search_update_source('download', $this->id, $this->dataByID['name'], $category, '', $this->dataByID['description']);
	}
	
	private function getFile(){
		$this->id = gg('id');
		$this->selectByID(array('id', 'file', 'name', 'permission'));
		if(authority(array(), $this->dataByID['permission'])){
			$fileName = ROOT_PATH.$this->dataByID['file'];
			$mime = mime_content_type($fileName);
			$extension = split('\.', $this->dataByID['file']);
			$extension = $extension[count($extension) -1 ];
			header('Content-type: '.$mime);
			header('Content-Disposition: attachment; filename="'.$this->dataByID['name'].'.'.$extension.'"');
			readfile($fileName);
			execute_set_file();
			count_increase('download', $this->id);
		}else{
			execute_set_db();
			document_set_redirect('download');
			notify_add_error(tt('You are not allowed to donwload this file.'));
		}
	}
	
	private function fileList(){
		$this->listMeta->title = sprintf(tt('List of %s'), $this->moduleTitle);
		$this->listMeta->defaultOrder = 'id';
		if(MODE_ID) $mode = MODE.'/'.MODE_ID;
		else $mode = MODE;
		$this->listMeta->mode = $mode;
		$this->setListWriteIcon(tt('Upload new File'));
		$this->setListEditIcon();
		$this->setListDropIcon();
		$this->selectFile();
		$this->renderFileDescription();
		$this->getListColumn();
		return $this->renderListPage();
	}
	
	private function selectFile(){
		$category = (int) gg('category');
		if($category) $category = ' AND category='.$category;
		else $category = '';
		$this->selectListPage(10, 'WHERE (section='.SECTION.$category.')', $this->column);
	}
	
	private function renderFileDescription(){
		include_once(INCLUDE_PATH.'tool/icon.tool.inc');
		
		foreach($this->data as $key => $value){
			$uploader = tt('by').' '.render_profile_link($value['uploader']);
			$icon = icon_render($value['file']);
			$categoryName = $value['category']['category'];
			$categoryMode = SECTION_URI.Q.'download/category/'.$value['category']['id'];
			$category = '<a href="'.$categoryMode.'">'.$categoryName.'</a>';
			
			$render = '<h4>'.$icon.'<a href="'.SECTION_URI.Q.'download/'.$key.'">'.$value['name'].'</a></h4>';
			$render.= $value['description'];
			$render.= '<p>'.$uploader.' @ '.$value['upload_time'].' in '.$category.'</p>';
			$render.= '<p>'.sprintf(tt('%s times downloaded'), '<!--{ximple_read/download/'.$key.'}-->').'</p>';
			
			$this->data[$key]['name'] = $render;
			document_add_keyword($value['category']['category']);
		}
		if(gg('category')){
			$this->listMeta->title .= ' : '.$categoryName;
			document_set_path($categoryName);
		}
	}
	
	private function renameFile(){
		$this->selectByID($this->column);
		$type = io_get_file_extension($this->dataByID['file']);
		$fileName = 'files/download/'.sha1(rand()).md5(rand()).sha1(rand()).md5(rand()).'.'.$type;
		$success = true;
		@rename(ROOT_PATH.$this->dataByID['file'], ROOT_PATH.$fileName) or $success = false;
		if($success){
			$size = filesize(ROOT_PATH.$fileName);
			db_update('download', array('file' => $fileName, 'size' => $size), $this->id);
		}
	}
}
?>
