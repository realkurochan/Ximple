<?php
execute_add_class('activity', 'Activity');
include_once(MODULE_PATH.'content/Content.class.inc');
include_once('ActivityModel.class.inc');

class Activity extends Content{
	function Activity(){
		$this->tableName = 'activity';
		$this->moduleName = 'activity';
		$this->moduleTitle = tt('Activity');
		$this->init(new ActivityModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		if($this->isSingleEntry()) return $this->renderSingleActivity();
		elseif(gg('tag')) return $this->renderTagPage();
		else return $this->renderCalendar();
	}
	
	private function renderTagPage(){
		head_add_css('activity.css');
		$this->selectFirstPage($this->column, cc('activity_entries_number'));
		$this->getCommentNumber();
		$this->getDataEditable();
		$this->setPathMutiplePage();
		$this->translateData();
		$renderArray = array(
			'data' => $this->data,
			'show_category' => in_array('category_icon', cc('activity_appearance')),
		);
		$render = $this->renderWriteIcon();
		$render.= render($renderArray, 'activity_multiple.tpl', 'activity_multiple');
		$render.= $this->pager->render('activity'.$this->getPageMode());
		return $render;
	}
	
	private function renderSingleActivity(){
		$this->id = mid();
		$this->selectByID($this->column);
		$this->publishDate = date('Y-m-d H:i:s', strtotime($this->dataByID['publish_time']));
		if($this->isReadable()) return $this->executeRender();
		else $this->accessDeniedEntry();
	}
	
	private function renderCalendar(){
		head_add_css('activity.css');
		document_set_path(tt('Activity Calendar'));
		list($month, $year) = input_get_month();
		$this->getActivity($month, $year);
		$calendarData = $this->getCalendarData($month);
		$calendar = render_calendar($calendarData, 'activity', $month, $year);
		$renderArray = array(
			'calendar' => $calendar,
			'month' => date('F Y', strtotime("$year-$month-01")),
			'write_icon' => $this->renderWriteIcon(),
			'data' => $this->data,
		);
		return render($renderArray, 'activity_calendar.tpl', 'activity_calendar');
	}
	
	private function getActivity($month, $year){
		if(strlen($month) == 1) $month = '0'.$month;
		$monthBegin = "'$year-$month-01 00:00:00'";
		$monthEnd = "'$year-$month-".date('t', strtotime($monthBegin))." 23:59:59'";
		$clause  = "WHERE ((start_date >= $monthBegin AND start_date <= $monthEnd) ";
		$clause .= "OR (end_date >= $monthBegin AND end_date <= $monthEnd)) ";
		$clause .= "AND publishing_status < 2 ORDER BY start_date";
		$this->select(array('id', 'topic', 'start_date', 'end_date'), $clause);
	}
	
	private function getCalendarData($month){
		$this->getDayOfMonth($month);
		$calendarArray = array();
		for($i=1; $i<=31; $i++){
			foreach($this->data as $key => $value){
				if($i >= $value['start'] and $i <= $value['end']){
					$calendarArray[$i]['class'] = 'active_activity';
					$calendarArray[$i]['link'][] = 'activity/'.$key;
				}
			}
		}
		return $calendarArray;
	}
	
	private function getDayOfMonth($month){
		foreach($this->data as $key => $value){
			$this->data[$key]['start'] = $this->processDayOfMonth($value['start_date'], $month);
			$this->data[$key]['end'] = $this->processDayOfMonth($value['end_date'], $month);
		}
	}
	
	private function processDayOfMonth($date, $month){
		$time = strtotime($date);
		$dateMonth = (int) date('n', $time);
		if($dateMonth < $month) return -1;
		elseif($dateMonth > $month) return 1000;
		else return (int) date('j', $time);
	}
	
	private function executeRender(){
		head_add_css('activity.css');
		$this->prepareRender();
		$render = $this->renderWriteIcon();
		$render.= render($this->dataByID, 'activity.tpl', 'activity');
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
		$operation = $this->getExtendedOperation();
		$this->dataByID['addition_operation'] = $operation[$this->id];
	}
	
	private function getAppearanceConfiguration(){
		$appearance = cc('activity_appearance');
		if(!is_array($appearance)) $appearance = array();
		$this->dataByID['show_avatar'] = in_array('avatar', $appearance);
		$this->dataByID['show_category'] = in_array('category_icon', $appearance);
	}
}
?>
