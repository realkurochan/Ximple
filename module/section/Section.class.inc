<?php
if(SECTION != 1) error_denied();
execute_add_class('section', 'Section');
include('SectionModel.class.inc');

define('SECTION_SCRIPT',
"<?php
	define('BOOT_TIME', (float)microtime(true));
	define('SECTION', %s);
	define('SECTION_URI', '%s');
	define('SECTION_PATH', dirname(__FILE__).'/');
	require('%sinclude/tool/boot.tool.inc');
	boot();
?>");

define('CRON_SCRIPT',
"<?php
	if(PHP_SAPI === 'cli'){
		define('SECTION', %s);
		define('SECTION_PATH', dirname(__FILE__).'/');
		define('XIMPLE_CRON_PROCESS', 'Cron Process');
		require('%sinclude/tool/boot.tool.inc');
		boot_cron();
	}else{
		die('Cron Job can only be started from CLI.');
	}
?>");

class Section extends Control{
	function Section(){
		$this->tableName = 'section';
		$this->moduleName = 'section';
		$this->moduleTitle = 'Section';
		$this->addAction('section_redirect', 'redirect');
		$this->addAction('section_manage', 'index');
		$this->setAdmin('section_admin');
		$this->init(new SectionModel());
		document_set_path($this->moduleTitle, 'section_manage');
	}
	
	public function index(){
		$description  = tt('Section is an instance of Ximple. ');
		$description .= tt('You can install Ximple only one time on a server and create multiple Sections. ');
		$description .= tt('Every Section is a self-contained website and has the own configurations but joint user and database.');
		$description .= tt('This can dramatically reduce your work for maintenance of multiple websites.');
		$this->listMeta->title = sprintf(tt('List of %s'), $this->moduleTitle);
		$this->listMeta->defaultOrder = 'id';
		$this->listMeta->description = $description;
		$this->setListWriteIcon();
		$this->setListEditIcon();
		$this->selectListPage();
		return $this->renderListPage();
	}
	
	public function write(){
		$this->initJS();
		head_add_js_var('sectionInsert', true);
		return Control::write();
	}
	
	public function edit(){
		$this->initJS();
		head_add_js_var('sectionUpdateID', $this->id);
		head_add_js_var('sectionInsert', false);
		return Control::edit();
	}
	
	public function insert(){
		$this->initScript();
		$this->initConfig();
		$this->initAdminMap();
		document_set_redirect('section_manage');
	}
	
	public function update(){
		$success = true;
		if($this->id != 1) $this->renamePath();
		if($success){
			Control::update();
			$this->updateConfig();
		}
		document_set_redirect('section_manage');
	}
	
	public function drop(){
		notify_add_error(tt('Section is not allowed to be deleted.'));
	}
	
	public function redirect(){
		$this->id = (int)gg('id');
		$this->selectByID($this->column);
		header('Location: '.$this->dataByID['uri']);
		die();
	}
	
	private function initJS(){
		head_add_js('module/section/section.js');
		head_add_js_var('nameTooShort', tt('Section name is too short.'));
		head_add_js_var('pathTooShort', tt('Section path is too short.'));
		head_add_js_var('nameExists', tt('Section name exists.'));
		head_add_js_var('pathExists', tt('Section path exists.'));
		head_add_js_var('pathNotConformed', tt('Section path is not conformed.'));
		head_add_js_var('uriNotMatch', tt('Given URI is not conformed.'));
	}
	
	private function renamePath(){
		$oldPath =  ROOT_PATH.'section/'.$this->dataByID['path'];
		$newPath =  ROOT_PATH.'section/'.pp('path');
		if(is_dir($oldPath)){
			if(is_file($oldPath.'/index.php')) unlink($oldPath.'/index.php');
			if(is_file($oldPath.'/.htaccess')) unlink($oldPath.'/.htaccess');
			@rename($oldPath, $newPath) or $success = false;
		}else{
			@mkdir($oldPath) or $success = false;
		}
		$this->writeScript($newPath.'/');
	}
	
	private function updateConfig(){
		$path = FILES_PATH.'config/config'.$this->id;
		$config = io_unserialize($path);
		$config['section_name'] = pp('name');
		$config['section_path'] = pp('path');
		io_serialize($config, $path);
	}
	
	private function initScript(){
		$success = true;
		$path = ROOT_PATH.'section/'.pp('path').'/';
		if(!is_dir($path)) @mkdir($path) or $success = false;
		if($success) Control::insert();
		if($success and !empty($this->id)){
			$this->writeScript($path);
		}else{
			$p = tt('Section can not be created. Please check permission of %s.');
			notify_add_error(sprintf($p, ROOT_PATH.'section'));
		}
	}
	
	private function writeScript($path){
		$script = sprintf(SECTION_SCRIPT, $this->id, pp('uri'), ROOT_PATH);
		$cron = sprintf(CRON_SCRIPT, $this->id, ROOT_PATH);
		io_write_file($path.'index.php', $script);
		io_write_file($path.'cron.php', $cron);
		copy($path.'.htaccess', ROOT_PATH.'.htaccess');
		copy($path.'cron-start.php', ROOT_PATH.'cron-start.php');
	}
	
	private function initConfig(){
		$path = FILES_PATH.'config/config'.$this->id;
		$config = config_check_default(array());
		$config['section_name'] = pp('name');
		$config['section_path'] = pp('path');
		io_serialize($config, $path);
	}
	
	private function initAdminMap(){
		$data = array(
			'users' => USER_ID,
			'groups' => 1,
			'section' => $this->id,
		);
		db_insert('map', $data);
	}
}
?>
