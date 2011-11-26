<?php
execute_add_class('contact', 'Contact');
include('ContactModel.class.inc');

class Contact extends Control{
	private $attachment;
	
	function Contact(){
		$this->tableName = 'contact';
		$this->moduleName = 'contact';
		$this->moduleTitle = tt('Contact');
		$this->setAdmin('contact_admin');
		$this->addAction('contact_send', 'send');
		$this->addAccessMethod('guest', 'index');
		$this->addAccessMethod('guest', 'send');
		document_set_path($this->moduleTitle, $this->moduleName);
		$this->init(new ContactModel());
	}
	
	public function index(){
		if(MODE_ID and MODULE == $this->moduleName){
			return $this->renderContactForm();
		}else{
			$this->selectListPage(20, 'WHERE (section='.SECTION.')');
			$this->translateData();
			return $this->renderDefaultListPage();
		}
	}
	
	public function send(){
		include_once(INCLUDE_PATH.'tool/mail.tool.inc');
		$this->id = (int)pp('id');
		$this->selectByID($this->column);
		$this->translateDataByID();
		$this->uploadAttachment();
		$authority = authority(array(), $this->dataByID['permission']);
		if($authority and $this->checkCaptcha()){
			$this->sendReply();
			$this->sendContact();
			$groups = cc('contact_message_receiver_group');
			if(count($groups)) $this->sendMessage($groups);
			notify_add_info(tt('Your message has been already sent to the administrator of the web-site. Thank you for your opinion.'));
			execute_set_db();
			document_set_redirect('contact');
		}else{
			$this->denied();
		}
	}
	
	public function insert(){
		Control::insert();
		$this->addNavigation();
	}
	
	public function update(){
		Control::update();
		$this->addNavigation();
	}
	
	public function drop(){
		Control::drop();
		$this->dropNavigation();
	}
	
	private function renderContactForm(){
		include('ContactForm.class.inc');
		$this->id = mid();
		$this->selectByID($this->column, true);
		$this->translateDataByID();
		if(authority(array(), $this->dataByID['permission'])){
			$form = new ContactForm($this->id, $this->dataByID['name'], $this->dataByID['enable_attachment']);
			return $form->render($this->dataByID['description']);
		}else{
			$this->denied();
		}
	}
	
	private function addNavigation(){
		include_once(INCLUDE_PATH.'tool/widget.tool.inc');
		widget_add_navigation('contact/'.$this->id, pp('name'));
		config_save();
		notify_add_tip(tt('Contact has been automatically inserted into Navigation-Widget.'));
	}
	
	private function dropNavigation(){
		include_once(INCLUDE_PATH.'tool/widget.tool.inc');
		widget_drop_navigation('contact/'.$this->id);
		config_save();
		notify_add_tip(tt('Contact has been automatically deleted from Navigation-Widget.'));
	}
	
	private function sendReply(){
		$subject = cc('contact_subject');
		$message = $this->dataByID['reply_message'];
		$message.= "<blockquote>".$_POST['message']."</blockquote>";
		$result = mail_send(pp('email'), $subject, $message, true, null);
		if($result !== true) notify_add_error($result);
	}
	
	private function sendContact(){
		$subject = cc('contact_head').' '.$this->dataByID['name'].' : '.pp('subject');
		$message = sprintf('<p>'.tt('Email from %s').'</p>', pp('email')).pp('message');
		$receiver = $this->dataByID['email_list'];
		$result = mail_send($receiver, $subject, $message, true, $this->attachment);
		if(is_file(ROOT_PATH.$this->attachment)) unlink(ROOT_PATH.$this->attachment);
		if($result !== true) notify_add_error($result);
	}
	
	private function sendMessage($groups){
		$body = '<p>'.tt('This message is generated from contact module.').'</p>';
		$body.= '<h4>'.cc('contact_head').pp('subject').'</h4>';
		$body.= '<p>'.pp('message').'</p><p>from '.pp('email').'</p>';
		
		include_once(MODULE_PATH.'message/Message.class.inc');
		$message = new Message();
		$message->send(userinfo_group_to_id($groups), $body, 1, pp('email'));
	}
	
	private function denied(){
		execute_set_db();
		document_set_redirect('contact');
		notify_add_error(tt('You are not allowed to use the Contact-Form.'));
	}
	
	private function uploadAttachment(){
		if($this->dataByID['enable_attachment']){
			$this->attachment = io_upload(ff('attachment'), 'files/contact/', false, false);
		}
	}
	
	private function checkCaptcha(){
		return $this->dataModifier->checkCaptcha();
	}
}
?>
