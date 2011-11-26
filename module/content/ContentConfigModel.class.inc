<?php
abstract class ContentConfigModel extends Model{
	function ContentConfigModel($mode){
		$appearance = $mode.'_appearance';
		$option = array(
			'avatar' => tt('avatar'),
			'category_icon' => tt('category icon'),
			'comment_avatar' => tt('avatar for comments'),
			'signature' => tt('user signature'),
		);
		$this->$appearance = $this->checkbox(tt('show in rendered page'), $option);
		$this->$appearance->defaultData = array();
		
		$comment_write_label = $mode.'_comment_write_label';
		$this->$comment_write_label = $this->title(tt('label for comment write form'));
		$this->$comment_write_label->defaultData = tt('Please, write your opinion.');
		
		$comment_page_length = $mode.'_comment_page_length';
		$this->$comment_page_length = $this->spin(tt('comments number per page'), 5, 50);
		$this->$comment_page_length->defaultData = 10;
		
		$option = $this->getCommentPermission();
		$comment_permission = $mode.'_comment_permission';
		$this->$comment_permission = $this->checkbox(tt('available comment permission for contributor'), $option);
		$this->$comment_permission->defaultData = array(0, 1, 2, 3);
		
		$comment_permission_default = $mode.'_comment_permission_default';
		$this->$comment_permission_default = $this->radio(tt('default comment permission for contributor'), $option);
		$this->$comment_permission_default->defaultData = 0;
		
		$option = $this->getPublishingStatus();
		$publishing_status = $mode.'_publishing_status';
		$this->$publishing_status = $this->checkbox(tt('available publishing status for contributor'), $option);
		$this->$publishing_status->defaultData = array(2, 3);
		
		$publishing_status_default = $mode.'_publishing_status_default';
		$this->$publishing_status_default = $this->radio(tt('default publishing status for contributor'), $option);
		$this->$publishing_status_default->defaultData = 3;
	}
	
	public function getCommentPermission(){
		return content_get_comment_permission();
	}
	
	public function getPublishingStatus(){
		return content_get_publishing_status();
	}
}
?>
