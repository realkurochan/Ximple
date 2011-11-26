<?php
require_once('blog.tool.inc');

class BlogConfigModel extends ContentConfigModel{
	function BlogConfigModel(){
		ContentConfigModel::ContentConfigModel('blog');
		$this->blog_icon_size = $this->spin(tt('icon size'), 16, 1024);
		
		$option = array(
			'avatar' => tt('show avatar'),
			'category_icon' => tt('show category icon'),
			'comment_avatar' => tt('show avatar for comments'),
			'signature' => tt('user signature'),
			'sibling' => tt('next/previous entry'),
		);
		$this->blog_appearance = $this->checkbox(tt('show in rendered blog'), $option);
		$this->blog_entries_number = $this->spin(tt('blog entries per page'), 5, 50);
		$this->extendBefore('blog_comment_write_label', 'blog_entries_number');
	}
	
	public function getPublishingStatus(){
		return blog_get_publishing_status();
	}
}
?>
