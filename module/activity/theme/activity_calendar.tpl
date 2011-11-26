<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Activity')?> : <?=$month?></h3>
<?=$write_icon?>
<?=$calendar?>
<h4 class="small_label"><?=tt('List of Activies')?></h4>
<ul>
<?php foreach($data as $key => $value):?>
	<li>
		<?=render_operation_link('activity', $key, $value['topic'])?>
		@ <?=$value['start_date']?> - <?=$value['end_date']?>
	</li>
<?php endforeach?>
</ul>
