<?php
execute_add_extended_class('blog', 'config', 'BlogConfig');
include_once(MODULE_PATH.'content/ContentConfig.class.inc');
include_once('BlogConfigModel.class.inc');

class BlogConfig extends ContentConfig{
	function BlogConfig(){
		$this->moduleName = 'blog';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('Blog Configuration');
		$this->setAdmin('blog_admin');
		$this->init(new BlogConfigModel());
		$this->setMode('blog');
	}
	
	public function index(){
		document_set_path(tt('Blog'), 'blog');
		$this->groupConfigForm();
		$this->appearanceFormGroup->append('blog_icon_size');
		$this->appearanceFormGroup->append('blog_entries_number');
		$this->formMeta->mode = 'blog_config_save';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
}
?>
