<?php

include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('FlexiFunConfigModel.class.inc');
include('flexifun.tool.inc');

global $flexifun_config;

class FlexiFunConfig extends ConfigControl{
	function FlexiFunConfig(){
		$this->moduleTitle = tt('Configuration of FlexiFun Theme');
		$this->init(new FlexiFunConfigModel());
	}
	
	public function render(){
		$this->formMeta->id = 'Flexifun';
		$description  = tt('This configuration form should help the people, ');
		$description .= tt('who can not design their own theme, to customize their website. ');
		$description .= tt('However, it is not the main concept of the Ximple theme system. ');
		$description .= tt('If it is possible, you can use available theme or customize your CSS. ');
		$description .= tt('The later is much more flexible and more powerful.');
		$this->formMeta->description = $description;
		$this->formMeta->mode = 'theme_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return ConfigControl::render();
	}
	
	public function save(){
		ConfigControl::save();
		flexifun_generate_css(pp());
		flexifun_generate_image(pp());
		document_set_redirect('theme_config');
		execute_set_db();
	}
	
	protected function getConfig(){
		global $flexifun_config;
		$config_path = FILES_PATH.'customtheme/'.SECTION.'/'.cc('theme').'/config';
		$flexifun_config = io_unserialize($config_path);
		return $flexifun_config;
	}
	
	protected function setConfig($key, $value){
		global $flexifun_config;
		$flexifun_config[$key] = $value;
	}
	
	protected function saveConfig(){
		global $flexifun_config;
		$config_path = FILES_PATH.'customtheme/'.SECTION.'/'.cc('theme').'/config';
		io_serialize($flexifun_config, $config_path);
	}
}
?>
