<?php
class DownloadModel extends Model{
	function DownloadModel(){
		$this->name = $this->title(tt('name'));
		$this->icon = $this->imageLoader(tt('icon'), 'files/download/', 64);
		$this->file = $this->file(tt('upload file'), 'files/download/');
		$this->size = $this->fileSize(tt('size'));
		$this->category = $this->category('download');
		$this->description = $this->shortText(tt('description'));
		$this->description->isReachable = true;
		$this->upload_time = $this->now();
		$this->permission = $this->group(tt('allowed group'), tt('Check guest to allow everybody.'));
		$this->uploader = $this->owner();
		$this->section = $this->section();
	}
}
?>
