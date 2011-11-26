<?php
execute_add_class('like', 'Like');
require_once('LikeModel.class.inc');

class Like extends Control{
	private $module;
	private $moduleID;
	private $ownerID;
	
	function Like(){
		$this->moduleName = 'like';
		$this->moduleTitle = tt('Like');
		$this->tableName = 'like_it';
		$this->init(new LikeModel());
		$this->setAction();
		$this->setAccess();
	}
	
	public function clickLike(){
		execute_set_ajax();
		$this->module = gg('module');
		$this->moduleID = (int) gg('module_id');
		$this->owner = (int) gg('owner');
		$clause = "WHERE module='$this->module' AND module_id=$this->moduleID AND liker=".USER_ID;
		$this->select(array('id'), $clause);
		
		if(count($this->data) == 0){
			$data = array(
				'module' => $this->module,
				'module_id' => $this->moduleID,
				'liker' => USER_ID,
			);
			db_insert($this->tableName, $data);
			$this->notifyRelated();
			$link = $this->renderUnlikeLink(gg('module'), (int) gg('module_id'));
			return $link['like'];
		}
	}
	
	public function clickUnlike(){
		execute_set_ajax();
		$this->module = gg('module');
		$this->moduleID = (int) gg('module_id');
		$this->owner = (int) gg('owner');
		$clause = "module='$this->module' AND module_id=$this->moduleID AND liker=".USER_ID;
		db_drop_many($this->tableName, $clause);
		$link = $this->renderLikeLink(gg('module'), (int) gg('module_id'));
		return $link['like'];
	}
	
	public function showUsers(){
		execute_set_ajax();
		$this->module = gg('module');
		$this->moduleID = (int) gg('module_id');
		$clause = "WHERE module='$this->module' AND module_id=$this->moduleID";
		$this->select(array('liker'), $clause);
		$this->data = userinfo_get_info($this->data, 'liker');
		$data = array();
		foreach($this->data as $key => $value){
			if(strlen($value['liker']['avatar']) == 0){
				$value['liker']['avatar'] = 'files/default.png';
			}
			$data[$key] = $value['liker'];
		}
		$renderArray = array(
			'data' => $data,
		);
		return render($renderArray, 'like_user.tpl', 'like_user');
	}
	
	public function embed($parameter, $result){
		head_add_css('like.css');
		$this->module = $parameter['module'];
		$this->moduleID = $parameter['id'];
		$this->ownerID = $parameter['owner'];
		$clause = "WHERE module='$this->module' AND module_id=$this->moduleID AND liker=".USER_ID;
		$this->select(array('id'), $clause);
		if(count($this->data)){
			return $this->renderUnlikeLink();
		}else{
			return $this->renderLikeLink();
		}
	}
	
	private function renderLikeLink(){
		head_add_js('module/like/like.js');
		if(authority_is_guest()){
			$result['like']  = '';
		}else{
			$result['like']  = $this->renderLink('like', tt('like'));
		}
		$result['like'] .= $this->renderShowPeopleLink();
		return $result;
	}
	
	private function renderUnlikeLink(){
		head_add_js('module/like/like.js');
		if(authority_is_guest()){
			$result['like']  = '';
		}else{
			$result['like']  = $this->renderLink('unlike', tt('unlike'));
		}
		$result['like'] .= $this->renderShowPeopleLink();
		return $result;
	}
	
	private function renderShowPeopleLink(){
		$clause = "WHERE module='$this->module' AND module_id=$this->moduleID";
		$data = db_get_rows_number($this->tableName, $clause);
		if($data){
			$label = sprintf(tt('%d people like it'), $data);
			$render = $this->renderLink('showLikePeople', $label, '_show');
			return $render;
		}
	}
	
	private function renderLink($function, $label, $extension=''){
		$id = 'like_'.$this->module.'_'.$this->moduleID.$extension;
		$onclick = "$function('$this->module', '$this->moduleID', '$this->ownerID')";
		return '<a href="#" onclick="return '.$onclick.'" id="'.$id.'">'.$label.'</a>';
	}
	
	private function notifyRelated(){
		$uri = SECTION_URI.Q.$this->module.'/'.$this->moduleID;
		$message = sprintf(tt('%s likes your <a href="%s">contribution</a>.'), USER_NAME, $uri);
		
		$subscriber = io_unserialize(FILES_PATH.'subscribe/'.SECTION.'/like');
		if(is_array($subscriber)) $subscriber = array_keys($subscriber);
		else $subscriber = array();
		
		$clause = "WHERE module='$this->module' AND module_id=$this->moduleID";
		$this->select(array('liker'), $clause);
		$uid = array();
		$uid[$this->ownerID] = true;
		foreach($this->data as $key => $value){
			$uid[$value['liker']] = true;
		}
		foreach($uid as $key => $value){
			notify_add_info($message, $key);
		}
		$subscriber = array_intersect($subscriber, array_keys($uid));
		if(count($subscriber)){
			notify_send_mail($message, userinfo_get_email($subscriber));
		}
	}
	
	private function setAction(){
		$this->unsetAction();
		$this->addAction('like', 'clickLike');
		$this->addAction('like_unlike', 'clickUnlike');
		$this->addAction('like_embed', 'embed');
		$this->addAction('like_users', 'showUsers');
	}
	
	private function setAccess(){
		$this->addAccessMethod('guest', 'embed');
		$this->addAccessMethod('guest', 'showUsers');
		$this->addAccessMethod('user', 'clickLike');
		$this->addAccessMethod('user', 'clickUnlike');
	}
}
?>
