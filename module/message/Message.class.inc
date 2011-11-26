<?php
execute_add_class('message', 'Message');
include('MessageModel.class.inc');

class Message extends Control{
	function Message(){
		$this->tableName = 'message';
		$this->moduleName = 'message';
		$this->moduleTitle = tt('My Message');
		$this->ownerColumn = 'receiver';
		$this->addAccessMethod('user', 'index');
		$this->addAccessMethod('user', 'write');
		$this->addAccessMethod('user', 'insert');
		$this->writeLabel = tt('Send a Message');
		$this->init(new MessageModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		document_set_title(tt('My Message'));
		$this->selectPage($this->column, 20, 'WHERE receiver='.USER_ID.' ORDER BY id DESC');
		
		foreach($this->data as $key => $value){
			if($value['status'] == 'new'){
				db_update('message', array('status' => 'old'), $value['id']);
			}
		}
		
		head_add_css('message.css');
		$render = render(array('data' => $this->data), 'message.tpl', 'message');
		$render.= $this->pager->render('message');
		return $render;
	}

	public function insert(){
		$userID = userinfo_name_to_id(input_split_comma(pp('receiver')));
		$data = pp();
		unset($data['sendback']);
		$this->implementSend($userID, $data, USER_NAME, pp('sendback'));
		notify_add_info(sprintf(tt('Your message to %s has been successfully sent'), pp('receiver')));
	}
	
	public function send($userID, $message, $senderID=USER_ID, $senderName=USER_NAME){
		$data = array(
			'message' => $message,
			'sender' => $senderID,
			'status' => 'new',
			'write_time' => NOW,
			'section' => SECTION,
		);
		$this->implementSend($userID, $data, $senderName);
	}
	
	private function implementSend($userID, $data, $senderName, $sendBack=false){
		$notify = sprintf(tt('You have <a href="%s">new message</a> from %s.'), SECTION_URI.Q.'message', $senderName);
		foreach($userID as $id){
			$data['receiver'] = $id;
			notify_add_info($notify, $id);
			db_insert('message', $data);
		}
		if($sendBack and !in_array(USER_ID, $userID)){
			$data['receiver'] = USER_ID;
			db_insert('message', $data);
		}
		execute_set_db();
		document_set_redirect('message');
	}
}
?>
