<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label">
	<?=tt('System Log')?>
	<?php if($selected > 1):?>
		: <?=sprintf(tt('%d days ago'), $selected)?>
	<?php elseif($selected == 1):?>
		: <?=tt('Yesterday')?>
	<?php else:?>
		: <?=tt('Today')?>
	<?php endif?>
	
	<?php if(!empty($type)):?>
		: <?=str_replace('_', ' ', $type)?>
	<?php endif?>
</h3>
<p>
	<?=tt('System-Log shows you the errors, which occoured in the last days.')?>
	<?=tt('You should monitor the system with System-Log to protect it against feasible attacks.')?>
</p>
<ul class="log_group">
	<li><a href="<?=SECTION_URI.Q?>syslog_render/"><?=tt('Today')?></a></li>
<?php foreach($day as $key => $value ):?>
	<?php if($value > 1):?>
		<li><a href="<?=SECTION_URI.Q?>syslog_render/<?=$value?>"><?=sprintf(tt('%d days ago'), $value)?></a></li>
	<?php elseif($value == 1):?>
		<li><a href="<?=SECTION_URI.Q?>syslog_render/<?=$value?>"><?=tt('Yesterday')?></a></li>
	<?php endif?>
<?php endforeach?>
</ul>
<p style="clear:both;"></p>

<h4><?=tt('System Log Type')?></h4>
<ul class="log_group" id="syslog_legend">
<?php foreach($type_number as $key => $value ):?>
	<li class="syslog_<?=$key?>">
		<a href="<?=SECTION_URI.Q?>syslog_render/<?=$selected?>/type/<?=$key?>"><?=$key?> (<?=$value?>)</a>
	</li>
<?php endforeach?>
</ul>
<p style="clear:both;"></p>
<div class="list">
	<table width="100%" id="syslog_table"><tbody>
		<tr>
			<th><?=tt('time')?></th>
			<th><?=tt('user')?></th>
			<th><?=tt('type')?></th>
			<th><?=tt('message')?></th>
		</tr>
	<?php foreach($log as $key => $value ):?>
		<tr class="syslog_<?=$value[4]?>">
			<td><?=date('H:i:s', $value[0])?></td>
			<?php if($value[1] != -1):?>
				<td><a href="<?=SECTION_URI.Q?>profile/<?=$value[1]?>"><?=$value[2]?></a></td>
			<?php else:?>
				<td><?=$value[2]?></td>
			<?php endif?>
			<td><a href="<?=SECTION_URI.Q?>syslog_render/<?=$selected?>/type/<?=$value[4]?>"><?=$value[4]?></a></td>
			<td>
				<p><?=$value[5]?></p>
				<p><strong><?=tt('URI')?> : </strong><?=$value[3]?></p>
			</td>
		</tr>
	<?php endforeach?>
	</tbody></table>
</div>
