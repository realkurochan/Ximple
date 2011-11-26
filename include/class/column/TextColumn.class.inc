<?php
class TextColumn extends ModelColumn{
	function TextColumn($label, $help=null){
		$this->setType('text');
		$this->label = $label;
		$this->help = $help;
		$this->isAllowedTranslate = true;
		$this->setFormAttribute('class', 'text');
		$this->isReachable = true;
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('name', $this->name);
		$this->setFormAttribute('id', $id);
		$this->setFormAttribute('style', 'width:100%');
		$this->setFormAttribute('rows', 20);
		$data = str_replace('&lt;', '&amp;lt;', $this->data);
		$data = str_replace('&gt;', '&amp;gt;', $data);
		$render = '<textarea '.$this->renderFormAttribute().'>'.$data.'</textarea>';
		$render.= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	public function triggerSelect(){
		include_once(INCLUDE_PATH.'tool/highlight.tool.inc');
		$this->data = highlight($this->data);
	}
	
	public function triggerGetJavaScript(){
		head_add_css('include/external/cleditor/jquery.cleditor.css');
		head_add_js('include/external/cleditor/jquery.cleditor.js');
		head_add_js('include/external/cleditor/jquery.cleditor.advancedtable.min.js');
		head_add_js('include/external/cleditor/jquery.cleditor.icon.js');
		head_add_js('include/external/cleditor/jquery.cleditor.code.js');
		head_add_js('include/external/cleditor/jquery.cleditor.quote.js');
		head_add_js('include/external/cleditor/jquery.cleditor.advancedimage.js');
		head_add_js('include/javascript/text.column.js');
		
		$smileyFolder  = FILES_PATH.'smiley/';
		if(!is_dir($smileyFolder)) mkdir($smileyFolder);
		$smileyFolder .= SECTION.'/';
		if(!is_dir($smileyFolder)){
			mkdir($smileyFolder);
			$cleFolder = INCLUDE_PATH.'external/cleditor/images/icons/';
			io_copy_directory($cleFolder, $smileyFolder);
		}
		$iconArray = io_get_directory_entry($smileyFolder);
		foreach($iconArray as $key => $value){
			$iconArray[$key] = ROOT_URI.'files/smiley/'.SECTION.'/'.$value;
		}
		head_add_js_array('iconArray', $iconArray);
	}
}
?>
