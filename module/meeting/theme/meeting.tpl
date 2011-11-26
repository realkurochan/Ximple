<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="meeting">
	<h3 class="title_label"><?=$topic?></h3>
	<p class="meeting_time_span">
		<?=$meeting_date?>
		<label><?=tt('from')?></label> <?=$meeting_time?>
		<label><?=tt('to')?></label> <?=$end_time?>
		(<?=$meeting_period?> <?=tt('hours')?>)
	</p>

	<?php if(!$is_summarizable):?>
		<?=$participate_icon?>
		<br />
		<?=$decline_icon?>
	<?php endif?>

	<table  style="width:100%;"><tbody>
	<tr>
		<td>
			<?php if(strlen($icon)):?>
				<img src="<?=ROOT_URI.$icon?>" align="left" style="margin:10px;" alt="Meeting Icon" />
			<?php endif?>
			<?=$content?>
		</td>
		<?php if($show_avatar or $show_category):?>
			<td valign="top" align="center">
				<?php if($show_avatar):?>
					<?=render_avatar($writer)?>
				<?php endif?>
				<?php if($show_category):?>
					<?=render_category_icon($category, 'meeting')?>
				<?php endif?>
			</td>
		<?php endif?>
	</tr>
	<?php if($is_summarizable):?>
		<tr>
			<td colspan="2">
				<h4 class="small_label"><?=tt('Summary')?></h4>
				<?=$summary?>
			</td>
		</tr>
	<?php endif?>
	</tbody></table>
	
	<?php if(count($participant)):?>
		<h4 class="small_label"><?=tt('Paricipants')?></h4>
		<ul>
		<?php foreach($participant as $uid => $user):?>
			<li><?=render_profile_link($user['participant'])?></li>
		<?php endforeach?>
		</ul>
	<?php endif?>
	
	<?php if(count($disclaimer)):?>
		<h4 class="small_label"><?=tt('Disclaimers')?></h4>
		<ul>
		<?php foreach($disclaimer as $uid => $user):?>
			<li><?=render_profile_link($user['participant'])?></li>
		<?php endforeach?>
		</ul>
	<?php endif?>
	
	<p class="meeting_info">
		<?=tt('by')?> <?=render_profile_link($writer)?> | <?=tt('tags')?>
		<?=render_tag($category, $freetag, 'meeting')?>
	</p>
	<p class="operation_bar">
		<span>read <!--{ximple_read/meeting/<?=$id?>}--></span>
		<?=render_print_link('meeting_edit/'.$id)?>
		<?php if($editable):?>
			<?=render_operation_link('meeting_edit', $id, tt('edit'))?>
			<?=render_operation_link('meeting_drop', $id, tt('drop'))?>
		<?php endif?>
		<?php foreach($addition_operation as $link):?>
			<?=$link?>
		<?php endforeach?>
	</p>
</div>
