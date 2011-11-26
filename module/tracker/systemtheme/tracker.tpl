<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label">
	<?=$title?>
	<?php if($selected > 1):?>
		: <?=sprintf(tt('%d days ago'), $selected)?>
	<?php elseif($selected == 1):?>
		: <?=tt('Yesterday')?>
	<?php else:?>
		: <?=tt('Today')?>
	<?php endif?>
	
	<?php if(!empty($action)):?>
		: <?=$action?>
	<?php endif?>
</h3>
<p>
	<?=tt('Tracker shows you actions of a User in the last days.')?>
</p>
<ul class="tracker_group">
	<li><a href="<?=SECTION_URI.Q?>tracker/"><?=tt('Today')?></a></li>
<?php foreach($day as $key => $value ):?>
	<?php if($value > 1):?>
		<li><a href="<?=SECTION_URI.Q?>tracker/<?=$uid?>/day/<?=$value?>"><?=sprintf(tt('%d days ago'), $value)?></a></li>
	<?php elseif($value == 1):?>
		<li><a href="<?=SECTION_URI.Q?>tracker/<?=$uid?>/day/<?=$value?>"><?=tt('Yesterday')?></a></li>
	<?php endif?>
<?php endforeach?>
</ul>
<p style="clear:both;"></p>

<h4><?=tt('Tracker Action')?></h4>
<ul class="tracker_group" id="tracker_legend">
<?php foreach($action_number as $key => $value ):?>
	<li class="tracker_<?=$key?>">
		<a href="<?=SECTION_URI.Q?>tracker/<?=$uid?>/day/<?=$selected?>/action/<?=$key?>"><?=$key?> (<?=$value?>)</a>
	</li>
<?php endforeach?>
</ul>
<p style="clear:both;"></p>
<div class="list">
	<table width="100%" id="tracker_table"><tbody>
		<tr>
			<th><?=tt('content entry')?></th>
			<th><?=tt('writer')?></th>
			<th><?=tt('user action')?></th>
		</tr>
	<?php foreach($tracker as $key => $value ):?>
		<tr class="tracker_<?=$value[5]?>">
			<td><a href="<?=SECTION_URI.Q.$value[0]?>/<?=$value[1]?>"><?=$value[2]?></a></td>
			<?php if($value[3] != -1):?>
				<td><a href="<?=SECTION_URI.Q?>profile/<?=$value[3]?>"><?=$value[4]?></a></td>
			<?php else:?>
				<td><?=$value[4]?></td>
			<?php endif?>
			<td><?=$value[5]?></td>
		</tr>
	<?php endforeach?>
	</tbody></table>
</div>
