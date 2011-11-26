<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=$title?></h3>
<?=$personal_icon?>
<?=$write_icon?>
<?php foreach($data as $id => $card):?>
<div class="develcard_item" style="border-left:4px solid <?=$card['priority_color']?>;">
	<h4><a href="<?=SECTION_URI.Q.'develcard/'.$id?>"><?=$card['topic']?></a></h4>
	<p>
		<label><?=tt('priority')?> : </label><?=$card['priority']['label']?>

		<label><?=tt('status')?> : </label>
		<?=render_operation_link($card['status_uri'], null, $card['status']['label'])?>

		<label><?=tt('type')?> : </label>
		<?=render_operation_link($card['type_uri'], null, $card['type']['label'])?>
	</p>

	<p>
		<label><?=tt('dead line')?> : </label>
		<?=$card['deadline']?> (<?=$card['residual_time']?>)
	</p>

	<p class="operation_bar">
		<?=render_operation_link('develcard', $id, tt('see detail'))?>
		<?=render_operation_link('develcard_edit', $id, tt('edit'))?>
		<?=render_operation_link('develcard_drop', $id, tt('drop'))?>
	</p>
</div>
<?php endforeach?>
