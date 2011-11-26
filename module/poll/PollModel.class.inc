<?php
require('PollChoiseModel.class.inc');
require_once(MODULE_PATH.'comment/comment.tool.inc');

class PollModel extends Model{
	function PollModel(){
		$isEmbed = (gg('is_embed') == true);
		
		$this->topic = $this->title(tt('topic'));
		$this->topic->listMode = 'poll';
		
		if($isEmbed){
			$this->description = $this->hidden('', 'text');
		}else{
			$this->description = $this->shortText(tt('description'));
		}
		
		$this->choises = $this->grid(tt('choises'))->setModel(new PollChoiseModel());
		$this->writer = $this->owner();
		
		$this->create_date = $this->now();
		$this->create_date->formatDateBySelect = true;
		$this->create_date->dateFormat = cc('time_format');
		
		$this->end_date = $this->calendar(tt('end date'));
		$this->end_date->defaultData = date('Y-m-d H:i:s', time() + 60*60*24*7);
		
		$this->comment_time = $this->now();
		
		$this->permission = $this->radio(tt('vote permission'), poll_permission_option());
		
		if($isEmbed){
			$this->comment_permission = $this->hidden(XIMPLE_COMMENT_DISABLE, 'int');
		}else{
			$this->comment_permission = $this->radio(tt('comment permission'), comment_permission_option());
		}
		
		$this->module = $this->hidden(gg('module'), 'char');
		$this->module_id = $this->hidden(gg('module_id'), 'int');
		$this->section = $this->section();
	}
}
?>
