<?php
function blog_get_publishing_status(){
	$publishing_status = content_get_publishing_status();
	$publishing_status[4] = tt('promote');
	return $publishing_status;
}
?>
