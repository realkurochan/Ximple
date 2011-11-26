<?php
class LocaleModuleModel extends Model{
	function LocaleModuleModel(){
		$this->module = $this->select(tt('module'), locale_get_module());
		$this->module->data = '';
		$this->theme = $this->select(tt('theme'), locale_get_theme());
		$this->theme->data = '';
	}
}
?>
