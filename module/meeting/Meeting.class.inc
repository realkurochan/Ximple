<?php
execute_add_class('meeting', 'Meeting');
include_once(MODULE_PATH.'content/Content.class.inc');
include_once('MeetingModel.class.inc');

define('XIMPLE_MEETING_DECLINE', 0);
define('XIMPLE_MEETING_PARTICIPATE', 1);
define('XIMPLE_MEETING_NO_RESPONSE', 2);

class Meeting extends Content{
	function Meeting(){
		$this->tableName = 'meeting';
		$this->moduleName = 'meeting';
		$this->moduleTitle = tt('Meeting');
		$this->setAccess();
		$this->init(new MeetingModel());
		$participant = $this->getNewConfigForm(tt('participant groups'));
		$participant->append('participant_group');
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		$this->checkPermission();
		if($this->isSingleEntry()) return $this->renderSingleMeeting();
		elseif(gg('tag')) return $this->renderTagPage();
		else return $this->renderCalendar();
	}
	
	public function edit(){
		if($this->isSummarizable($this->dataByID)){
			$this->modelConnector->setAttribute('summary', 'isForm', true);
		}
		return Content::edit();
	}
	
	public function insert(){
		Content::insert();
		$this->sendInvitation();
	}
	
	public function printPage(){
		$this->checkPermission();
		$this->id = MODE_ID;
		$this->selectByID($this->column);
		execute_set_print();
		document_set_title($this->dataByID['topic']);
		$this->dataByID['content'] = $this->dataByID['content'].$this->dataByID['summary'];
		return render($this->dataByID, 'content_print.tpl', 'meeting_print');
	}
	
	public function participate($uid=USRE_ID, $checkParticipant=true){
		$this->addParicipantStatus(XIMPLE_MEETING_PARTICIPATE, $uid, $checkParticipant);
		$pattern = tt('Your name has been successfully inserted into the list of participants in %s.');
		$message = sprintf($pattern, $this->dataByID['topic']);
		notify_add_info($message);
	}
	
	public function decline($uid=USRE_ID, $checkParticipant=true){
		$this->addParicipantStatus(XIMPLE_MEETING_DECLINE, $uid, $checkParticipant);
		$pattern = tt('Your name has been successfully inserted into the list of disclaimers in %s.');
		$message = sprintf($pattern, $this->dataByID['topic']);
		notify_add_info($message);
	}
	
	public function participatePerMail(){
		$uid = $this->checkSecureKey(XIMPLE_MEETING_PARTICIPATE);
		if($uid) $this->participate($uid, false);
		else error_denied();
		if(authority_is_guest()) document_set_redirect('');
	}
	
	public function declinePerMail(){
		$uid = $this->checkSecureKey(XIMPLE_MEETING_DECLINE);
		if($uid) $this->decline($uid, false);
		else error_denied();
		if(authority_is_guest()) document_set_redirect('');
	}
	
	protected function isReadable(){
		if(authority_group('meeting_admin')) return true;
		if(authority_group('meeting_editor')) return true;
		if(authority_group('meeting_contributor')) return true;
		$group = $this->dataByID['participant_group'];
		if(count(array_intersect(ss('login_group'), $group))) return true;
	}
	
	private function renderTagPage(){
		head_add_css('meeting.css');
		$column = $this->modelConnector->getColumnNameExcepted(array('summary'));
		$this->selectFirstPage($column, cc('meeting_entries_number'));
		$this->getCommentNumber();
		$this->getDataEditable();
		$this->setPathMutiplePage();
		$this->translateData();
		$renderArray = array(
			'data' => $this->data,
			'show_category' => in_array('category_icon', cc('meeting_appearance')),
		);
		$render = $this->renderWriteIcon();
		$render.= render($renderArray, 'meeting_multiple.tpl', 'meeting_multiple');
		$render.= $this->pager->render('meeting'.$this->getPageMode());
		return $render;
	}
	
	private function renderSingleMeeting(){
		$this->id = mid();
		$this->selectByID($this->column);
		if(!$this->isReadable()) error_denied();
		$this->publishDate = date('Y-m-d H:i:s', strtotime($this->dataByID['publish_time']));
		if($this->isReadable()) return $this->executeRender();
		else $this->accessDeniedEntry();
	}
	
