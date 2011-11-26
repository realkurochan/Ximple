<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="activity">
	<h3 class="title_label"><?=$topic?></h3>
	<p class="activity_time_span">
		<label><?=tt('from')?></label> <?=$start_date?>
		<label><?=tt('to')?></label> <?=$end_date?>
	</p>
	<table  style="width:100%;"><tbody>
	<tr>
		<td>
			<?php if(strlen($icon)):?>
				<img src="<?=ROOT_URI.$icon?>" align="left" style="margin:10px;" alt="Activity Icon" />
			<?php endif?>
			<?=$content?>
		</td>
		<?php if($show_avatar or $show_category):?>
			<td valign="top" align="center">
				<?php if($show_avatar):?>
					<?=render_avatar($writer)?>
				<?php endif?>
				<?php if($show_category):?>
					<?=render_category_icon($category, 'activity')?>
				<?php endif?>
			</td>
		<?php endif?>
	</tr>
	</tbody></table>
	<p class="activity_info">
		<?=tt('by')?> <?=render_profile_link($writer)?> | <?=tt('tags')?>
		<?=render_tag($category, $freetag, 'activity')?>
	</p>
	<p class="operation_bar">
		<span>read <!--{ximple_read/activity/<?=$id?>}--></span>
		<?=render_print_link('activity_edit/'.$id)?>
		<?php if($editable):?>
			<?=render_operation_link('activity_edit', $id, tt('edit'))?>
			<?=render_operation_link('activity_drop', $id, tt('drop'))?>
		<?php endif?>
		<?php foreach($addition_operation as $link):?>
			<?=$link?>
		<?php endforeach?>
	</p>
</div>
