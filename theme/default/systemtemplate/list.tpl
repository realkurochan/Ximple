<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="list">
	<h3 class="list_label"><?=$meta->title?></h3>
	<?php if(!empty($meta->writeMode)) echo(render_write_icon($meta->writeMode, $meta->writeLabel));?>
	<p><?=$meta->description?></p>
	<table width="100%"><tbody>
		<tr>
		<?php foreach($column as $key => $col):?>
			<th style="min-width:100px;">
				<?php if(in_array($col, $is_sort)):?>
					<a href="<?=SECTION_URI.Q.$sort_mode?>/order_desc/<?=$col?>">
						<img src="<?=ROOT_URI?>files/icon/uparrow.png" border="0" style="padding:0;margin:0;">
					</a>
				<?php endif?>
				<?=$label[$col]?>
				<?php if(in_array($col, $is_sort)):?>
					<a href="<?=SECTION_URI.Q.$sort_mode?>/order/<?=$col?>">
						<img src="<?=ROOT_URI?>files/icon/downarrow.png" border="0">
					</a>
				<?php endif?>
			</th>
		<?php endforeach?>
		<?php if(count($meta->operation)):?>
			<th colspan="<?=count($meta->operation)?>"><?=tt('operation')?></th>
		<?php endif?>
		</tr>
		
		<?php foreach($data as $id => $value):?>
		<tr>
			<?php foreach($column as $key => $col):?>
				<?php if(is_array($value[$col])) $value[$col] = $value[$col]['label'];?>
				<?php if(substr($value[$col], 0, 4) == '<img'):?>
					<td align="center">
				<?php else:?>
					<td>
				<?php endif?>
				
				<?php if(isset($mode[$col]) and !(isset($value['avoidLink']) and $value['avoidLink'])):?>
					<a href="<?=SECTION_URI.Q.$mode[$col].'/'.$value['id']?>"><?=$value[$col]?></a></td>
				<?php else:?>
					<?=$value[$col]?></td>
				<?php endif?>
			<?php endforeach?>
			<?php if(count($meta->operation)):?>
				<?php foreach($meta->operation as $op_mode => $op_data):?>
					<td style="width:20px;" align="center">
						<?php if(isset($op_data['icon'])):?>
							<a href="<?=SECTION_URI.Q.$op_mode.'/'.$value['id'].'/'.$op_data['addition']?>">
								<img alt="<?=$op_data['label']?>" src="<?=ROOT_URI.$op_data['icon']?>" border="0"/>
							</a>
						<?php else:?>
							<a href="<?=SECTION_URI.Q.$op_mode.'/'.$value['id'].'/'.$op_data['addition']?>"><?=$op_data['label']?></a>
						<?php endif?>
					</td>
				<?php endforeach?>
			<?php endif?>
		</tr>
		<?php endforeach?>
	</tbody></table>
</div>
