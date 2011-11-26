<?php
class ActivityConfigModel extends ContentConfigModel{
	function ActivityConfigModel(){
		ContentConfigModel::ContentConfigModel('activity');
		$this->activity_icon_size = $this->spin(tt('icon size'), 16, 1024);
		
		$option = array(
			'avatar' => tt('show avatar'),
			'category_icon' => tt('show category icon'),
			'comment_avatar' => tt('show avatar for comments'),
			'signature' => tt('user signature'),
		);
		$this->activity_appearance = $this->checkbox(tt('show in rendered activity'), $option);
	}
}
?>
