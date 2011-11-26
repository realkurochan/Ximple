<?php
class VideoModel extends ContentModel{
	function VideoModel(){
		ContentModel::ContentModel('video');
		$help = sprintf(tt('maximum file size : %s'), render_file_size(MAX_UPLOADED_FILE_SIZE)).', ';
		$help.= tt('support only .flv file.');
		$this->icon = $this->imageLoader(tt('icon'), 'files/video/', cc('video_icon_size'));
		$this->file = $this->file(tt('video file'), 'files/video/', $help);
		$this->size = $this->fileSize(tt('size'));
		$this->size->isList = true;
		$this->extendAfter('topic', 'icon');
		$this->extendAfter('topic', 'file');
	}
}
?>
