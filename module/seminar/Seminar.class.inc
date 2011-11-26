<?php
execute_add_class('seminar', 'Seminar');
include('SeminarModel.class.inc');

class Seminar extends Control{
	function Seminar(){
		$this->moduleTitle = tt('Seminar');
		$this->moduleName = 'seminar';
		$this->tableName = 'seminar';
		$this->setAccess();
		$this->setAction();
		$this->setAdmin('seminar_admin');
		$this->init(new SeminarModel());
		document_set_path($this->moduleTitle, $this->moduleName);
		$this->writeLabel = tt('Submit new seminar');
	}
	
	public function seminarMapGetData(){
		document_set_path(tt('Seminar Attendants'));
		$this->id = gg('id');
		$this->selectByID($this->column);
		$seminarName = $this->dataByID;
		
		head_add_css('list.css');
		$column = array('seminar', 'attendant');
		$clause = 'WHERE seminar='.gg('id');
		$map = db_select('seminar_map', $column, $clause);
		$renderArray = array(
			'seminar' => $this->dataByID,
			'attendant' => userinfo_get_info($map, 'attendant'),
		);
		
		if(authority_group('seminar_moderator') or authority_group('seminar_admin') or authority_group('seminar_organizer'))
			$render = render($renderArray, 'seminarAttendantList.tpl', 'seminardisplay');
		else
			notify_add_info(tt('You have no permission, '));
		return $render;
	}
	
	public function index(){
		if(authority_group('seminar_moderator') or authority_group('seminar_admin')){
			document_set_path(tt('Seminar Summarize'));
			head_add_css('list.css');
			$page = render($renderArray, 'seminarreport.tpl', 'seminar_seminarreport');
		}else{
			$page = render($renderArray, 'seminarreport.tpl', 'seminar_seminarreport');
		}
		return $page;
	}
	
	public function listPage(){
		document_set_path(tt('List of Seminar'));
		$this->listMeta->title = sprintf(tt('List of %s'), $this->moduleTitle);
		$this->listMeta->defaultOrder = 'createdate';
		if(mid()) $mode = mm().'/'.mid();
		else $mode = mm();
		$this->listMeta->mode = $mode;
		$this->setListWriteIcon(tt('Add new Seminar'));
		$this->setListEditIcon();
		$this->setListDropIcon();
		$this->selectListPage(20, 'WHERE (section='.SECTION.')');
		return $this->renderListPage();
	}
	
	public function listMyPage(){
		document_set_path(tt('List of My Seminar'));
		$this->listMeta->title = sprintf(tt('List of My %s'), $this->moduleTitle);
		$this->listMeta->defaultOrder = 'createdate';
		if(mid()) $mode = mm().'/'.mid();
		else $mode = mm();
		$this->listMeta->mode = $mode;
		$this->setListWriteIcon(tt('Add new Seminar'));
		$this->modelConnector->setAttribute('size', 'isList', false);
		$this->modelConnector->setAttribute('classdate', 'isList', false);
		if(authority_group('seminar_moderator') or authority_group('seminar_admin') or authority_group('seminar_organizer')){
			$this->selectListPage(20, 'WHERE (owner='.USER_ID.')');
		}else{
			$this->selectListPage(20, 'WHERE (id='.USER_ID.')');
		}
		return $this->renderListPage();
	}
	public function display(){
		document_set_path(tt('Display'));
		$enrollFlag = FALSE;

		$column = array('seminar', 'attendant');
		$clause = 'WHERE attendant='.USER_ID.' AND seminar='.gg('id');
		$map = db_select('seminar_map', $column, $clause);

		if(count($map) == 0)
			$enrollFlag = TRUE;
		else
			$enrollFlag = FALSE;

		$this->id = MODE_ID;
		$this->selectByID($this->column);
		head_add_css('list.css');
		$renderArray = array(
			'data' => $this->data[$this->id],
			'enrollFlag' => $enrollFlag,
		);
		if(authority_group('seminar_moderator') or authority_group('seminar_admin') or authority_group('seminar_organizer'))
			$render = render($renderArray, 'seminardisplay.tpl', 'seminardisplay');
		else
			$render = render($renderArray, 'seminardisplay_attendant.tpl', 'seminardisplay');
		return $render;
	}
	
