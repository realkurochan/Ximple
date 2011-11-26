<?php
execute_add_class('poll', 'Poll');

require('PollModel.class.inc');
require('poll.tool.inc');

class Poll extends Control{
	private $comment;
	
	function Poll(){
		$this->tableName = 'poll';
		$this->moduleName = 'poll';
		$this->moduleTitle = tt('Poll');
		$this->setAccess();
		$this->redirectToIDPage = true;
		$this->init(new PollModel());
		$this->setConfigForm();
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		$this->id = (int) gg('id');
		if($this->id){
			$this->selectByID($this->column);
			count_increase($this->fullModuleName, $this->id);
			if($this->isVotable($this->id)) return $this->renderVotePage();
			else return $this->renderResultPage();
		}else{
			return $this->listPage();
		}
	}
	
	public function listPage(){
		document_set_path(tt('List of Polls'));
		$this->selectListPage(20, 'WHERE section='.SECTION);
		return $this->renderDefaultListPage();
	}
	
	public function vote(){
		$this->id = (int) gg('id');
		$this->selectByID(array('permission', 'module', 'module_id'));
		if($this->isVotable($this->id)){
			$choise = (int) pp('choise');
			$this->increaseChoise($choise);
			$this->checkVote($this->id);
		}
		execute_set_db();
		if(strlen($this->dataByID['module'])){
			document_set_redirect($this->dataByID['module'].'/'.$this->dataByID['module_id']);
		}else{
			document_set_redirect('poll/'.$this->id);
		}
	}
	
	
	public function widget(){
		head_add_css('poll.css');
		$this->select(array('topic', 'permission'), "WHERE end_date > '".NOW."'");
		$render = '';
		foreach($this->data as $id => $data){
			if($this->isVotable($id)) $poll = $this->renderVoteForm($id);
			else $poll = $this->renderResult($id);
			$renderArray = array(
				'id' => $id,
				'topic' => $data['topic'],
				'poll' => $poll,
			);
			$render .= render($renderArray, 'poll_widget.tpl', 'poll_widget');
		}
		return $render;
	}
	
	public function embedIcon($description){
		if(authority_group('poll_admin')){
			head_add_css('poll.css');
			head_add_css('list.css');
			head_add_js('module/poll/poll.js');
			head_add_js('include/javascript/grid.column.js');
			$label = tt('Add or edit poll for this entry');
			$render  = render_ajax_icon('pollEmbedForm', 'module/poll/theme/poll.png', $label);
			$render .= '<div id="poll_embed_form" title="'.$label.'"></div>';
			$description = $render.$description;
		}
		return $description;
	}
	
	public function embedForm(){
		execute_set_ajax();
		$this->formMeta->isAjax = true;
		$this->formMeta->ajaxFunction = 'submitEmbedPollForm';
		if($this->checkEmbedExistence(gg('module'), (int)gg('module_id'))){
			$this->selectByID($this->column, true);
			return $this->edit();
		}else{
			return $this->write();
		}
	}
	
	public function embed($data){
		foreach($data as $key => $value){
			if(isset($data[$key]['content'])){
				$poll = $this->renderEmbed($value['module'], $key);
				$data[$key]['content'] = $poll.$value['content'];
			}else{
				break;
			}
		}
		return $data;
	}
	
	private function renderEmbed($module, $moduleID){
		if($this->checkEmbedExistence($module, $moduleID)){
			head_add_css('poll.css');
			$this->selectByID($this->column);
			$render = array('label' => $this->dataByID['topic']);
			if($this->isVotable($this->id)){
				$render['page'] = $this->renderVoteForm($this->id);
			}else{
				$render['page'] = $this->renderResult($this->id);
			}
			return render($render, 'poll_embed.tpl', 'poll_embed');
		}
	}
	
	private function isVotable($id){
		return ($this->checkPermission($id) and $this->isVoted($id));
	}
	
	private function renderVotePage(){
		$poll = $this->renderVoteForm($this->id);
		return $this->renderPage($poll);
	}
	
	private function renderResultPage(){
		$poll = $this->renderResult($this->id);
		return $this->renderPage($poll);
	}
	
