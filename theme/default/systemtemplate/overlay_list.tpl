<?php if(!defined('XIMPLE_CMS')) die();?>
<?php if(strlen($description)):?>
	<p class="form_help"><?=$description?></p>
<?php endif?>
<div class="list">
	<table width="100%"><tbody>
		<tr>
		<?php foreach($column as $key => $col):?>
			<th style="min-width:100px;">
				<?=$label[$col]?>
			</th>
		<?php endforeach?>
			<th colspan="2"><?=tt('operation')?></th>
		</tr>
		
		<?php foreach($data as $id => $value):?>
		<tr>
			<?php foreach($column as $key => $col):?>
				<?php if(substr($value[$col], 0, 4) == '<img'):?>
					<td align="center">
				<?php else:?>
					<td>
				<?php endif?>
				
				<?php if(isset($mode[$col]) and !(isset($value['avoidLink']) and $value['avoidLink'])):?>
					<a href="<?=SECTION_URI.Q.$mode[$col].'/'.$value['id']?>" target="_blank"><?=$value[$col]?></a></td>
				<?php else:?>
					<?=$value[$col]?></td>
				<?php endif?>
			<?php endforeach?>
			<td style="width:20px;" align="center">
				<a href="#" onclick="return <?="overlayEdit('$form_id', '$module', $id)"?>">
					<img src="<?=ROOT_URI.'files/icon/edit.png'?>" border="0" >
				</a>
			</td>
			<td style="width:20px;" align="center">
				<a href="#" onclick="return <?="overlayDrop('$form_id', '$module', $id)"?>">
					<img src="<?=ROOT_URI.'files/icon/drop.png'?>" border="0" >
				</a>
			</td>
		</tr>
		<?php endforeach?>
	</tbody></table>
</div>
