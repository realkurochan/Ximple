<?php
class FAQModel extends Model{
	function FAQModel(){
		$this->owner = $this->owner();
		$this->sort_number = $this->hidden(0, 'int');
		
		$this->question = $this->title(tt('question'));
		$this->question->isList = true;
		$this->question->listMode = 'faq#';
		$this->question->isReachable = true;
		$this->question->enableTranslate();
		
		$this->category = $this->category('faq');
		
		$option = array(
			0 => tt('draft'),
			1 => tt('publish'),
		);
		$this->status = $this->select(tt('status'), $option);
		$this->status->isList = true;
		
		$this->answer = $this->text(tt('answer'));
		$this->answer->enableTranslate();
		
		$this->section = $this->section();
	}
}
?>
