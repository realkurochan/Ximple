<?php
class AccountConfigModel extends Model{
	private $uid;
	function AccountConfigModel($uid){
		$this->uid = $uid;
		$this->signature = $this->char(tt('signature'));
		$this->signature->data = user_info_get_config($uid, 'signature');
		
		$this->language = $this->select(tt('default language for my user interface'), translate_get_interface_language());
		$this->language->data = user_info_get_config($uid, 'language');
		
		$subscribeOption = notify_get_subscribable_module();
		$this->subscribe = $this->checkbox(tt('send me an email by'), $subscribeOption);
		$this->subscribe->data = $this->getSubscriptionConfig();
		
		$permissionOption = $this->getProfilePermissionOption();
		$this->profile_permission = $this->radio(tt('who can see my profile'), $permissionOption);
		$this->profile_permission->data = user_info_get_config($uid, 'profile_permission');
		
		$this->email_permission = $this->radio(tt('who can send me an email'), $permissionOption);
		$this->email_permission->data = user_info_get_config($uid, 'email_permission');
	}
	
	private function getProfilePermissionOption(){
		return array(
			0 => tt('Everybody included anonymous visitor'),
			1 => tt('All authenticated users'),
			2 => tt('Only you and administration'),
		);
	}
	
	private function getSubscriptionConfig(){
		$subscribe = notify_get_subscribable_module();
		$directory = notify_create_subscribe_directory();
		$config = array();
		foreach($subscribe  as $key => $value){
			$subscriber = io_unserialize($directory.$key);
			if(isset($subscriber[$this->uid])) $config[] = $key;
		}
		return $config;
	}
}
?>
