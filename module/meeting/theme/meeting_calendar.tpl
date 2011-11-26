<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Meeting')?> : <?=$month?></h3>
<?=$write_icon?>
<?=$calendar?>
<h4 class="small_label"><?=tt('List of Meeting')?></h4>
<ul>
<?php foreach($data as $key => $value):?>
	<li>
		<?=render_operation_link('meeting', $key, $value['topic'])?>
		@ <?=$value['meeting_date']?>
		<label><?=tt('from')?></label> <?=$value['meeting_time']?>
		<label><?=tt('to')?></label> <?=$value['end_time']?>
		(<?=$value['meeting_period']?> <?=tt('hours')?>)
	</li>
<?php endforeach?>
</ul>
