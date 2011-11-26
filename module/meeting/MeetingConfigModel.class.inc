<?php
class MeetingConfigModel extends ContentConfigModel{
	function MeetingConfigModel(){
		ContentConfigModel::ContentConfigModel('meeting');
		$this->meeting_default_group = $this->group(tt('default participant groups'));
		$this->meeting_default_group->isExceptedGuest = true;
		$this->meeting_default_group->isExceptedAdmin = false;
		$this->meeting_icon_size = $this->spin(tt('icon size'), 16, 1024);
		$this->meeting_permission = $this->group('allowed group');
		
		$this->meeting_invitation_message = $this->shortText(tt('invitation text'));
		$option = array(
			'avatar' => tt('show avatar'),
			'category_icon' => tt('show category icon'),
			'comment_avatar' => tt('show avatar for comments'),
			'signature' => tt('user signature'),
		);
		$this->meeting_appearance = $this->checkbox(tt('show in rendered meeting'), $option);
	}
}
?>
