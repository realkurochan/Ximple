<?php
execute_add_class('develcard', 'DevelCard');
include('DevelCardModel.class.inc');
include('develcard.tool.inc');

class DevelCard extends Control{
	private $comment;
	private $pointYear;
	
	private $type = null;
	private $status = null;
	
	function DevelCard(){
		$this->moduleTitle = tt('DevelCard');
		$this->moduleName = 'develcard';
		$this->tableName = 'develcard';
		$this->setAccess();
		$this->setAction();
		$this->setAdmin('develcard_admin');
		$this->redirectToIDPage = true;
		$this->init(new DevelCardModel());
		$this->setConfig();
		$this->writeLabel = tt('Submit new DevelCard');
		$this->formMeta->isCaptcha = true;
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		$this->id = (int) gg('id');
		if($this->id) return $this->display();
		else return $this->summaryPage();
	}
	
	public function insert(){
		Control::insert();
		$message = $this->getSubscribeMessage();
		$this->subscribeResolver($message);
		notify_subscribe_by_mode($message, 'develcard');
		$this->selectByID($this->column);
		$this->insertTracker('write');
	}
	
	public function update(){
		$message = $this->getSubscribeMessage();
		$this->subscribeResolver($message);
		Control::update();
		$this->insertTracker('update');
		$this->setResolverPoint((int)pp('status'));
	}
	
	public function personalPage(){
		head_add_css('develcard.css');
		$uid = $this->getUserID();
		$user = userinfo_get_info_by_id($uid);
		document_set_path($user['login_name'], 'develcard_personal/'.$uid);
		$rednerArray = array(
			'user' => $user,
			'write_icon' => $this->renderWriteIcon(),
			'calendar' => $this->renderCalendar($uid),
			'point' => $this->renderPointGraph($uid),
		);
		return render($rednerArray, 'develcard_personal.tpl', 'develcard_personal');
	}
	
	public function personalListPage(){
		document_set_path(tt('Personals List'));
		$renderArray = array(
			'resolver' => userinfo_get_login_name_by_role('develcard_resolver'),
			'reporter' => userinfo_get_login_name_by_role('develcard_reporter'),
		);
		return render($renderArray, 'develcard_personal_list.tpl', 'develcard_personal_list');
	}
	
	public function reporterListPage(){
		$uid = $this->getUserID();
		return $this->renderReporterListPage($uid);
	}
	
	public function resolverListPage(){
		$uid = $this->getUserID();
		return $this->renderResolverListPage($uid);
	}
	
	public function dropResolver(){
		db_drop('develcard_resolver', (int)gg('id'));
	}
	
	public function setStatus(){
		$this->id = (int) gg('id');
		$status = (int) gg('status');
		
		$data = array(
			'status' => $status,
			'end_date' => NOW
		);
		db_update($this->tableName, $data, $this->id);
		$this->setResolverPoint($status);
		
		execute_set_db();
		document_set_redirect('develcard/'.$this->id);
		
		$statusOption = develcard_get_status_option();
		$statusName = $statusOption[$status];
		notify_add_info(tt('Status of DevelCard has been successfully changed to : ').$statusName);
	}
	
	public function renderTagPage(){
		$tag = gg('tag');
		$mode = 'develcard_tag/tag/'.$tag;
		$clause = $this->getStatusTypeClause($mode);
		$title = tt('DevelCard tagged by : ').$tag;
		document_set_path($tag, $mode);
		$clause = "WHERE freetag LIKE '%$tag%' $clause ORDER BY comment_time DESC";
		return $this->renderListPage($title, $clause, $mode);
	}
	
	public function renderWeekList(){
		$week = (int) gg('week');
		$year = (int) gg('year');
		$resolver = (int) gg('resolver');
		$mode = "develcard_week_list/resolver/$resolver/week/$week/year/$year";
		
		$resolverInfo = userinfo_get_info_by_id($resolver);
		$title  = tt('DevelCard resolved by : ').$resolverInfo['login_name'].' '.$year.'W'.$week;

		document_set_path($resolverInfo['login_name'], 'develcard_personal/'.$resolver);
		document_set_path($year.'W'.$week);
				
		$weekClause = $this->getWeekListClause($resolver, $week, $year);
		$clause = $this->getStatusTypeClause($mode);
		$clause = "WHERE ($weekClause) $clause AND status=2 ORDER BY comment_time DESC";
		return $this->renderListPage($title, $clause, $mode);
	}
	
