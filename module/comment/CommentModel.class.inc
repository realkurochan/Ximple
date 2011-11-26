<?php
class CommentModel extends Model{
	function CommentModel($mode, $parent, $parentWriter){
		$this->parent = $this->hidden($parent, 'int');
		$this->parent->isList = true;
		$this->parent->label = tt('commented in');
		if(authority_is_guest()){
			$this->writer = $this->title(tt('Your name'));
			$this->email = $this->email(tt('Your email (will not be shown)'));
			$this->homepage = $this->char(tt('Your blog/website/homepage (optional)'));
		}else{
			$this->writer = $this->hidden(USER_NAME, 'char');
			$this->writer->label = tt('commented by');
			$this->writer->isList = true;
			$this->writer->isReachable = true;
			$this->email = $this->hidden();
			$this->homepage = $this->hidden();
		}
		$this->writer_id = $this->owner();
		$this->comment = $this->shortText(tt('Your opinion'));
		$this->write_time = $this->now(cc('time_format'));
		$this->write_time->label = tt('comment time');
		$this->write_time->isList = true;
		$this->mode = $this->hidden($mode, 'char');
		$this->parent_writer = $this->hidden($parentWriter, 'int');
		
		$this->parent_topic_column = $this->hidden(null);
		$this->parent_topic_column->isDB = false;
		
		$this->parent_writer_column = $this->hidden(null);
		$this->parent_writer_column->isDB = false;
		
		$this->parent_uri_column = $this->hidden(null);
		$this->parent_uri_column->isDB = false;
		
		$this->parent_has_uri_column = $this->hidden(null);
		$this->parent_has_uri_column->isDB = false;
		
		$this->parent_permission_column = $this->hidden(null);
		$this->parent_permission_column->isDB = false;
		
		$this->section = $this->section();
		
		$this->parent_topic_column = $this->hidden('topic', 'char');
		$this->parent_topic_column->isDB = false;
		
		$this->parent_writer_column = $this->hidden('writer', 'char');
		$this->parent_writer_column->isDB = false;
		
		$this->parent_uri_column = $this->hidden('uri', 'char');
		$this->parent_uri_column->isDB = false;
		
		$this->parent_permission_column = $this->hidden('comment_permission', 'char');
		$this->parent_permission_column->isDB = false;
	}
}
?>
