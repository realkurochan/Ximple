<?php
class BlogModel extends ContentModel{
	function BlogModel(){
		ContentModel::ContentModel('blog');
		$this->checkPath();
		$this->icon = $this->imageLoader(tt('icon'), 'files/blog/', cc('blog_icon_size'));
		$this->intro = $this->text(tt('introduction'));
		$this->intro->enableTranslate();
		$this->extendAfter('topic', 'icon');
		$this->extendAfter('topic', 'intro');
	}
	
	private function checkPath(){
		$path  = FILES_PATH.'blog/';
		if(!is_dir($path))  mkdir($path);
	}
	
	public function getPublishingStatus(){
		return blog_get_publishing_status();
	}
}
?>