	private function renderCalendar(){
		head_add_css('meeting.css');
		document_set_path(tt('Meeting Calendar'));
		list($month, $year) = input_get_month();
		$this->getMeetingInMonth($month, $year);
		$calendarData = $this->getCalendarData($month);
		$calendar = render_calendar($calendarData, 'meeting', $month, $year);
		$renderArray = array(
			'calendar' => $calendar,
			'month' => date('F Y', strtotime("$year-$month-01")),
			'write_icon' => $this->renderWriteIcon(),
			'data' => $this->data,
		);
		return render($renderArray, 'meeting_calendar.tpl', 'meeting_calendar');
	}
	
	private function getMeetingInMonth($month, $year){
		if(strlen($month) == 1) $month = '0'.$month;
		$monthBegin = "'$year-$month-01 00:00:00'";
		$monthEnd = "'$year-$month-".date('t', strtotime($monthBegin))." 23:59:59'";
		$clause  = "WHERE (meeting_date >= $monthBegin AND meeting_date <= $monthEnd) ";
		$clause .= "AND publishing_status < 2 ORDER BY meeting_date";
		$this->select(array('id', 'topic', 'meeting_date', 'meeting_period'), $clause);
	}
	
	private function getCalendarData($month){
		$this->getDayOfMonth($month);
		$calendarArray = array();
		for($i=1; $i<=31; $i++){
			foreach($this->data as $key => $value){
				if($i == $value['day']){
					$calendarArray[$i]['class'] = 'active_meeting';
					$calendarArray[$i]['link'][] = 'meeting/'.$key;
				}
			}
		}
		return $calendarArray;
	}
	
	private function getDayOfMonth($month){
		foreach($this->data as $key => $value){
			$this->data[$key] = $this->processMeetingTime($value);
		}
	}
	
	private function executeRender(){
		head_add_css('meeting.css');
		$this->prepareRender();
		$render = $this->renderWriteIcon();
		$this->dataByID['participate_icon'] = $this->renderParticipateIcon();
		$this->dataByID['decline_icon'] = $this->renderDeclineIcon();
		$this->dataByID['participant'] = $this->getParticipant();
		$this->dataByID['disclaimer'] = $this->getDisclaimer();
		$this->dataByID['is_summarizable'] = $this->isSummarizable($this->dataByID);
		$render.= render($this->dataByID, 'meeting.tpl', 'meeting');
		$render.= $this->comment->render();
		$this->setDocumentData();
		return $render;
	}
	
	private function prepareRender(){
		$this->translateDataByID();
		$this->handlePublishingStatus();
		$this->initComment();
		$this->insertTracker('read');
		$this->addMetaData();
		$this->getAppearanceConfiguration();
		$this->dataByID['editable'] = $this->isEditable($this->dataByID['writer']['id']);
		$this->dataByID = $this->processMeetingTime($this->dataByID);
		$operation = $this->getExtendedOperation();
		$this->dataByID['addition_operation'] = $operation[$this->id];
	}
	
	private function processMeetingTime($data){
		$meetingTime = strtotime($data['meeting_date']);
		$data['meeting_date'] = date(cc('time_format'), $meetingTime);
		$data['meeting_time'] = date('H:i', $meetingTime);
		$data['end_time'] = date('H:i', $meetingTime + $data['meeting_period']);
		$data['meeting_period'] = date('H:i', $data['meeting_period']);
		$data['day'] = (int) date('j', $meetingTime);
		return $data;
	}
	
	private function getAppearanceConfiguration(){
		$appearance = cc('meeting_appearance');
		if(!is_array($appearance)) $appearance = array();
		$this->dataByID['show_avatar'] = in_array('avatar', $appearance);
		$this->dataByID['show_category'] = in_array('category_icon', $appearance);
	}
	
	private function renderParticipateIcon(){
		if($this->isParticipant($this->dataByID)){
			$icon = 'module/meeting/theme/participate.png';
			return render_icon('meeting_participate/'.$this->id, $icon, tt('join Meeting'));
		}
	}
	
	private function renderDeclineIcon(){
		if($this->isParticipant($this->dataByID)){
			$icon = 'files/icon/drop.png';
			return render_icon('meeting_decline/'.$this->id, $icon, tt('decline Meeting'));
		}
	}
	
	private function isParticipant($data, $uid=USER_ID){
		$clause = 'WHERE meeting='.$data['id'].' AND participant='.$uid;
		$isParticipated = count(db_select_single_column('meeting_participant', 'id', $clause));
		if($isParticipated == 0){
			$group = $data['participant_group'];
			$isInGroup = count(array_intersect(ss('login_group'), $group));
			if($isInGroup) return true;
		}
		return false;
	}
	
