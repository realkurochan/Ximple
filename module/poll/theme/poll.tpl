<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="title_label"><?=$topic?></h3>
<?=$poll?>
<p><?=$description?></p>
<p class="poll_info">
	<?=$create_date?> | <?=tt('by')?> <?=render_profile_link($writer)?>
</p>
<p class="operation_bar">
	<span>read <!--{ximple_read/poll/<?=$id?>}--></span>
	<?php if($editable):?>
		<?=render_operation_link('poll_edit', $id, tt('edit'))?>
		<?=render_operation_link('poll_drop', $id, tt('drop'))?>
	<?php endif?>
</p>

