<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="directory_element_label" style="display:none;"><?=tt('discription and icon of pages')?></div>
<div class="directory_element">
	<?php foreach($item as $id => $value):?>
		<?php if(strlen($value['icon'])):?>
			<div class="page_image_container">
				<img src="<?=ROOT_URI.$value['icon']?>">
			</div>
			<div class="page_content_container">
		<?php else:?>
			<div>
		<?php endif?>
				<h4><a href="<?=render_uri('page', $value)?>"><?=$value['topic']?></a></h4>
				<?=$value['introduction']?>
			</div>
			<div style="clear:both;height:15px;width:100%;"></div>
	<?php endforeach?>
</div>