	private function display(){
		$this->selectByID($this->column);
		$this->dataByID = $this->prepareData($this->dataByID, 'develcard');
		head_add_css('develcard.css');
		document_set_path($this->dataByID['status']['label'], $this->dataByID['status_uri']);
		document_set_path($this->dataByID['type']['label'], $this->dataByID['type_uri']);
		document_set_path($this->dataByID['topic']);
		
		$this->dataByID['editable'] = (develcard_is_admin() or authority_group('develcard_resolver'));
		$this->initComment();
		$this->insertTracker('read');
		
		$operation = $this->getExtendedOperation();
		$this->dataByID['addition_operation'] = $operation[$this->id];
		$render = render($this->dataByID, 'develcard_display.tpl', 'develcard_display');
		$render.= $this->comment->render();
		return $render;
	}
	
	
	private function renderResolverListPage($uid){
		$user = userinfo_get_info_by_id($uid);
		$mode = 'develcard_resolver_list/'.$uid;
		document_set_path($user['login_name'], 'develcard_personal/'.$uid);
		document_set_path(tt('in duty'), $mode);
		
		$clause = 'WHERE resolver='.$uid;
		$id = db_select_single_column('develcard_resolver', 'develcard', $clause);
		if(count($id)) $idClause = 'id IN ('.join(',', $id).')';
		else $idClause = 'id=0';
		
		$clause = $this->getStatusTypeClause($mode);
		$title = tt('DevelCard in Duty of : '.$user['login_name']);
		$clause = 'WHERE '.$idClause.$clause.' ORDER BY comment_time DESC';
		return $this->renderListPage($title, $clause, $mode);
	}
	
	private function renderReporterListPage($uid){
		$user = userinfo_get_info_by_id($uid);
		$mode = 'develcard_reporter_list/'.$uid;
		document_set_path($user['login_name'], 'develcard_personal/'.$uid);
		$clause = $this->getStatusTypeClause($mode);
		$title = tt('DevelCard reported by : ').$user['login_name'];
		$clause = 'WHERE writer='.$uid.$clause.' ORDER BY comment_time DESC';
		return $this->renderListPage($title, $clause, $mode);
	}
	
	private function summaryPage(){
		$mode   = 'develcard';
		$clause = $this->getStatusTypeClause($mode);
		$title  = tt('DevelCard');
		$clause = 'WHERE id!=0 '.$clause.' ORDER BY comment_time DESC';
		return $this->renderListPage($title, $clause, $mode);
	}
	
	private function renderCalendar($uid){
		list($month, $year) = input_get_month();
		$this->getCalendarData($month, $year);
		$this->filterData($uid);
		$calendarData = $this->getCalendar($month);
		return render_calendar($calendarData, 'develcard_personal/'.gg('id'), $month, $year);
	}
	
	private function renderPointGraph($uid){
		$this->getPointYear();
		$pointData = $this->getYearPointData($uid);
		list($point, $sumLoad, $sumPoint) = $this->calculatePoint($pointData);
		$renderArray = array(
			'resolver' => $uid,
			'point' => $point,
			'pixel' => cc('develcard_pixel_per_point'),
			'first_year' => $this->getResolverFirstYear($uid),
			'last_year' => (int) date('Y'),
			'load_color' => cc('develcard_load_color'),
			'load_bg_color' => cc('develcard_load_bg_color'),
			'point_color' => cc('develcard_point_color'),
			'point_bg_color' => cc('develcard_point_bg_color'),
			'point_year' => $this->pointYear,
			'sum_load' => $sumLoad,
			'sum_point' => $sumPoint,
		);
		if(count($point)){
			$renderArray['first_week'] = min(array_keys($point));
			$renderArray['last_week'] = max(array_keys($point));
		}
		return render($renderArray, 'develcard_point.tpl', 'develcard_point');
	}
	
