<?php
execute_add_extended_class('directory', 'config', 'DirectoryConfig');
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include_once('directory.tool.inc');
include('DirectoryConfigModel.class.inc');

class DirectoryConfig extends ConfigControl{
	private $config;
	function DirectoryConfig(){
		$this->moduleName = 'directory';
		$this->moduleExtension = 'config';
		$this->moduleTitle = tt('Directory Configuration');
		$this->setAdmin('directory_admin');
		$this->addAction('directory_config', 'index');
		$this->addAction('directory_config_save', 'save');
		$this->saveRedirection = 'directory_config/'.mid();
		$this->init(new DirectoryConfigModel(), false);
	}
	
	public function index(){
		$this->formMeta->mode = 'directory_config_save/'.mid();
		$this->formMeta->title = $this->moduleTitle;
		directory_set_path(mid());
		document_set_path($this->moduleTitle);
		head_add_js_var('translateModuleName', 'directory_config');
		
		$directory = $this->getNewConfigForm(tt('Sub-Directory Render'));
		$directory->append('directory_list_label');
		$directory->append('show_directory');
		
		$page = $this->getNewConfigForm(tt('Page Render'));
		$page->append('page_list_label');
		$page->append('related_page_label');
		$page->append('page_render_item');
		$page->append('show_page');
		
		$status = $this->getNewConfigForm(tt('Page Publishing Status'));
		$status->append('publishing_status');
		$status->append('publishing_status_default');
		
		$comment = $this->getNewConfigForm(tt('Page Comment'));
		$comment->append('comment_page_length');
		$comment->append('comment_show_avatar');
		$comment->append('comment_write_label');
		$comment->append('comment_permission');
		$comment->append('comment_permission_default');
		return $this->render(false);
	}
		
	protected function getConfig(){
		$data = db_select_single_column('directory', 'configuration', 'WHERE id='.mid(), false);
		$this->config = unserialize($data[0]);
		return $this->config;
	}
	
	protected function setConfig($key, $value){
		if($key == 'comment_permission'){
			$avaiable = pp('comment_permission');
			if(!is_array($avaiable) or count($avaiable) == 0) $value = array(0, 1, 2, 3);
		}elseif($key == 'publishing_status'){
			$avaiable = pp('publishing_status');
			if(!is_array($avaiable) or count($avaiable) == 0) $value = array(2, 3);
		}elseif($key == 'comment_permission_default'){
			$avaiable = pp('comment_permission');
			if(!is_array($avaiable) or count($avaiable) == 0) $value = 0;
			elseif(!in_array($value, $avaiable)) $value = $avaiable[0];
		}elseif($key == 'publishing_status_default'){
			$avaiable = pp('publishing_status');
			if(!is_array($avaiable) or count($avaiable) == 0) $value = 3;
			elseif(!in_array($value, $avaiable)) $value = $avaiable[0];
		}
		$this->config[$key] = $value;
	}
	
	protected function saveConfig(){
		$data = array(
			'configuration' => serialize($this->config)
		);
		db_update('directory', $data, mid());
	}
}
?>