	private function getParticipant(){
		return $this->getRelatedUser(XIMPLE_MEETING_PARTICIPATE);
	}
	
	private function getDisclaimer(){
		return $this->getRelatedUser(XIMPLE_MEETING_DECLINE);
	}
	
	private function getRelatedUser($status){
		$clause = 'WHERE meeting='.$this->id.' AND status='.$status;
		$participant = db_select('meeting_participant', array('participant'), $clause);
		return userinfo_get_info($participant, 'participant');
	}
	
	private function isSummarizable($data){
		return (strtotime($data['meeting_date']) < time());
	}
	
	private function addParicipantStatus($status, $uid, $checkParticipant=true){
		$this->id = mid();
		$this->selectByID(array('id', 'participant_group', 'topic'));
		if(!$checkParticipant or $this->isParticipant($this->dataByID, $uid)){
			$data = array(
				'meeting' => $this->id,
				'status' => $status,
				'participant' => $uid,
			);
			db_insert('meeting_participant', $data);
		}
		execute_set_db();
		document_set_redirect('meeting/'.$this->id);
	}
	
	private function checkSecureKey($status){
		$clause = 'WHERE status='.XIMPLE_MEETING_NO_RESPONSE.' ';
		$clause.= "AND secure_key='".gg('secure_key')."' ";
		$clause.= 'AND participant='.gg('participant').' ';
		$clause.= 'AND meeting='.((int) gg('id'));
		$uid = db_select_single_column('meeting_key', 'participant', $clause);
		if(count($uid)){
			$id = array_pop(array_keys($uid));
			db_update('meeting_key', array('status' => $status), $id);
			return array_pop($uid);
		}
		return false;
	}
	
	private function sendInvitation(){
		if($this->dataByID['meeting_date'] > NOW){
			require_once(INCLUDE_PATH.'tool/mail.tool.inc');
			$uidArray = userinfo_group_to_id($this->dataByID['participant_group']);
			$secureKey = $this->generateSecureKey($uidArray);
			$messageArray = $this->getInvitationMessage($secureKey);
			$subject = tt('Meeting Invitation');
			$email = userinfo_get_email($uidArray);
			$invitation = '';
			foreach($messageArray as $uid => $message){
				mail_send($email[$uid], $subject, $message, true);
			}
		}
	}
	
	private function generateSecureKey($uidArray){
		$clause = 'WHERE meeting='.$this->id;
		$existence = db_select_single_column('meeting_key', 'participant', $clause, false);
		$uidArray = array_diff($uidArray, $existence);
		$secureKey = array();
		foreach($uidArray as $key => $uid){
			$key = md5(rand().time()).sha1(rand().rand());
			$secureKey[$uid] = $key;
			$data = array(
				'secure_key' => $key,
				'meeting' => $this->id,
				'participant' => $uid,
				'status' => XIMPLE_MEETING_NO_RESPONSE,
			);
			db_insert('meeting_key', $data);
		}
		return $secureKey;
	}
	
	private function getInvitationMessage($secureKey){
		$message = array();
		$topic = $this->dataByID['topic'];
		$params  = $this->id.'/participant/%d/secure_key/%s/';
		$pattern = cc('meeting_invitation_message');
		$pattern.= '<p>';
		$pattern.= render_operation_link('meeting_participate_per_mail', $params, tt('join Meeting : ').$topic);
		$pattern.= '</p><p>';
		$pattern.= render_operation_link('meeting_decline_per_mail', $params, tt('decline Meeting : ').$topic);
		$pattern.= '</p>';
		foreach($secureKey as $uid => $key){
			$message[$uid] = sprintf($pattern, $uid, $key, $uid, $key);
		}
		return $message;
	}
	
	private function setAccess(){
		$this->addAction('meeting_participate', 'participate');
		$this->addAction('meeting_decline', 'decline');
		$this->addAction('meeting_participate_per_mail', 'participatePerMail');
		$this->addAction('meeting_decline_per_mail', 'declinePerMail');
		
		$this->addAccessMethod('user', 'participate');
		$this->addAccessMethod('user', 'decline');
		
		$this->addAccessMethod('guest', 'participatePerMail');
		$this->addAccessMethod('guest', 'declinePerMail');
	}
	
	private function checkPermission(){
		if(!authority(array(), cc('meeting_permission'))) error_denied();
	}
}
?>
