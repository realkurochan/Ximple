<?php
class MultipleFilesColumn extends ModelColumn{
	private $path;
	protected $fileArray;
	public $isImage = false;
	
	function MultipleFilesColumn($label, $path, $help=null){
		$this->setType('char');
		$this->form = 'multiplefiles';
		$this->label = $label;
		$this->path = $path;
		$this->help = $help;
		$this->isMultipleInsert = true;
	}
	
	public function triggerGetJavaScript(){
		head_add_js('include/javascript/multiplefiles.column.js');
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$render  = '<div class="multiplefiles_list" id="'.$id.'">';
		$render .= '<ul>';
		for($i=0;$i<6;$i++){
			$render .= '<li><input type="file" name="'.$this->name.'[]" size="25" ></li>';	
		}
		$render .= '</ul>';
		$render .= '<span id="info_'.$id.'" style="clear:both;display:block;"></span>';
		$render .= '<p><a onclick="return addFileInput(\''.$id.'\', \''.$this->name.'\')" href="#">';
		$render .= '<img src="'.ROOT_URI.'files/icon/add.png" border="0" style="margin-right:20px;"/>';
		$render .= tt('Upload More File').'</a></p>';
		$render .= '</div>';
		return $render;
	}
	
	public function triggerPostInsert(){
		$this->upload();
	}
	
	public function triggerPostUpdate(){
		$this->upload();
	}
	
	private function upload(){
		$this->fileArray = input_reorder_multiple_files($this->name);
		foreach($this->fileArray as $key => $value){
			if($value['size']){
				$this->data[] = io_upload($value, $this->path, $this->isImage);
			}else{
				unset($this->fileArray[$key]);
			}
		}
	}
}
?>