	public function displayAll(){
		document_set_path(tt('Display Seminars'));
		$enrollFlag = FALSE;
		$statusFlag = FALSE;
		$controlFlag = FALSE;
		$attendantFlag = FALSE;
		
		if(authority_group('seminar_attendant')){
			$attendantFlag = TRUE;
			echo('_attendant_');
			$column = array('seminar', 'attendant');
			$clause = 'WHERE attendant='.USER_ID.' AND seminar='.gg('id');
			$map = db_select('seminar_map', $column, $clause);
		
			if(count($map) == 0)
				$enrollFlag = TRUE;
			else
				$enrollFlag = FALSE;
		}
		elseif(authority_group('seminar_organizer')){
			$statusFlag = TRUE;
			echo('_organizer_');
			/*select the list for organizer*/
		}
		else {
			echo('_mod_and_admin_');
			$statusFlag = TRUE;
			$controlFlag = TRUE;
			/*select the list for organizer*/
		}
		$this->id = MODE_ID;
//		$this->selectByID($this->column);
		echo('test2');
		head_add_css('list.css');
		if(mid()) $mode = mm().'/'.mid();
		else $mode = mm();
		$this->listMeta->mode = $mode;
		$this->setListWriteIcon(tt('Add new Seminar'));
		$selcolumn = array('owner', 'classtime', 'duration', 'description');
		$this->selectListPage(20, 'WHERE (section='.SECTION.')',$selcolumn);
		$renderArray = array(
//			'data' => $this->data[$this->id],
			'data' => $this->data,
			'enrollFlag' => $enrollFlag,
			'attendantFlag' => $attendantFlag,
			'$statusFlag' => $statusFlag,
			'$controlFlag' => $controlFlag,
		);
		
		$render = render($renderArray, 'seminardisplayall.tpl', 'seminardisplay');
		return $render;
	}
	
	public function insert(){
		Control::insert();
		document_set_redirect('seminar_display/'.$this->id);
	}
	
	public function insertMap(){
		if($this->checkFull()){
			$data = array(
				'seminar' => gg('id'),
				'attendant' => USER_ID 
			);
			db_insert('seminar_map', $data);
			execute_set_db(); //after insert coz no display.
			document_set_redirect('seminar_mylist');
			notify_add_info(tt('You have been added to the seminar.'));
		}
	}
	
	private function setAccess(){
		$access = cc('seminar_access');
		$this->addAccessMethod('guest', 'display');
		$this->addAccessMethod('guest', 'listPage');
		
		$this->addAccessMethod('seminar_admin', 'insertMap');

		$this->addAccessMethod('seminar_attendant', 'insertMap');

		$this->addAccessMethod('seminar_organizer', 'insert');
		$this->addAccessMethod('seminar_organizer', 'write');
		$this->addAccessMethod('seminar_organizer', 'seminarMapGetData');
		
		$this->addAccessMethod('seminar_moderator', 'insert');
		$this->addAccessMethod('seminar_moderator', 'write');
		$this->addAccessMethod('seminar_moderator', 'seminarMapGetData');
		
		$this->addAccessMethod('seminar_admin', 'seminarMapGetData');
	}
	
	private function setAction(){
		$this->addAction('seminar_list', 'listPage');
//		$this->addAction('seminar_mylist', 'listMyPage');
		$this->addAction('seminar_displayAll', 'displayAll');
		$this->addAction('seminar_display', 'display');
		$this->addAction('seminar_insert_map', 'insertMap');
		$this->addAction('seminar_attendantlist', 'seminarMapGetData');
	}
	
	private function checkFull(){
		return true;
	}
	
}
?>