	protected function renderListPage($title, $clause, $mode){
		$mode = $this->getModeParameter($mode);
		head_add_css('develcard.css');
		document_set_title($title);
		$this->selectPage($this->column, cc('develcard_list_per_page'), $clause);
		foreach($this->data as $key => $value){
			$this->data[$key] = $this->prepareData($value, $mode);
		}
		$renderArray = array(
			'data' => $this->data,
			'title' => $title,
			'write_icon' => $this->renderWriteIcon(),
			'personal_icon' => render_icon('develcard_personal', 'module/develcard/theme/devel.png', tt('My DevelCard')),
		);
		$render = render($renderArray, 'develcard_list.tpl', 'develcard_list');
		$render.= $this->pager->render($mode);
		return $render;
	}
	
	private function setAction(){
		$this->addAction('develcard_reporter_list', 'reporterListPage');
		$this->addAction('develcard_resolver_list', 'resolverListPage');
		$this->addAction('develcard_drop_resolver', 'dropResolver');
		$this->addAction('develcard_personal', 'personalPage');
		$this->addAction('develcard_personal_list', 'personalListPage');
		$this->addAction('develcard_set_status', 'setStatus');
		$this->addAction('develcard_tag', 'renderTagPage');
		$this->addAction('develcard_week_list', 'renderWeekList');
	}
	
	private function setAccess(){
		$access = cc('develcard_access');
		if($access == 0){
			$this->addAccessMethod('guest', 'index');
			$this->addAccessMethod('guest', 'write');
			$this->addAccessMethod('guest', 'insert');
		}elseif($access == 1){
			$this->addAccessMethod('user', 'index');
			$this->addAccessMethod('user', 'write');
			$this->addAccessMethod('user', 'insert');
		}
		
		$this->addAccessMethod('develcard_reporter', 'index');
		$this->addAccessMethod('develcard_reporter', 'write');
		$this->addAccessMethod('develcard_reporter', 'insert');
		$this->addAccessMethod('develcard_reporter', 'reporterListPage');
		$this->addAccessMethod('develcard_reporter', 'personalPage');
		$this->addAccessMethod('develcard_reporter', 'renderTagPage');
		$this->addAccessMethod('develcard_reporter', 'renderWeekList');
		
		$this->addAccessMethod('develcard_resolver', 'index');
		$this->addAccessMethod('develcard_resolver', 'resolverListPage');
		$this->addAccessMethod('develcard_resolver', 'personalPage');
		$this->addAccessMethod('develcard_resolver', 'setStatus');
		$this->addAccessMethod('develcard_resolver', 'renderTagPage');
		$this->addAccessMethod('develcard_resolver', 'renderWeekList');
		
		$this->addAccessMethod('develcard_moderator', 'index');
		$this->addAccessMethod('develcard_moderator', 'write');
		$this->addAccessMethod('develcard_moderator', 'insert');
		$this->addAccessMethod('develcard_moderator', 'edit');
		$this->addAccessMethod('develcard_moderator', 'update');
		$this->addAccessMethod('develcard_moderator', 'drop');
		$this->addAccessMethod('develcard_moderator', 'listPage');
		$this->addAccessMethod('develcard_moderator', 'reporterListPage');
		$this->addAccessMethod('develcard_moderator', 'resolverListPage');
		$this->addAccessMethod('develcard_moderator', 'dropResolver');
		$this->addAccessMethod('develcard_moderator', 'personalPage');
		$this->addAccessMethod('develcard_moderator', 'personalListPage');
		$this->addAccessMethod('develcard_moderator', 'setStatus');
		$this->addAccessMethod('develcard_moderator', 'renderTagPage');
		$this->addAccessMethod('develcard_moderator', 'renderWeekList');
	}
	
	private function prepareData($data, $mode){
		$data['type_uri'] = $this->getTypeURI($data, $mode);
		$data['status_uri'] = $this->getStatusURI($data, $mode);
		$data['priority_color'] = develcard_get_priority_color($data['priority']['value']);
		$data['residual_time'] = $this->getResidualTime($data);
		return $data;
	}
	
	private function getTypeURI($data, $mode){
		return $mode.'/status/'.$data['status']['value'].'/type/'.$data['type']['value'];
	}
	
	private function getStatusURI($data, $mode){
		return $mode.'/status/'.$data['status']['value'];
	}
	
	private function getResidualTime($data){
		$residual = ceil((strtotime($data['deadline']) - time())/(60*60*24));
		if($residual == 1) $residual = tt('tomorrow');
		elseif($residual == 0) $residual = tt('today');
		elseif($residual == -1) $residual = tt('yesterday');
		elseif($residual > 1) $residual = sprintf(tt('%d days left'), $residual);
		else $residual = sprintf(tt('%d days ago'), $residual);
		return $residual;
	}
	
