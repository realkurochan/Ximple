<?php if(!defined('XIMPLE_CMS')) die();?>
<?php if($is_label):?>
	<div class="directory_element_label" style="display:none;"><?=tt('sub-directories discription')?></div>
<?php else:?>
	<div class="directory_element_label" style="display:none;"><?=tt('directory discription')?></div>
<?php endif?>
<div class="directory_element">
	<?php foreach($item as $id => $value):?>
		<?php if(strlen($value['icon'])):?>
			<div class="directory_image_container">
				<img src="<?=ROOT_URI.$value['icon']?>">
			</div>
			<div class="directory_content_container">
		<?php else:?>
			<div>
		<?php endif?>
		<?php if($is_label):?>
				<h4><a href="<?=render_uri('directory', $value)?>"><?=$value['name']?></a></h4>
		<?php endif?>
				<?=$value['description']?>
			</div>
		<p style="clear:both;height:15px;"></p>
	<?php endforeach?>
</div>
