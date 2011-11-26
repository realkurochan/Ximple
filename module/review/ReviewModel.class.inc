<?php
class ReviewModel extends Model{
	function ReviewModel($revieweeMode, $revieweeID){
		$this->reviewer = $this->owner();
		$this->isList = true;
		// Instead of reviewee => reviewee_mode + reviewee_id.
		// For more generic application.
		$this->reviewee_mode = $this->hidden($revieweeMode, 'int');
		$this->reviewee_id = $this->hidden($revieweeID, 'int');
		$this->create_date = $this->now();
		$starOption = array(
			20  => tt('*'),
			40  => tt('**'),
			60  => tt('***'),
			80  => tt('****'),
			100 => tt('*****'),
		);
		$this->star = $this->select(tt('star'), $starOption);
		$this->description = $this->shortText(tt('description'));
		$this->description->isReachable = true;
		// point1, point2 are bad name.
		$this->positive_point = $this->hidden(1, 'int');
		$this->negative_point = $this->hidden(0, 'int');
		$this->section = $this->section();
		
		// Dummy column (no DB, no Form) to show caller link in List Page.
		$this->caller = $this->listDummy(tt('reviewee'));
	}
}
?>
