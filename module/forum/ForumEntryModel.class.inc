<?php
include('ForumColumn.class.inc');

class ForumEntryModel extends Model{
	private $commentPermission;
	private $publishingStatus;
	
	function ForumEntryModel(){
		$moduleName = 'forum_entry';
		$this->topic = $this->title(tt('topic'));
		$this->topic->listMode = $moduleName;
		$this->forum = new ForumColumn();
		
		$this->writer = $this->owner();
		$this->writer->label = tt('writer');
		
		if(authority_is_guest()){
			$this->writer_name = $this->title(tt('your name'));
		}else{
			$this->writer_name = $this->hidden(USER_NAME, 'char');
			$this->writer_name->label = tt('writer');
			$this->writer_name->isList = true;
		}
		
		$this->write_time = $this->now();
		$this->write_time->dateFormat = cc('time_format');
		$this->write_time->formatDateBySelect = true;
		$this->update_time = $this->now();
		$this->update_time->isUpdateTime = true;
		$this->comment_time = $this->now();
		$this->comment_time->defaultData = '1970-01-01 12:00:00';
		$this->content = $this->text(tt('content'));
		$this->content->isReachable = true;
		$this->section = $this->section();
		$this->last_commentator = $this->hidden('', 'char');
		$this->last_commentator_id = $this->owner();
		
		if(!authority_is_guest()){
			$commentPermission = new ContentStatusColumn(tt('comment permission'), $moduleName);
			$commentPermission->setStatus('comment_permission');
			$commentPermission->setOption($this->getCommentPermission());
			$this->comment_permission = $commentPermission;
	
			$publishingStatus = new ContentStatusColumn(tt('status'), $moduleName);
			$publishingStatus->setStatus('publishing_status');
			$publishingStatus->setOption($this->getPublishingStatus());
			$publishingStatus->isList = true;
			$publishingStatus->isTriggerSelect = true;
			$this->publishing_status = $publishingStatus;
		}else{
			$this->comment_permission = $this->hidden(cc('forum_comment_permission_default'), 'int');
			$this->publishing_status = $this->hidden(cc('forum_publishing_status_default'), 'int');
		}
	}
	
	public function getCommentPermission(){
		return content_get_comment_permission();
	}
	
	public function getPublishingStatus(){
		return forum_get_publishing_status();
	}
}
?>
