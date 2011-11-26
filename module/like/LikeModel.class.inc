<?php
class LikeModel extends Model{
	function LikeModel(){
		$this->module = $this->hidden(null, 'char');
		$this->module_id = $this->hidden(null, 'int');
		$this->liker = $this->hidden(null, 'int');
	}
}
?>
