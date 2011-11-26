<?php
require('ConfigureColumn.class.inc');
require('PageColumn.class.inc');

class PageDirectoryModel extends Model{
	function PageDirectoryModel($parent){
		$this->name = $this->title(tt('directory name'));
		$this->name->listMode = 'directory';
		$this->name->enableTranslate();

		$this->uri = $this->char(tt('URI'));
		$this->category = $this->category('page');
		$this->index_page = new PageColumn();
		$this->icon = $this->imageLoader(tt('icon'), 'files/directory/', 64);

		$this->description = $this->shortText(tt('description'));
		$this->description->enableTranslate();

		$this->permission = $this->group(tt('editor group'));
		$this->permission->isExceptedGuest = true;
		$this->permission->isExceptedAdmin = true;
		$this->permission->help = tt('User-Groups, which can create Sub-Directory and Page in this Directory.');

		$this->parent = $this->hidden($parent, 'int');

		$this->owner = $this->owner();
		$this->owner->label = tt('created by');

		$this->configuration = new ConfigureColumn();
		$this->create_time = $this->now();
		$this->section = $this->section();
	}
}
?>
