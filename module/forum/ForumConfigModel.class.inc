<?php
require_once('forum.tool.inc');

class ForumConfigModel extends ContentConfigModel{
	function ForumConfigModel(){
		ContentConfigModel::ContentConfigModel('forum');
		$option = array(
			'avatar' => tt('show avatar'),
			'comment_avatar' => tt('show avatar for comments'),
			'level' => tt('show user level'),
			'signature' => tt('user signature'),
		);
		
		$this->forum_appearance = $this->checkbox(tt('show in rendered forum'), $option);
		$this->forum_user_level = $this->grid(tt('User Levels'));
		$userLevel = cc('forum_user_level');
		$userLevelNumber = Model::char(tt('contribution number'));
		$userLevelName = Model::char(tt('level name'));
		if(is_array($userLevel)){
			$userLevelNumber->data = array_keys($userLevel);
			$userLevelName->data = array_values($userLevel);
		}
		$this->forum_user_level->append('forum_user_level_number', $userLevelNumber);
		$this->forum_user_level->append('forum_user_level_name', $userLevelName);
	}
	
	public function getPublishingStatus(){
		return forum_get_publishing_status();
	}
}
?>
