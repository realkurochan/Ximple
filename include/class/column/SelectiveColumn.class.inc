<?php
abstract class SelectiveColumn extends ModelColumn{
	public $option;
	public $isSortable;
	
	protected $listClass;
	protected $itemClass;
	protected $divisionClass;
	protected $imageTag;
	
	function SelectiveColumn($label, $option, $help=null){
		$this->setType('char');
		$this->label = $label;
		$this->help = $help;
		$this->option = $option;
	}
	
	public function triggerGetJavaScript(){
		if($this->isSortable){
			head_add_js('include/javascript/sortable.js');
		}
	}
	
	public function check(){
		if(!is_array($this->option) and state_is_develop()){
			notify_add_error('Option of '.$this->name.' is not set.');
		}
	}
	
	public function triggerSelect(){
		if(!is_array($this->data) and isset($this->option[$this->data])){
			$this->data = array(
				'value' => $this->data,
				'label' => $this->option[$this->data],
			);
		}
	}
	
	public function triggerSerialize(){
		if($this->isSerialize){
			$this->data = serialize($this->data);
		}
	}
	
	protected function renderRawsForm($formID){
		$id = $formID.'_'.$this->name;
		$render = '<div class="'.$this->divisionClass.'">';
		$render.= '<div class="'.$this->listClass.'" id="'.$id.'">';
		$this->setFormAttribute('style', 'border:none;');
		foreach($this->option as $key => $value){
			$this->setFormAttribute('value', $key);
			$this->getCheck($key);
			$render .= '<div class="'.$this->itemClass.'">'.$this->imageTag;
			$render .= '<input '.$this->renderFormAttribute().'/>'.$value;
			$render .= '</div>';
		}
		$render .= '</div>';
		$render .= '</div>';
		$render .= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	protected function getCheck($data){
	}
	
	protected function checkSortable(){
		if($this->isSortable){
			$this->listClass = 'sortable';
			$this->itemClass = 'sortable_item';
			$this->imageTag = '<img src="'.ROOT_URI.'files/icon/move.png" />';
			head_add_js('include/javascript/sortable.js');
		}else{
			$this->listClass = 'unsortable';
			$this->itemClass = 'unsortable_item';
			$this->imageTag = '';
		}
	}
}
?>