	private function renderPage($poll){
		head_add_css('poll.css');
		document_set_path($this->dataByID['topic']);
		$this->dataByID['poll'] = $poll;
		$this->dataByID['editable'] = authority_group('poll_admin');
		$this->initComment();
		$render  = render($this->dataByID, 'poll.tpl', 'poll');
		$render .= $this->comment->render();
		return $render;
	}
	
	private function renderVoteForm($id){
		require_once(INCLUDE_PATH.'class/QuickFormRender.class.inc');
		require_once(INCLUDE_PATH.'class/FormMetaData.class.inc');
		
		$formMeta = new FormMetaData();
		$formMeta->id = 'poll_vote';
		$formMeta->mode = 'poll_vote/'.$id;
		
		$choises = db_select_single_column('poll_choise', 'choise', 'WHERE poll='.$id);
		
		$quickForm = new QuickFormRender('poll_vote');
		$quickForm->append('choise', Model::radio('', $choises));
		return $quickForm->renderEntireForm($formMeta);
	}
	
	private function renderResult($id){
		$clause = 'WHERE poll='.$id;
		$column = array('choise', 'voted_number');
		$choises = db_select('poll_choise', $column, $clause);
		$sum = 0;
		foreach($choises as $key => $value){
			$sum += $value['voted_number'];
		}
		foreach($choises as $key => $value){
			$choises[$key]['percent'] = floor(100*$value['voted_number']/$sum);
		}
		$renderArray = array(
			'choises' => $choises,
			'sum' => $sum,
		);
		return render($renderArray, 'poll_result.tpl', 'poll_result');
	}
	
	private function checkPermission($id){
		$permission = $this->data[$id]['permission'];
		if(is_array($permission)) $permission = $permission['value'];
		if($permission == POLL_PERMISSION_ALL){
			return true;
		}elseif($permission == POLL_PERMISSION_AUTHENTICATED){
			return !authority_is_guest();
		}else{
			return authority_group('poll_voter');
		}
	}
	
	private function isVoted($id){
		$clause = 'WHERE poll='.$id;
		if(authority_is_guest()){
			$ip = poll_get_ip();
			$clause .= " AND voter=-1 AND voter_ip='$ip'";
		}else{
			$clause .= ' AND voter='.USER_ID;
		}
		$vote = db_select_single_column('poll_vote', 'id', $clause);
		return (count($vote) == 0);
	}
	
	private function increaseChoise($choise){
		$clause = 'WHERE id='.$choise;
		$count = db_select_single_column('poll_choise', 'voted_number', $clause);
		$data = array('voted_number' => $count[$choise] + 1);
		db_update('poll_choise', $data, $choise);
	}
	
	private function checkVote($id){
		$data = array(
			'voter' => USER_ID,
			'voter_ip' => poll_get_ip(),
			'poll' => $id,
		);
		db_insert('poll_vote', $data);
	}
	
	private function initComment(){
		include_once(MODULE_PATH.'comment/Comment.class.inc');
		$this->comment = new Comment(false);
		$this->comment->setMode($this->fullModuleName);
		$this->comment->setParent($this->id);
		$this->comment->setPermission($this->dataByID['comment_permission']['value']);
		$this->comment->setParentWriter($this->dataByID['writer']['id']);
		$this->comment->init();
	}
	
	private function setConfigForm(){
		$permission = $this->getNewConfigForm(tt('permission'));
		$permission->append('end_date');
		$permission->append('permission');
		$permission->append('comment_permission');
	}
	
	private function checkEmbedExistence($module, $moduleID){
		$clause = "WHERE module='$module' AND module_id=$moduleID";
		$this->select(array('id'), $clause);
		if(count($this->data)){
			$this->id = array_pop(array_keys($this->data));
			return true;
		}else{
			return false;
		}
	}
	
	private function setAccess(){
		$this->setAdmin('poll_admin');
		
		$this->addAccessMethod('guest', 'index');
		$this->addAccessMethod('guest', 'vote');
		$this->addAccessMethod('guest', 'embed');
		$this->addAccessMethod('guest', 'widget');
		$this->addAccessMethod('guest', 'embedWriteIcon');
		
		$this->addAction('poll_vote', 'vote');
		$this->addAction('poll_embed', 'embed');
		$this->addAction('poll_embed_icon', 'embedIcon');
		$this->addAction('poll_embed_form', 'embedForm');
		$this->addAction('poll_widget', 'widget');
	}
}
?>
