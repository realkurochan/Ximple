<?php if(!defined('XIMPLE_CMS')) die();?>
<h4 class="title_label"><?=sprintf(tt('Weekly Workloads & Karma (%d)'), $point_year)?></h4>
<div style="float:left;padding:0 10px;background:<?=$load_bg_color?>;color:<?=$load_color?>;"><?=tt('total workload')?> : <?=$sum_load?></div>
<div style="float:left;padding:0 10px;background:<?=$point_bg_color?>;color:<?=$point_color?>;"><?=tt('total karma')?> : <?=$sum_point?></div>
<div style="clear:both;height:20px;"></div>
<?php if(!empty($first_week) and !empty($last_week)):?>
	<?php for($i=$first_week;$i<=$last_week;$i++):?>
		<div style="display:block;">
			<div style="float:left;margin-right:15px;">
				<?=render_operation_link('develcard_week_list', "resolver/$resolver/week/$i/year/$point_year", $i)?>. :
			</div>
		<?php if(isset($point[$i])):?>
			<div style="float:left;padding:0 10px;height:18px;width:<?=$point[$i]['load']*$pixel?>px;background:<?=$load_bg_color?>;color:<?=$load_color?>;"><?=$point[$i]['load']?></div>
			<div style="float:left;padding:0 10px;height:18px;width:<?=$point[$i]['point']*$pixel?>px;background:<?=$point_bg_color?>;color:<?=$point_color?>;"><?=$point[$i]['point']?></div>
		<?php else:?>
			<?=tt('no action')?>
		<?php endif?>
		<div style="clear:both;"></div>
		</div>
	<?php endfor?>
<?php endif?>
<h5 class="small_label"><?=tt('See another years')?></h5>
<ul>
<?php for($i=$first_year;$i<=$last_year;$i++):?>
	<li><?=render_operation_link('develcard_personal/point_year', $i, $i)?></li>
<?php endfor?>
</ul>
