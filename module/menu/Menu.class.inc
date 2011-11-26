<?php
execute_add_class('menu', 'Menu');
include('MenuModel.class.inc');

class Menu extends Control{
	function Menu(){
		$this->moduleTitle = 'Menu';
		$this->moduleName = 'menu';
		$this->tableName = 'menu';
		$this->addAction('menu_save_sort', 'saveSort');
		$this->setAdmin('menu_admin');
		$this->init(new MenuModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		document_set_title(tt('Menu'));
		$data = $this->selectSingleColumn('title', 'WHERE section='.SECTION.' ORDER by sort_number');
		head_add_css('list.css');
		head_add_css('form.css');
		return render(array('data' => $data), 'menu_edit.tpl', 'menu_edit');
	}
	
	public function write(){
		document_set_path(sprintf(tt('Add %s'), $this->moduleTitle));
		$this->formMeta->id = 'menu';
		$this->formMeta->title = tt('Add a new Menu');
		$this->formMeta->mode = 'menu_insert';
		return $this->renderInsertForm();
	}
	
	public function edit(){
		document_set_path(sprintf(tt('Edit %s'), $this->moduleTitle));
		$this->id = (int)gg('id');
		$this->selectByID($this->column);
		$this->formMeta->id = 'menu';
		$this->formMeta->title =  tt('Edit Menu');
		$this->formMeta->mode = 'menu_update';
		return $this->renderUpdateForm();
	}
	
	public function saveSort(){
		$menuID = pp('menu');
		foreach($menuID as $key => $value){
			db_update($this->tableName, array('sort_number' => $key), $value);
		}
		notify_add_info(tt('Menus sort setting has been updated.'));
		execute_set_db();
		document_set_redirect($this->moduleName);
	}
}
?>
