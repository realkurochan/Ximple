<?php
class VideoConfigModel extends ContentConfigModel{
	function VideoConfigModel(){
		ContentConfigModel::ContentConfigModel('video');
		$this->video_icon_size = $this->spin(tt('icon size'), 16, 1024);
		$this->video_width = $this->spin(tt('player width'), 120, 3200);
		$this->video_height = $this->spin(tt('player height'), 120, 3200);
		$this->video_entries_number = $this->spin(tt('number of shown video entries per page'), 5, 50);
		$this->extendBefore('video_show_avatar', 'video_width');
		$this->extendBefore('video_show_avatar', 'video_height');
		$this->extendBefore('video_comment_write_label', 'video_entries_number');
	}
}
?>
