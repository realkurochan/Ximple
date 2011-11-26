<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="list">
<h3 class="list_label"><?=tt('Manage Module')?></h3>
<h4>legends</h4>
<ul>
	<li><b>enable</b> : <?=tt('Check this field and submit. Checked modules will be activated.')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/stop.png" border="0" /> : <?=tt('deactivate module')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/correct.png" border="0" /> : <?=tt('activate module')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/refresh_16.png" border="0" /> : <?=tt('refresh module')?></li>
</ul>
<form action="<?=SECTION_URI.Q?>module_init" method="post">
<table width="100%"><tbody>
	<tr>
		<th width="20%"><?=tt('module name / description')?></th>
		<th width="10%"><?=tt('enable')?></th>
		<th width="10%"><?=tt('activate')?></th>
		<th width="10%"><?=tt('refresh')?></th>
	</tr>
	<?php if($has_private):?>
	<tr>
		<td colspan="4">
			<strong><?=tt('Private Module')?></strong>
			<p><?=tt('The followed modules can be used only in this Section.')?></p>
		</td>
	</tr>
	<?php endif?>
	<?php $previous=true?>
	<?php foreach($module as $name => $value):?>
	<?php if($value['is_private']):?>
		<tr class="private_module">
	<?php elseif($previous and $has_private):?>
		<tr>
			<td colspan="4"><strong><?=tt('Public Module')?></strong></td>
		</tr>
		<tr>
		<?php $previous=false?>
	<?php else:?>
		<tr>
	<?php endif?>
		<td width="80%">
			<p><strong><?=$value['name']?></strong></p>
			<p><?=$value['description']?></p>
		</td>
		<td align="center">
			<?php if($value['installed']) : ?>
				<input type="checkbox" value="<?=$name?>" name="<?=$name?>" checked="checked" />
			<?php else:?>
				<input type="checkbox" value="<?=$name?>" name="<?=$name?>" />
			<?php endif?>
		</td>
		<td align="center">
		<?php if($value['installed']) : ?>
			<a href="<?=SECTION_URI.Q?>module_deactivate/module_name/<?=$name?>">
				<img src="<?=ROOT_URI?>files/icon/stop.png" border="0" />
			</a>
		<?php else:?>
			<a href="<?=SECTION_URI.Q?>module_activate/module_name/<?=$name?>">
				<img src="<?=ROOT_URI?>files/icon/correct.png" border="0" />
			</a>
		<?php endif?>
		</td>
		<td align="center">
			<a href="<?=SECTION_URI.Q?>module_refresh/module_name/<?=$name?>">
				<img src="<?=ROOT_URI?>files/icon/refresh_16.png" border="0" />
			</a>
		</td>
	</tr>
	<?php endforeach?>
</tbody></table>
<input type="submit" value="<?=tt('submit Module configuration')?>"/>
</form>
</div>
