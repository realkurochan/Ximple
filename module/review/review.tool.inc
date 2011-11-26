<?php
function review_rating_create_table(){
	include_once('ReviewRatingModel.class.inc');
	include_once(INCLUDE_PATH.'class/QuickTableCreator.class.inc');
	$model = new ReviewRatingModel();
	$tableCreator = new QuickTableCreator('review_rating', $model);
	$tableCreator->create();
}
?>
