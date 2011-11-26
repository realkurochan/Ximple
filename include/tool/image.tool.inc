<?php
if(class_exists('Imagick')){
	require_once('image.imagick.tool.inc');
}else{
	require_once('image.gd.tool.inc');
}
?>
