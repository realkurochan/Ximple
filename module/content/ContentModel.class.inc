<?php
include_once(MODULE_PATH.'freetag/FreeTagColumn.class.inc');
include_once('ContentStatusColumn.class.inc');

abstract class ContentModel extends Model{
	private $commentPermission; 
	private $publishingStatus;
	
	function ContentModel($moduleName){
		$this->topic = $this->title(tt('topic'));
		$this->topic->listMode = $moduleName;
		$this->topic->enableTranslate();
		$this->writer = $this->owner();
		$this->writer->label = tt('writer');
		
		$this->write_time = $this->now();
		
		$this->update_time = $this->now();
		$this->update_time->isUpdateTime = true;
		
		$this->publish_time = $this->calendar(tt('publish date'));
		$this->comment_time = $this->hidden('0000-00-00', 'timestamp');
		
		$this->content = $this->text(tt('content'));
		$this->content->isReachable = true;
		$this->content->enableTranslate();
		
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
		
		$help = tt('You can use "extended URI" to increase search engine optimozation. ');
		$this->uri = $this->char(tt('extended URI'), $help);
		$this->uri->isReachable = true;
		$help = tt('Short description about this content for search engine optimization. ');
		$this->description = $this->plain(tt('description'), $help);
		$this->description->isReachable = true;
		$this->category = $this->category($moduleName);
		$this->freetag = new FreeTagColumn($moduleName);
		$this->section = $this->section();
	}
	
	public function getCommentPermission(){
		return content_get_comment_permission();
	}
	
	public function getPublishingStatus(){
		return content_get_publishing_status();
	}
}
?>
