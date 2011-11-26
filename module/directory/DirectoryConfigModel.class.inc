<?php
require_once(MODULE_PATH.'content/content.tool.inc');
require_once(INCLUDE_PATH.'class/column/CheckBoxColumn.class.inc');
require('ShowItemColumn.class.inc');
require('ShowPageColumn.class.inc');
require('ShowDirectoryColumn.class.inc');

class DirectoryConfigModel extends Model{
	function DirectoryConfigModel(){
		$this->show_item = new ShowItemColumn();
		$this->extension_mode = $this->char(tt('extension URI'));
		
		$this->page_list_label = $this->char(tt('label for list of pages'));
		$this->page_list_label->enableTranslate();
		
		$this->related_page_label  = $this->char(tt('label for related pages'));
		$this->related_page_label->enableTranslate();
		
		$option = array(
			'introduction' => tt('description with icon'),
			'related' => tt('show related pages by rendered page'),
			'sibling' => tt('show sibling pages by rendered page'),
			'avatar' => tt('writer avatar'),
			'category' => tt('category icon'),
		);
		$this->page_render_item = $this->checkbox(tt('show in rendered page'), $option);
		$this->show_page = new ShowPageColumn();
		
		$this->directory_list_label = $this->char(tt('label for list of sub-directories'));
		$this->directory_list_label->enableTranslate();
		
		$this->show_directory = new ShowDirectoryColumn();
		
		$option = content_get_publishing_status();
		$this->publishing_status = $this->checkbox(tt('available publishing status for contributor'), $option);
		$this->publishing_status_default = $this->radio(tt('default comment permission for contributor'), $option);
		
		$option = array(5 => 5, 10 => 10, 15 => 15, 20 => 20, 50 => 50);
		$this->comment_page_length = $this->select(tt('number of shown comments per page'), $option);
		$this->comment_show_avatar = $this->enable(tt('enable avatar by comment'));
		$this->comment_write_label = $this->title(tt('label for comment write form'));
		$this->comment_write_label->enableTranslate();
		
		$option = content_get_comment_permission();
		$this->comment_permission = $this->checkbox(tt('available publishing status for contributor'), $option);
		$this->comment_permission_default = $this->radio(tt('default publishing status for contributor'), $option);
		
	}
}
?>