	private function getStatusTypeClause($mode){
		$this->type = gg('type');
		$this->status = gg('status');
		$clause  = $this->handleStatus($mode);
		$clause .= $this->handleType($mode);
		$clause .= ' AND section='.SECTION;
		return $clause;
	}
	
	private function handleType($mode){
		if(strlen($this->type)){
			document_set_path(develcard_get_type_name($this->type), $mode);
			return ' AND type='.$this->type;
		}
	}
	
	private function handleStatus($mode){
		if(strlen($this->status)){
			document_set_path(develcard_get_status_name($this->status), $mode);
			return ' AND status='.$this->status;
		}
	}
	
	private function setConfig(){
		$config = $this->getNewConfigForm(tt('Work Flow & Configuration'));
		$config->append('priority');
		if(develcard_is_admin()){
			$config->append('status');
			$config->append('resolver');
		}
		$config->append('deadline');
		$config->append('comment_permission');
	}
	
	private function getUserID(){
		$uid = (int) gg('id');
		if(empty($uid)) $uid = USER_ID;
		return $uid;
	}
	
	private function setResolverPoint($status){
		$clause = 'WHERE develcard='.$this->id;
		$resolver = db_select_single_column('develcard_resolver', 'work_load', $clause);
		if($status >= 2) $this->setResolverEndPoint($status, $resolver);
		else $this->setResolverProgressPoint($status, 'develcard='.$this->id);
	}
	
	private function setResolverEndPoint($status, $resolver){
		$this->selectByID(array('deadline'));
		$deadline = strtotime($this->dataByID['deadline']);
		foreach($resolver as $key => $value){
			$point = $this->caculateResolverPoint($value, $deadline);
			$data = array(
				'status' => $status,
				'point' => $point,
				'end_date' => NOW,
			);
			db_update('develcard_resolver', $data, $key);
		}
	}
	
	private function setResolverProgressPoint($status, $clause){
		$data = array('status' => $status);
		db_update_many('develcard_resolver', $data, $clause);
	}
	
	private function caculateResolverPoint($load, $deadline){
		$difference = time() - $deadline;
		if(abs($difference) < 86400) return $load*2;
		elseif($difference > 0)	return max($load, $load*2 - floor($difference/86400));
		else return $load*2 - floor($difference/86400);
	}
	
	private function initComment(){
		if(!$this->comment){
			include_once(MODULE_PATH.'comment/Comment.class.inc');

			if(!isset($this->dataByID['comment_permission'])) $permission = null;
			else $permission = $this->dataByID['comment_permission']['value'];

			$this->comment = new Comment(false);
			$this->comment->setMode($this->fullModuleName);
			$this->comment->setParent($this->id);
			$this->comment->setPermission($permission);
			$this->comment->setSubscribeMode('develcard_comment');
			$this->comment->setParentWriter($this->dataByID['writer']['id']);
			$this->comment->init();
		}
	}
	
	private function getCalendarData($month, $year){
		if(strlen($month) == 1) $month = '0'.$month;
		$monthBegin = "'$year-$month-01 00:00:00'";
		$monthEnd = "'$year-$month-".date('t', strtotime($monthBegin))." 23:59:59'";
		$clause  = "WHERE ((deadline >= $monthBegin AND deadline <= $monthEnd) OR ";
		$clause .= "(end_date >= $monthBegin AND end_date <= $monthEnd))";
		$clause .= "AND section=".SECTION;
		$this->select(array('id', 'topic', 'deadline', 'end_date', 'status'), $clause);
	}
	
	private function filterData($uid){
		if(count($this->data)){
			$id = array_keys($this->data);
			$clause = 'WHERE (develcard='.join(' OR develcard=', $id).') AND resolver='.$uid;
			$resolver = db_select_single_column('develcard_resolver', 'develcard', $clause);
			$data = $this->data;
			$this->data = array();
			foreach($resolver as $key => $value){
				$this->data[$value] = $data[$value];
			}
		}
	}
	
