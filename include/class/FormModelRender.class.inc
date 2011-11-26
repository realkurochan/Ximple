<?php
abstract class FormModelRender{
	protected $render = array();
	protected $modelConnector;
	protected abstract function getFormColumn();
	
	public function prepareHead(){
		head_add_css('form.css');
		head_add_js('include/external/jquery/jquery-form.js');
		head_add_js('include/javascript/form.js');
		head_add_js_var('pleaseFill', tt('Please, fill the required field : '));
		head_add_js_var('formNotCorrect', tt('Any field in the form is not correctly filled. Please correct it and try again.'));
		head_add_js_var('isInstalled', true);
	}
	
	protected function renderFormModel($id){
		$this->modelConnector = extension_execute_append_result('form_render', $id, $this->modelConnector);
		$modelArray = $this->getFormColumn();
		foreach($modelArray as $name => $model){
			if(method_exists($model, 'triggerGetJavaScript')) $model->triggerGetJavaScript();
			if(!isset($mode->name)) $model->name = $name;
			$this->render[$name] = $model->renderForm($id);
		}
		return $this->render;
	}
}
?>
