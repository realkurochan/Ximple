<?php
include_once(INCLUDE_PATH.'class/ConfigControl.class.inc');
include('ForgetPasswordModel.class.inc');

execute_add_class('forgetpassword', 'ForgetPassword');
if(!cc('forget_password')) error_denied();

class ForgetPassword extends ConfigControl{
	private $userID;
	private $fileName;
	private $key;
	
	function ForgetPassword(){
		$this->moduleName = 'forgetpassword';
		$this->moduleTitle = tt('Forgot Password');
		$this->addAction('forgetpassword_response', 'response');
		$this->addAccessMethod('guest', 'index');
		$this->addAccessMethod('guest', 'response');
		$this->init(new ForgetPasswordModel());
	}
	
	public function index(){
		$this->formMeta->description  = tt('Enter your email, which you use for your account. ');
		$this->formMeta->description .= tt('We will send you a link to this email. ');
		$this->formMeta->description .= tt('You can use this link to reset your password.');
		$this->formMeta->isCaptcha = true;
		$this->formMeta->mode = 'forgetpassword_response';
		$this->formMeta->title = $this->moduleTitle;
		return $this->render();
	}
	
	public function response(){
		if($this->checkEmailAddress()){
			$this->saveControlFile();
			$this->sendLink();
			notify_add_info('The link for password resetting has been already sent to the given email.');
		}
		execute_set_db();
	}
	
	private function checkEmailAddress(){
		include_once(INCLUDE_PATH.'tool/captcha.tool.inc');
		$gentime = pp('captcha_gentime_captcha');
		$parity = pp('captcha_code_captcha');
		$code = pp('captcha');
		if(captcha_check($gentime, $code, $parity)){
			$clause = "WHERE email='".addslashes((string) pp('email'))."'";
			$email = db_select_single_column('users', 'id', $clause, false);
			$exist = count($email);
			if($exist) $this->userID = $email[0];
			else notify_add_error(sprintf(tt('The given email : %s does not exist.'), pp('email')));
			return $exist;
		}else{
			notify_add_error(tt('Your given code from the image was wrong. Data is not inserted.'));
			return false;
		}
	}
	
	private function saveControlFile(){
		if($this->userID){
			$this->fileName = md5(rand()).sha1(rand());
			$this->key = sha1(rand()).md5(rand());
			$data = array(
				'uid' => $this->userID,
				'key' => $this->key,
			);
			$passwordDir = FILES_PATH.'forgetpassword/';
			if(!is_dir($passwordDir)) mkdir($passwordDir);
			io_serialize($data, $passwordDir.$this->fileName);
		}
	}
	
	private function sendLink(){
		if($this->userID){
			require_once(INCLUDE_PATH.'tool/mail.tool.inc');
			$receiver = pp('email');
			$subject = cc('section_name').' : '.tt('Reset your password.');
			$body  = tt('You can use the following link to reset your password : ')."\n";
			$body .= SECTION_URI.Q.'forgetpassword_reset/file/'.$this->fileName.'/key/'.$this->key;
			mail_send($receiver, $subject, $body);
		}
	}
}
?>
