<?php
class CurrencyColumn extends ModelColumn{
	private $mainData;
	private $subData;
	
	function CurrencyColumn($label, $help=null){
		$this->setType('real');
		$this->label = $label;
		$this->help = $help;
	}
	
	public function triggerGetJavaScript(){
		head_add_css('include/external/jquery-spinbox/jquery.spinbox.css');
		head_add_js('include/external/jquery-spinbox/jquery.spinbox.js');
		head_add_js('include/javascript/currency.column.js');
	}
	
	public function renderForm($formID){
		$this->prepareData();
		$id = $formID.'_'.$this->name;
		
		$this->setFormAttribute('type', 'hidden');
		$this->setFormAttribute('name', $this->name);
		$this->setFormAttribute('class', 'currency');
		$this->setFormAttribute('id', $id);
		$render = '<input '.$this->renderFormAttribute().'/>';
		
		$this->resetFormAttribute();
		$this->setFormAttribute('name', $this->name.'_main');
		$this->setFormAttribute('id', $id.'_main');
		$this->setFormAttribute('value', (int) $this->mainData);
		$this->setFormAttribute('class', 'currency_main');
		$this->setFormAttribute('size', 5);
		$render.= '<input '.$this->renderFormAttribute().'/>.';
		
		$this->resetFormAttribute();
		$this->setFormAttribute('name', $this->name.'_sub');
		$this->setFormAttribute('id', $id.'_sub');
		$this->setFormAttribute('value', (int) $this->subData);
		$this->setFormAttribute('class', 'currency_sub');
		$this->setFormAttribute('size', 2);
		$render.= '<input '.$this->renderFormAttribute().'/> '.cc('currency_label');
		$render.= '<span id="info_'.$id.'"></span>';
		return $render;
	}
	
	public function renderGridForm($formID){
		$this->prepareData();
		$id = $formID.'_'.$this->name;
		
		$this->setFormAttribute('type', 'hidden');
		$this->setFormAttribute('name', $this->name.'[]');
		$this->setFormAttribute('class', 'currency');
		$render = '<input '.$this->renderFormAttribute().'/>';
		
		$this->resetFormAttribute();
		$this->setFormAttribute('name', $this->name.'_main[]');
		$this->setFormAttribute('value', (int) $this->mainData);
		$this->setFormAttribute('class', 'currency_main');
		$this->setFormAttribute('size', 5);
		$render.= '<input '.$this->renderFormAttribute().'/>.';
		
		$this->resetFormAttribute();
		$this->setFormAttribute('name', $this->name.'_sub[]');
		$this->setFormAttribute('value', (int) $this->subData);
		$this->setFormAttribute('class', 'currency_sub');
		$this->setFormAttribute('size', 5);
		$render.= '<input '.$this->renderFormAttribute().'/>'.cc('currency_label');
		return $render;
	}
	
	private function prepareData(){
		$this->data = (float) $this->data;
		$this->mainData = (int) $this->data;
		$this->subData = (int)(($this->data - $this->mainData)*100);
	}
}
?>
