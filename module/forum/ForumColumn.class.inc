<?php
require_once(INCLUDE_PATH.'class/column/SelectColumn.class.inc');
require_once(INCLUDE_PATH.'class/column/HiddenColumn.class.inc');

class ForumColumn extends SelectColumn{
	private $isSelect;
	
	function ForumColumn(){
		$forumID = gg('forum');
		if(!$forumID) $this->setAsSelect();
		else $this->setAsHidden($forumID);
		$this->setType('int');
		$this->option = array();
	}
	
	public function renderForm($formID){
		if($this->isSelect){
			return SelectColumn::renderForm($formID);
		}else{
			$hidden = new HiddenColumn($this->defaultData, 'int');
			$hidden->name = $this->name;
			return $hidden->renderForm($formID);
		}
	}
	
	public function triggerInsertForm(){
		if($this->isSelect) $this->getOption();
	}
	
	public function triggerUpdateForm(){
		if($this->isSelect) $this->getOption();
	}
	
	private function setAsHidden($forumID){
		$this->defaultData = $forumID;
		$this->isSelect = false;
	}
	
	private function setAsSelect(){
		SelectColumn::SelectColumn(tt('forum'), array());
		$this->isSelect = true;
	}
	
	private function getOption(){
		$forumID = forum_get_writable_id();
		if(count($forumID)){
			$clause = 'WHERE id='.join(' OR id=', $forumID);
			$this->option = db_select_single_column('forum', 'name', $clause);
		}elseif(forum_check_authority_group()){
			$this->option = array();
			$pattern = tt('No Forum is created, please <a href="%s">create forum</a> before write a new entry.');
			$message = sprintf($pattern, SECTION_URI.Q.'forum_manage');
			notify_add_error($message);
		}else{
			$this->option = array();
			error_denied();
		}
	}
}
?>
