<?php
class AboutModel extends Model{
	function AboutModel(){
		$this->title = $this->title(tt('heading'));
		$this->title->enableTranslate();
		$this->body = $this->text(tt('text'));
		$this->body->enableTranslate();
	}
}
?>
