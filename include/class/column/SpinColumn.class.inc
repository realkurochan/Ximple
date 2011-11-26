<?php

global $spin_init_js;
$spin_init_js = false;

class SpinColumn extends ModelColumn{
	public $min;
	public $max;
	public $step;
	
	function SpinColumn($label, $min, $max=4294967295, $step=1, $help=null){
		$this->setType('int');
		$this->label = $label;
		$this->max = $max;
		$this->min = $min;
		$this->step = $step;
		$this->help = $help;
		$this->setFormAttribute('size', 5);
	}
	
	public function triggerGetJavaScript(){
		head_add_css('include/external/jquery-spinbox/jquery.spinbox.css');
		head_add_js('include/external/jquery-spinbox/jquery.spinbox.js');
	}
	
	public function renderForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('name', $this->name);
		$this->setFormAttribute('id', $id);
		$this->setFormAttribute('value', $this->data);
		$render = '<input '.$this->renderFormAttribute().'/>';
		$render.= '<span id="info_'.$id.'"></span>';
		$render.= '
		<script>
			if($("#'.$id.'").attr("rel") != "initialized"){
				$("#'.$id.'").spinbox({
					min  : '.$this->min.',
					max  : '.$this->max.',
					step : '.$this->step.'
				});
			}
			$("#'.$id.'").attr("rel", "initialized");
		</script>';
		return $render;
	}
	
	public function renderGridForm($formID){
		$id = $formID.'_'.$this->name;
		$this->setFormAttribute('name', $this->name.'[]');
		$this->setFormAttribute('class', $id);
		$this->setFormAttribute('value', $this->data);
		$this->initGridJS($id);
		$render  = '<input '.$this->renderFormAttribute().'/>';
		return $render;
	}
	
	private function initGridJS($id){
		global $spin_init_js;
		if(!$spin_init_js){
			$js = '
			gridInitFunction[".'.$id.'"] =  function(){
				$(".'.$id.'").each(function(){
					if($(this).attr("rel") != "initialized"){
						$(this).spinbox({
							min  : '.$this->min.',
							max  : '.$this->max.',
							step : '.$this->step.'
						});
					}
					$(this).attr("rel", "initialized");
				});
			}';
			head_inject_js($js);
			$spin_init_js = true;
		}
	}
}
?>
