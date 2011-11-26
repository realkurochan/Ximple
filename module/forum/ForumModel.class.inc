<?php
class ForumModel extends Model{
	function ForumModel(){
		$this->name = $this->title(tt('forum name'));
		$this->category = $this->category('forum');
		$this->description = $this->plain(tt('description'));
		$help = tt('Check guest to allow everyone.');
		$this->writable_group = $this->group(tt('writable group'), $help);
		$this->readable_group = $this->group(tt('readable group'), $help);
		$this->publishable_group = $this->group(tt('publishable group'), $help);
		$this->position = $this->hidden(0, 'int');
		$this->last_posted = $this->hidden(0, 'int');
		$this->last_commented = $this->hidden(0, 'int');
		$this->section = $this->section();
	}
	
	public function triggerPostDBIntert(){
		$this->updateSubscription();
	}
	
	public function triggerPostDBUpdate(){
		$this->updateSubscription();
	}
	
	private function updateSubscription(){
		$clause = 'WHERE section='.SECTION;
		$forum = db_select_single_column('forum', 'name', $clause);
		foreach($forum as $id => $name){
			notify_add_subscribe('forum_'.$id, tt('someone posts in forum : ', true).$name);
			notify_add_subscribe('forum_comment_'.$id, tt('someone comments in forum : ', true).$name);
		}
		config_save();
	}
}
?>
