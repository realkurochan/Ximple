<?php
execute_add_class('forum', 'Forum');

include_once('ForumModel.class.inc');
include_once('forum.tool.inc');
include_once('forum.module.inc');

class Forum extends Control{
	function Forum(){
		$this->moduleTitle = 'Forum';
		$this->moduleName = 'forum';
		$this->tableName = 'forum';
		$this->setAdmin('forum_admin');
		$this->addAccessMethod('guest', 'index');
		$this->addAction('forum_manage', 'manage');
		$this->addAction('forum_save_sort', 'saveSort');
		$this->insertRedirect = 'forum_manage';
		$this->updateRedirect = 'forum_manage';
		$this->init(new ForumModel());
		$this->setConfigForm();
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		$clause = "WHERE mode='forum' AND section=".SECTION;
		$category = db_select('category', array('id', 'pic', 'category', 'description'), $clause);
		if(gg('category')){
			document_set_path($category[gg('category')]['category']);
		}
		$renderArray = array(
			'data' => $this->getAllowedData(gg('category')),
			'category' => $category,
		);
		head_add_css('forum.css');
		head_add_css('list.css');
		head_add_css('form.css');
		return render($renderArray, 'forum.tpl', 'forum');
	}
	
	public function manage(){
		document_set_path(tt('Manage Forum'));
		$clause = "WHERE mode='forum' AND section=".SECTION;
		$category = db_select_single_column('category', 'category', $clause);
		$category[0] = tt('Disable');
		$renderArray = array(
			'data' => $this->getSortedData(),
			'category' => $category,
		);
		head_add_css('forum.css');
		head_add_css('list.css');
		head_add_css('form.css');
		return render($renderArray, 'forum_edit.tpl', 'forum_edit');
	}
	
	public function drop(){
		execute_set_db();
		document_set_redirect('forum_manage');
		$pattern = tt('Forum is not allowed to be dropped. You can use <a href="%s">manage page</a> to disable Forum.');
		$message = sprintf($pattern, SECTION_URI.Q.'forum_manage');
		notify_add_error($message);
	}
	
	public function listPage(){
		return $this->manage();
	}
	
	public function saveSort(){
		execute_set_db();
		document_set_redirect('forum_manage');
		$forumID = pp('forum');
		$category = pp('category');
		foreach($forumID as $key => $value){
			$data = array(
				'position' => $key,
				'category' => $category[$key],
			);
			db_update('forum', $data, $value);
		}
	}
	
	private function getAllowedData(){
		if(gg('category')){
			$clause = "WHERE section=".SECTION.' AND category='.gg('category').' ORDER BY position';
		}else{
			$clause = "WHERE section=".SECTION.' ORDER BY position';
		}
		$this->select($this->column, $clause);
		$this->getCount();
		$allowedData = array();
		$allowedID = forum_get_readable_id();
		foreach($this->data as $key => $value){
			if(isset($value['category']['id']) and in_array($value['id'], $allowedID)){
				$value['last_entry'] = forum_get_last_entry($value['id']);
				$allowedData[$value['category']['id']][$key] = $value;
			}
		}
		return $allowedData;
	}
	
	private function getSortedData(){
		$clause = "WHERE section=".SECTION.' ORDER BY position';
		$this->select($this->column, $clause);
		$data = array();
		foreach($this->data as $key => $value){
			if(isset($value['category']['id'])){
				$data[$value['category']['id']][] = $value;
			}else{
				$data[0][] = $value;
			}
		}
		return $data;
	}
	
	private function getCount(){
		$dataID = array_keys($this->data);
		$countEntry = count_get_array('forum_entry_no', $dataID);
		$countComment = count_get_array('forum_comment_no', $dataID);
		foreach($this->data as $key => $value){
			$this->data[$key]['comment_no'] = $countComment[$key];
			$this->data[$key]['topic_no'] = $countEntry[$key];
		}
	}
	
	private function setConfigForm(){
		$appearance = $this->getNewConfigForm(tt('Permission'));
		$appearance->append('writable_group');
		$appearance->append('readable_group');
		$appearance->append('publishable_group');
	}
}
?>
