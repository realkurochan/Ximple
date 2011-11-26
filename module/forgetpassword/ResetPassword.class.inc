<?php
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('ResetPasswordModel.class.inc');

execute_add_extended_class('forgetpassword', 'reset', 'ResetPassword');
if(!cc('forget_password')) error_denied();

class ResetPassword extends ConfigControl{
	private $userID;
	private $fileName;
	
	function ResetPassword(){
		$this->moduleName = 'forgetpassword';
		$this->moduleTitle = tt('Reset Password');
		$this->addAction('forgetpassword_reset', 'index');
		$this->addAction('forgetpassword_reset_save', 'save');
		$this->addAccessMethod('guest', 'index');
		$this->addAccessMethod('guest', 'save');
		$this->init(new ResetPasswordModel());
	}
	
	public function index(){
		if($this->checkKey()) return $this->renderForm();
		else $this->invalidLink();
	}
	
	public function save(){
		include_once(INCLUDE_PATH.'tool/captcha.tool.inc');
		$gentime = pp('captcha_gentime_captcha');
		$parity = pp('captcha_code_captcha');
		$code = pp('captcha');
		execute_set_db();
		if(captcha_check($gentime, $code, $parity)){
			if($this->checkKey()){
				db_update('users', array('password' => pp('password')), $this->userID);
				unlink($this->fileName.'.php');
				notify_add_info(tt('Your password has been already changed, please login.'));
			}else{
				$this->invalidLink();
			}
		}else{
			notify_add_error(tt('Your given code from the image was wrong. Data is not inserted.'));
		}
	}
	
	private function renderForm(){
		$this->formMeta->id = 'user';
		$this->formMeta->mode = 'forgetpassword_reset_save/file/'.gg('file').'/key/'.gg('key');
		$this->formMeta->title = $this->moduleTitle;
		$this->formMeta->isCaptcha = true;
		head_add_js('module/user/user.js');
		head_add_js_var('insertUser', true);
		head_add_js_var('passwdTooShort', tt('Password is too short.'));
		head_add_js_var('passwdVeryStrong', tt('Password strength is very strong.'));
		head_add_js_var('passwdStrong', tt('Password strength is strong.'));
		head_add_js_var('passwdMedium', tt('Password strength is medium.'));
		head_add_js_var('passwdWeak', tt('Password strength is weak.'));
		head_add_js_var('passwdMismatch', tt('Password is mismathed.'));
		return $this->render();
	}
	
	private function checkKey(){
		if(count(split('\/\.\.', gg('filename'))) >= 2) error_denied();
		$this->fileName = FILES_PATH.'forgetpassword/'.gg('file');
		$controlData = io_unserialize($this->fileName);
		if(is_array($controlData)){
			if($controlData['key'] == gg('key')){
				$this->userID = $controlData['uid'];
				return true;
			}
		}
		return false;
	}
	
	private function invalidLink(){
		notify_add_error(tt('The requested URI for password resetting is invalid.'));
		execute_set_db();
	}
}
?>