	private function getCalendar($month){
		$calendarArray = array();
		for($i=1; $i<=31; $i++){
			foreach($this->data as $key => $value){
				$deadine = $value['deadline'];
				$end = $value['end_date'];
				$status = $value['status']['value'];
				if($i ==  $this->getDate('j', $deadine) and $month == $this->getDate('m', $deadine)){
					$calendarArray[$i]['class'] = 'active_deadline';
					$calendarArray[$i]['link'][] = 'develcard/'.$key;
				}
			}
		}
		return $calendarArray;
	}
	
	private function getDate($format, $data){
		return (int) date($format, strtotime($data));
	}
	
	private function getPointYear(){
		$this->pointYear = (int) gg('point_year');
		if(empty($this->pointYear)) $this->pointYear = (int) date('Y');
	}
	
	private function getYearPointData($uid){
		$yearBegin = $this->pointYear.'-01-01 00:00:00';
		$yearEnd = $this->pointYear.'-12-31 23:59:59';
		$clause = "WHERE (end_date >= '$yearBegin' AND end_date <= '$yearEnd') ";
		$clause.= "AND resolver=$uid AND status=2";
		return  db_select('develcard_resolver', array('end_date', 'work_load', 'point'), $clause);
	}
	
	private function calculatePoint($pointData){
		$point = array();
		$sumLoad = 0;
		$sumPoint = 0;
		foreach($pointData as $key => $value){
			$week = (int) date('W', strtotime($value['end_date']));
			if(isset($point[$week])){
				$point[$week]['load']  += $value['work_load'];
				$point[$week]['point'] += $value['point'];
			}else{
				$point[$week]['load']  = $value['work_load'];
				$point[$week]['point'] = $value['point'];
			}
			$sumLoad += $value['work_load'];
			$sumPoint += $value['point'];
		}
		return array($point, $sumLoad, $sumPoint);
	}
	
	private function getResolverFirstYear($uid){
		$clause = "WHERE resolver=$uid ORDER BY end_date LIMIT 1";
		$data = db_select_single_column('develcard_resolver', 'end_date', $clause, false);
		if(count($data)) return (int) date('Y', strtotime($data[0]));
		else return (int) date('Y');
	}
	
	private function subscribeResolver($message){
		$resolver = pp('resolver');
		if(is_array($resolver)){
			$message .= ' '.tt('And you got some duty.');
			notify_multiple_subscribe($message, pp('resolver'), 'develcard');
		}
	}
	
	private function getSubscribeMessage(){
		if(authority_is_guest()) $userName = pp('reporter_name');
		else $userName = USER_NAME;
		$uri = SECTION_URI.Q.'develcard/'.$this->id;
		return sprintf(tt('%s posts <a href="%s">%s</a> in DevelCard.'), $userName, $uri, pp('topic'));
	}
	
	private function getModeParameter($mode){
		if($this->status) $mode = $mode.'/status/'.$this->status;
		if($this->type) $mode = $mode.'/type/'.$this->type;
		return $mode;
	}
	
	private function getWeekListClause($resolver, $week, $year){
		$weekDate = $year."W".sprintf('%2d', $week);
		$start = date('Y-m-d', strtotime($weekDate."1")).' 00:00:00';
		$end = date('Y-m-d', strtotime($weekDate."7")).' 23:59:59';
		$clause = "WHERE resolver=$resolver AND end_date>='$start' AND end_date<='$end'";
		$cardID = db_select_single_column('develcard_resolver', 'develcard', $clause);
		if(count($cardID)) return 'id='.join(' OR id=', $cardID);
	}
	
	private function insertTracker($action){
		include_once(MODULE_PATH.'tracker/tracker.tool.inc');
		$data = $this->dataByID;
		if(!is_array($this->dataByID['writer'])){
			$data['writer'] = $this->dataByID['writer'];
		}else{
			$data['writer'] = $this->dataByID['writer']['id'];
		}
		$data['writer_name'] = $this->dataByID['reporter_name'];
		$data['action'] = $action;
		$data['mode'] = $this->fullModuleName;
		tracker_insert($data);
	}
	
	private function getExtendedOperation(){
		$operation = array();
		foreach($this->data as $id => $data){
			$data['module'] = $this->fullModuleName;
			if(is_array($data['writer'])) $data['owner'] = $data['writer']['id'];
			else $data['owner'] = $data['writer'];
			$operation[$id] = extension_execute_append_result('content_operation', $data, array());
		}
		return $operation;
	}
}
?>
