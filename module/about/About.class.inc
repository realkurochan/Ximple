<?php
execute_add_class('about', 'About');
require(INCLUDE_PATH.'class/ConfigControl.class.inc');
require('AboutModel.class.inc');

class About extends ConfigControl{
	private $path;
	private $data;
	
	function About(){
		$this->moduleName = 'about';
		$this->addAccessMethod('guest', 'index');
		$this->setAdmin('config_admin');
		$this->addAction('about_edit', 'edit');
		$this->path = FILES_PATH.'about/'.SECTION;
		$this->init(new AboutModel());
	}
	
	public function index(){
		$this->getConfig();
		$this->data = translate_content($this->data, 'about', SECTION);
		$this->data['is_admin'] = authority_group('config_admin');
		document_set_path($this->data['title']);
		return render($this->data, 'about.tpl', 'about');
	}
	
	public function edit(){
		$this->getConfig();
		$title = sprintf(tt('Edit %s'), $this->data['title']);
		document_set_path($this->data['title'], 'about');
		$this->moduleTitle = sprintf(tt('Edit : %s'), $this->data['title']);
		$this->formMeta->id = 'about';
		$this->formMeta->title = $this->moduleTitle;
		$this->formMeta->mode = 'about_save';
		return  $this->render();
	}

	protected function getConfig(){
		$this->data = io_unserialize($this->path);
		if(!is_array($this->data)){
			$this->data = array(
				'title' => tt('About Us'),
				'body' => tt('You can write something about your website on this page to describe your visitor, what are you doing here.'),
			);
		}
		return $this->data;
	}
	
	protected function setConfig($key, $value){
		$this->data[$key] = $value;
	}
	
	protected function saveConfig(){
		if(!is_dir(dirname($this->path))) mkdir(dirname($this->path));
		io_serialize(input_strip_slash_array($this->data), $this->path);
		config_append_assc('navigation', 'about', pp('title'));
		config_save();
	}
}
?>
