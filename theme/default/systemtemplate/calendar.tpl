<?php if(!defined('XIMPLE_CMS')) die();?>
<?php $week_day = 0;?>
<table class="calendar_table" width="100%">
	<tr>
		<th><?=tt('Sun')?></th>
		<th><?=tt('Mon')?></th>
		<th><?=tt('Tue')?></th>
		<th><?=tt('Wed')?></th>
		<th><?=tt('Thu')?></th>
		<th><?=tt('Fri')?></th>
		<th><?=tt('Sat')?></th>
	</tr>
	<?php if($first_day != 0):?>
		<tr>
		<?php for($i=0; $i<$first_day; $i++):?>
			<td></td>
			<?php $week_day++;?>
		<?php endfor?>
	<?php endif?>
	
	<?php for($i=1; $i<=$day_number; $i++):?>
		<?php if($week_day == 0):?>
			<tr>
		<?php endif?>
		
		<?php if("$year-$month-$i" == $today):?>
			<td id="calendar_today">
		<?php elseif(isset($data[$i]['class'])):?>
			<td class="<?=$data[$i]['class']?>">
		<?php else:?>
			<td>
		<?php endif?>
		
		<?php if(isset($data[$i]['link'])):?>
			<?php if(count($data[$i]['link']) == 1):?>
				<a href="<?=SECTION_URI.Q.$data[$i]['link'][0]?>" ><?=$i?></a>
			<?php else:?>
				<?=$i?><br />
				<?php foreach($data[$i]['link'] as $key => $link):?>
					<a href="<?=SECTION_URI.Q.$link?>" class="calendar_multiple_link"><?=$key + 1?></a>
				<?php endforeach?>
			<?php endif?>
		<?php else:?>
			<?=$i?>
		<?php endif?>
		
		</td>
		
		<?php $week_day++;?>
		
		<?php if($week_day == 7):?>
			<?php $week_day = 0;?>
			</tr>
		<?php endif?>
	<?php endfor?>
	
	<?php if($week_day != 7):?>
		<?php for($i=$week_day; $i<7; $i++):?>
			<td></td>
		<?php endfor?>
		</tr>
	<?php endif?>
</table>

<div id="calendar_detail"></div>

<p class="operation_bar">
	<?=render_operation_link($mode, 'month/'.$prev_month.'/year/'.$prev_year, '&#8249;')?>
	<?=render_operation_link($mode, 'month/'.$month.'/year/'.($year-1), '&#171;')?>
	<span><?=$month_name?> <?=$year?></span>
	<?=render_operation_link($mode, 'month/'.$month.'/year/'.($year+1), '&#187;')?>
	<?=render_operation_link($mode, 'month/'.$next_month.'/year/'.$next_year, '&#8250;')?>
</p>


