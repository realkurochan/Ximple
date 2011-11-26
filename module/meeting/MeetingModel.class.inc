<?php
class MeetingModel extends ContentModel{
	function MeetingModel(){
		ContentModel::ContentModel('meeting');
		$this->checkPath();
		$this->icon = $this->imageLoader(tt('icon'), 'files/meeting/', cc('meeting_icon_size'));
		
		$this->meeting_date = $this->timeStamp(tt('meeting date time'));
		$this->meeting_date->dateFormat = 'Y-m-d H:i:s';
		
		$this->meeting_period = $this->timeInterval(tt('meeting period'));
		$this->meeting_period->defaultData = 7200;
		$this->meeting_period->isDay = false;
		$this->meeting_period->isSecond = false;
		
		$this->summary = $this->text(tt('summary'));
		$this->summary->isForm = false;
		
		$this->participant_group = $this->group('');
		$this->participant_group->defaultData = cc('meeting_default_group');
		$this->participant_group->isExceptedGuest = true;
		$this->participant_group->isExceptedAdmin = false;
		
		$this->publish_time = $this->now();
		$this->publish_time->isUpdateTime = true;
		
		$this->extendAfter('topic', 'icon');
		$this->extendAfter('topic', 'meeting_date');
		$this->extendAfter('topic', 'meeting_period');
		$this->extendAfter('content', 'summary');
	}
	
	private function checkPath(){
		$path  = FILES_PATH.'meeting/';
		if(!is_dir($path))  mkdir($path);
	}
}
?>
