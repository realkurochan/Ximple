<?php
require('PageStatusColumn.class.inc');

class PageModel extends ContentModel{
	function PageModel($parent){
		ContentModel::ContentModel('page');
		$this->comment_permission = new PageStatusColumn(tt('comment permission'), 'comment_permission');
		$this->publishing_status = new PageStatusColumn(tt('status'), 'publishing_status');
		$this->publishing_status->isList = true;
		$this->publishing_status->isTriggerSelect = true;
		$this->parent = $this->hidden($parent, 'int');
		$this->icon = $this->imageLoader(tt('icon'), 'files/directory/', 64);
		$this->author_name = $this->char(tt('author name'), tt('If blank, your login-name will be shown.'));
		$this->introduction = $this->shortText(tt('introduction'));
		$this->introduction->enableTranslate();
		$this->extendAfter('topic', 'icon');
		$this->extendAfter('topic', 'author_name');
		$this->extendAfter('topic', 'introduction');
	}
}
?>
