<?php
execute_add_class('widget', 'Widget');
include('WidgetModel.class.inc');

class Widget extends Control{
	function Widget(){
		$this->moduleTitle = tt('Widget');
		$this->moduleName = 'widget';
		$this->tableName = 'widget';
		$this->addAction('widget_save_sort', 'saveSort');
		$this->setAdmin('widget_admin');
		$this->insertRedirection = 'widget#position_'.pp('position');
		$this->init(new WidgetModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		$this->select($this->column, 'WHERE section='.SECTION.' ORDER BY sort_number');
		$data = array();
		foreach($this->data as $key => $value){
			if(!in_array($value['position'], array_keys(cc('widget_position')))){
				$data['disabled'][] = $value;
			}else{
				$data[$value['position']][] = $value;
			}
		}
		$renderArray = array(
			'data' => $data,
			'position' => cc('widget_position'),
		);
		head_add_css('list.css');
		head_add_css('form.css');
		return render($renderArray, 'widget_edit.tpl', 'widget_edit');
	}
	
	public function saveSort(){
		$position = pp('position');
		$callMode = pp('call_mode');
		$widgetID = pp('widget');
		foreach($widgetID as $key => $value){
			if($callMode[$key] == 'widget_render_panel' and $position[$key] == 'disabled'){
				notify_add_error(tt('Control Panel is not allowed to disable. Its position is not updated.'));
			}else{
				$data = array(
					'position' => $position[$key],
					'sort_number' => $key,
				);
				db_update($this->tableName, $data, $value);
			}
			if($callMode[$key] == 'widget_render_navigation' and $position[$key] == 'disabled'){
				notify_add_warning(tt('It is not a good idea to disable Navigation Widget.'));
			}
		}
		notify_add_info(tt('Widgets sort and position setting has been updated.'));
		execute_set_db();
		document_set_redirect($this->moduleName);
	}
	
	public function write(){
		document_set_path(sprintf(tt('Add %s'), $this->moduleTitle));
		$position = cc('widget_position');
		$this->formMeta->id = 'Widget';
		$this->formMeta->title = sprintf(tt('Add a new Widget to %s'), $position[gg('position')]);
		$this->formMeta->mode = 'widget_insert';
		$this->setConfigForm();
		return $this->renderInsertForm();
	}
	
	public function edit(){
		document_set_path(sprintf(tt('Edit %s'), $this->moduleTitle));
		$this->id = (int)gg('id');
		$this->selectByID($this->column);
		$this->formMeta->id = 'Widget';
		$this->formMeta->title = tt('Edit Widget');
		$this->formMeta->mode = 'widget_update';
		$configMode = $this->dataByID['config_form_mode'];
		if(!empty($configMode)){
			$configMode .= '/is_translate/false';
			$this->renderAdditionalConfig($configMode);
		}
		$this->setConfigForm();
		return $this->renderUpdateForm();
	}
	
	public function update(){
		Control::update();
		$configMode = $this->dataByID['config_save_mode'];
		if(!empty($configMode)){
			$configMode .= '/is_translate/false';
			execute_string($configMode);
		}
		document_set_redirect('widget#widget_'.$this->id);
	}
	
	public function renderTranslateForm(){
		$this->id = gg('module_id');
		$this->selectByID($this->column);
		$configMode = $this->dataByID['config_form_mode'];
		if(!empty($configMode)){
			$configMode .= '/is_translate/true';
			$configMode .= '/module_id/'.gg('module_id');
			$configMode .= '/language/'.gg('language');
			$this->renderAdditionalConfig($configMode);
		}
		return Control::renderTranslateForm();
	}
	
	private function setConfigForm(){
		$appearConfig = $this->getNewConfigForm('Appearance Setting');
		$appearConfig->append('appear_group');
		$appearConfig->append('appear_module');
		$appearConfig->append('appear_uri');
	}
	
	private function renderAdditionalConfig($configMode){
		include_once(INCLUDE_PATH.'class/QuickFormRender.class.inc');
		$configModel = execute_string($configMode);
		$quickForm = new QuickFormRender('widget');
		if(is_array($configModel)){
			foreach($configModel as $key => $value){
				$quickForm->append($key, $value);
			}
		}
		$this->formMeta->additionalForm = array($quickForm->render());
	}
}
?>
