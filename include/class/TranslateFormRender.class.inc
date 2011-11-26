<?php
class TranslateFormRender extends FormRender{
	private $moduleID;
	private $translateData;
	private $moduleName;
	
	function TranslateFormRender($modelConnector, $formMeta, $moduleName){
		$this->moduleName = $moduleName;
		FormRender::FormRender($modelConnector, $formMeta);	
	}
	
	public function renderTranslateForm($data){
		$this->translateData = $data;
		$this->modelConnector->setData($data);
		$this->modelConnector->triggerUpdateForm();
		$this->prepareRender();
		$this->label['status'] = tt('status');
		$this->formMeta->isAjax = true;
		$this->formMeta->ajaxFunction = 'submitTranslateForm';
		return FormRender::renderForm();
	}
	
	protected function checkTranslate(){
		$this->isTranslate = false;
	}
	
	protected function getFormColumn(){
		$model = $this->modelConnector->getTranslateFormColumn();
		$additionalModel = $this->getAdditionalModel();
		return array_merge($additionalModel, $model);
	}
	
	private function getAdditionalModel(){
		$statusOption = array(
			0 => tt('draft'),
			1 => tt('published'),
		);
		$status = Model::select(tt('status'), $statusOption);
		$status->data = $this->translateData['status'];
		$model = array(
			'status' => $status,
			'language' => Model::hidden(gg('language')),
			'module' => Model::hidden($this->moduleName),
			'module_id' => Model::hidden(gg('module_id')),
		);
		if(isset($this->translateData['id'])){
			$model['id'] = Model::hidden($this->translateData['id']);
		}
		return $model;
	}
}
?>
