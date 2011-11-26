<?php
class PollConfigModel extends Model{
	function PollConfigModel(){
		$this->poll_result_color = $this->color('result bar color');
		$this->poll_comment_write_label = $this->title(tt('comment label'));
		$this->poll_comment_page_length = $this->spin(tt('comments number per page'), 5, 50);
	}
}
?>
