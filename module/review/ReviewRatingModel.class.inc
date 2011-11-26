<?php
class ReviewRatingModel extends Model {
	function ReviewRatingModel(){
		$this->review_id = $this->hidden(null, 'int');
		$this->scorer = $this->hidden(null, 'int');
		$this->point = $this->hidden(null, 'int');
	}
}
?>
