<?php
class QuickFormRender{
	private $id;
	private $model = array();
	private $modelFactory;
	private $modelConnector;
	private $renderArray= null;
	private $render = array();
	
	function QuickFormRender($id){
		$this->id = $id;
	}
	
	public function append($name, $modelColumn){
		$this->model[$name] = $modelColumn;
		$this->model[$name]->name = $name;
		$message = $modelColumn->check();
		if(strlen($message) and state_is_develop()){
			notify_add_error('Model corrupted : '.$name.' : '.$message);
		}
	}
	
	public function setModel($model){
		$this->modelFactory = new ModelFactory($model);
		$this->modelConnector = $modelFactory->bind();
		$column = $this->modelConnector->getColumnName();
		$this->model = array();
		foreach($column as $key => $value){
			$this->model[$value] = $this->modelConnector->getColumn();
		}
	}
	
	public function reset(){
		$this->model = array();
	}
	
	public function render(){
		$this->getRenderArray();
		return render($this->renderArray, 'quick_form.tpl', 'quick_form');
	}
	
	public function renderEntireForm($formMeta){
		head_add_js('include/javascript/form.js');
		head_add_css('form.css');
		$this->renderArray['rendered_form'] = $this->render();
		$this->renderArray['meta'] = $formMeta;
		$data = array(
			'form' => render($this->renderArray, 'form_frame.tpl', 'quick_form_frame'),
			'meta' => $formMeta,
		);
		return render($data, 'quick_form_appearance.tpl', 'quick_form_appearance');
	}
	
	private function getRenderArray(){
		if(!is_array($this->renderArray)){
			require_once('ModelConnector.class.inc');
			$modelConnector = new ModelConnector($this->model, null);
			$this->renderArray = array(
				'id' => $this->id,
				'form' => $this->renderFormModel($this->id),
				'label' => $modelConnector->getColumnAttribute('label'),
				'help' => $modelConnector->getColumnAttribute('help'),
				'not_null' => $modelConnector->getColumnNameByBoolean('isNotNull'),
			);
		}
		return $this->renderArray;
	}
	
	private function renderFormModel($id){
		foreach($this->model as $name => $model){
			$this->render[$name] = $model->renderForm($id);
		}
		return $this->render;
	}
}

?>
