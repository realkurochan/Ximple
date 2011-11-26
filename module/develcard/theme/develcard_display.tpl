<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="develcard" style="border-top:4px solid <?=$priority_color?>;">
	<h3 class="title_label"><?=$topic?> </h3>
	<p>
		<label><?=tt('ID')?> : </label>
		#<?=$id?>
	</p>
	<p>
		<label><?=tt('priority')?> : </label>
		<?=$priority['label']?>
	</p>
	<p>
		<label><?=tt('status')?> : </label>
		<?=render_operation_link($status_uri, null, $status['label'])?>
	</p>
	<p>
		<label><?=tt('type')?> : </label>
		<?=render_operation_link($type_uri, null, $type['label'])?>
	</p>
	<p>
		<label><?=tt('dead line')?> : </label>
		<?=$deadline?> (<?=$residual_time?>)
	</p>
	<p><label><?=tt('resolver')?> : </label></p>
	<ul>
	<?php foreach($resolver as $uid => $user):?>
		<li><?=render_operation_link('develcard_personal', $user['resolver']['value'], $user['resolver']['label'].' ('. $user['work_load'].')')?></li>
	<?php endforeach?>
	</ul>
	<p><label><?=tt('description')?> : </label></p>
	<p><?=$description?></p>
	<p>
		<?php if($writer['id'] != -1):?>
			<?=tt('reported by').' '.render_operation_link('develcard_personal', $writer['id'], $reporter_name)?> @ 
		<?php else:?>
			<?=tt('reported by').' '.$reporter_name?> @
		<?php endif?>
		<?=$create_date?>
		<?=tt('tags').' '.render_tag(null, $freetag, 'develcard_tag')?>
	</p>
	<p class="operation_bar">
		<?php if($editable):?>
			<?php if($status['value'] == 0):?>
				<span><?=tt('set to')?></span>
				<?=render_operation_link('develcard_set_status', $id.'/status/1', tt('in progess'))?>
			<?php elseif($status['value'] == 1):?>
				<span><?=tt('set to')?></span>
				<?=render_operation_link('develcard_set_status', $id.'/status/2', tt('closed'))?>
				<?=render_operation_link('develcard_set_status', $id.'/status/3', tt('failed'))?>
				<?=render_operation_link('develcard_set_status', $id.'/status/4', tt('rejected'))?>
			<?php endif?>
		<?php endif?>
		<?=render_operation_link('develcard_edit', $id, tt('edit'))?>
		<?=render_operation_link('develcard_drop', $id, tt('drop'))?>
		<?php foreach($addition_operation as $link):?>
			<?=$link?>
		<?php endforeach?>
	</p>
</div>
