<?php
execute_add_class('review', 'Review');
include('ReviewModel.class.inc');

class Review extends Control{
	function Review(){
		$this->moduleTitle = tt('Review');
		$this->moduleName = 'review';
		$this->tableName = 'review';
		$this->setAccess();
		$this->setAction();
		$this->setAdmin('review_admin');
		$this->init(new ReviewModel());
		// As callee module Reiview should not set path.
	}
	
	public function index(){
		if(authority_group('review_reviewer')) {
			return $this->reviewerList();
		}else{
			error_denied();
			// List page for reviewee is task for caller module.
			// Review operates as callee module.
			// return $this->revieweeList();
		}
	}
	
	/// List page for admin to manage.
	public function listPage(){
		document_set_path(tt('List of all Review'));
		$this->listMeta->title = sprintf(tt('List of %s'), $this->moduleTitle);
		$this->listMeta->defaultOrder = 'createdate';
		$this->listMeta->mode = $mode;
		$this->setListWriteIcon(tt('Add new Review'), 'review_write/'.gg('id'));
		$this->setListEditIcon();
		$this->setListDropIcon();
		// Show all in section.
		$this->selectListPage(20, 'WHERE (section='.SECTION.')');
		foreach($this->data as $key => $value){
			$this->data[$key]['reviewer'] = render_profile_link($value['reviewer']);
		}
		// Render link to caller module to see reviews.
		$this->renderCallerLink();
		return $this->renderListPage();
	}
	
	public function reviewRating($point){
		$this->id = gg('id');
		$this->selectByID($this->column);
		execute_set_db();
		$mode = $this->dataByID['reviewee_mode'].'/';
		$mode.= $this->dataByID['reviewee_id'];
		document_set_redirect($mode);
		
		if ($this->isRated()){
			notify_add_info(tt('This review already has been rated by this reviewer'));
		}else{
			$this->updateRating($point);
			$this->updateSummary($point);
		}
	}
	
	public function reviewerList(){
		$selectClause = 'WHERE section='.SECTION." AND reviewer='".gg('id')."'";
		$this->select(array('reviewee', 'star','createdate', 'description'), $selectClause);
		foreach($this->data as $key => $value){
			$this->data[$key]['reviewee'] = render_profile_link($value['reviewee']);
		}
		$renderArray = array(
			'review_data' => $this->data
		);
		$page = render($renderArray, 'reviewerdisplay.tpl', 'show_faq');
		return $page;
	}
	
	public function insert(){
		Control::insert();
		document_set_redirect('review_reviewee_list/'.pp('reviewee'));
	}
	
	public function write(){
		if(strlen(gg('id')) == 0 or gg('id') == 0){
			notify_add_error(tt('Please choose reviewee before review.'));
			execute_set_db();
			document_set_redirect('review_reviewee_list');
		}
		else{
			$userInfo = userinfo_get_info_by_id(gg('id'));
			$this->writeLabel = sprintf(tt('Submit new Review for %s'), $userInfo['login_name']);
			return Control::write();
		}
	}
	
	private function setAccess(){
		$this->addAccessMethod('guest', 'revieweeList');
		$this->addAccessMethod('guest', 'index');
		$this->addAccessMethod('review_reviewer', 'write');
		$this->addAccessMethod('review_reviewer', 'insert');
		$this->addAccessMethod('review_reviewer', 'reviewerList');
		$this->addAccessMethod('review_reviewer', 'reviewRating');
	}
	
	private function setAction(){
		$this->addAction('review_reviewer_list', 'reviewerList');
		$this->addAction('review_reviewee_list', 'revieweeList');
		$this->addAction('review_rating', 'reviewRating');
	}
	
	private function renderCallerLink(){
		
	}
	
	private function isRated(){
		$selectClause  = "WHERE review_id=".gg('id')." AND scorer=".USER_ID;
		$foundRating = db_get_rows_number('review_rating', $selectClause);
		return $foundRating != 0;
	}
	
	private function updateRating($point){
		$data = array(
			'review_id' => gg('review_id'),
			'scorer' => USER_ID,
			'point' => $point, 
		);
		db_insert('review_rating', $data);
	}
	
	private function updateSummary($point){
		$assignedPoint = (int)gg('point');
		$point1 = $this->data[$this->id]['point1'];
		$point2 = $this->data[$this->id]['point2'];
		if ($assignedPoint > 0){
			$point1 = $point1 + $assignedPoint;
		} else {
			$point2 = $point2 - $assignedPoint;
		}
		db_update('review',array('point1' => $point1, 'point2' => $point2),$this->id);
	}
}

/// Eliminate review_add_link(), which is task of caller.
?>
