<?php
require_once('FileColumn.class.inc');
require_once(INCLUDE_PATH.'tool/image.tool.inc');

class ImageColumn extends FileColumn{
	private $maxWidth;
	
	function ImageColumn($label, $path, $maxWidth=600, $help=null){
		FileColumn::FileColumn($label, $path, $help);
		$this->maxWidth = $maxWidth;
		$this->help = $help.tt('Image must be only .png or .jpeg or .jpg. ');
		$this->help.= sprintf(tt('For image wider than %dpx, it will be automatically resized.'), $maxWidth);
	}
	
	public function triggerPostInsert(){
		require_once(INCLUDE_PATH.'tool/image.tool.inc');
		FileColumn::triggerPostInsert();
		$this->checkImage();
	}
	
	public function triggerPostUpdate(){
		require_once(INCLUDE_PATH.'tool/image.tool.inc');
		FileColumn::triggerPostUpdate();
		$this->checkImage();
	}
	
	public function triggerSelect(){
		$fileName = basename($this->data);
		if(is_file(ROOT_PATH.$this->data)){
			$this->data = '<img src="'.ROOT_URI.$this->data.'" alt="'.$fileName.'" border="0"/>';
		}else{
			$this->data = null;
		}
	}
	
	protected function checkImage(){
		$file = ff($this->name);
		if($file['size'] and is_file(ROOT_PATH.$this->data)){
			image_check_max_width($file, ROOT_PATH.$this->data, $this->maxWidth);
		}	
	}
}
?>
