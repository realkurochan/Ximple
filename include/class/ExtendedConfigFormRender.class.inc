<?php
class ExtendedConfigFormRender{
	private $control;
	private $configMode;
	
	function ExtendedConfigFormRender($control, $configMode){
		$this->control = $control;
		$this->configMode = $configMode;
	}
	
	public function render(){
		$configModel = extension_execute_append_result($this->configMode, null, array());
		if(is_array($configModel)){
			foreach($configModel as $label => $model){
				if(is_array($model)) $this->renderAdditionConfigModelArray($label, $model);
				elseif(is_a($model, 'Model')) $this->renderAdditionConfigModelObject($label, $model);
				elseif(state_is_develop) notify_add_error(sprintf(tt('The given addition configuration is corrupted (%s).'), $label));
			}
		}
	}
	
	private function renderAdditionConfigModelArray($label, $modelArray){
		$configForm = $this->control->getNewConfigForm($label);
		foreach($modelArray as $name => $model){
			$model->data = $this->control->getConfigValue($name);
			if(empty($model->data)) $model->data = $model->defaultData;
			$configForm->appendModelColumn($name, $model);
		}
	}
	
	private function renderAdditionConfigModelObject($label, $modelObject){
		$configForm = $this->control->getNewConfigForm($label);
		foreach($modelObject->asArray() as $key => $model){
			$model->data = $this->control->getConfigValue($key);
			if(empty($model->data)) $model->data = $model->defaultData;
		}
		$configForm->appendModel($modelObject->moduleName, $modelObject);
		if(isset($modelObject->showByDefault)){
			$configForm->setShowByDefault($modelObject->showByDefault);
		}
	}
}
?>
