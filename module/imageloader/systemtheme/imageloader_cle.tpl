<?php if(!defined('XIMPLE_CMS')) die();?>
<style>
.image_container{
	text-align:center;
	margin:10px;
	width:120px;
	float:left;
}

.image_container img{
	max-width:120px;
	max-height:145px;
}

.image_container{
	height:150px;
}
</style>
<form enctype="multipart/form-data" method="post" id="image_upload_form" action="<?=SECTION_URI.Q?>imageloader_simple_upload" onsubmit="return submitImageLoaderCLE();">
	<input type="file" name="file_to_upload" size="25" id="file_to_upload"/>
	<input type="hidden" name="current_dir" value="<?=$current_dir?>"/>
	<input value="uploade image" type="submit" style="width:120px;" >
</form>
<p><strong><?=tt('click on image to select')?> <span id="loader_info" style="color:red;"></span></strong></p>
<p><strong><a href="<?=SECTION_URI?>?mode=file_manage_user&dir=image/" target="_blank" ><?=tt('manage files')?></a></strong></p>
<?php foreach($image as $key => $value):?>
	<div class="image_container">
		<a href="#" onclick="loaderChangeImageCLE('<?=ROOT_URI.$value['src']?>');return false;"><img src="<?=ROOT_URI.$value['src']?>" border="0"/></a>
		<br />
		<strong>size : </strong><?=render_file_size($value['size']);?>
	</div>
<?php endforeach?>
<p style="clear:both;"></p>
