<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="profile">
	<h3 class="list_label"><?=sprintf(tt('Profile of %s'), $user['login_name'])?></h3>
	<p align="center"><?=render_avatar($user, false, true, null, true)?></p>
	<br />
	<?=render_write_icon('profile_write', tt('Add new User-Profile'))?>
	<?php if($readable):?>
		<?php foreach($data as $key => $value):?>
			<h4><?=$key?></h4>
			<table width="100%"><tbody>
			<?php foreach($value as $index => $profile):?>
			<tr>
				<td><?=$profile?></td>
				<?php if($editable):?>
				<td width="10%">
					<a href="<?=SECTION_URI.Q?>profile_edit/<?=$index?>" ><img alt="edit" src="<?=ROOT_URI?>files/icon/edit.png" border="0"/></a>
				</td>
				<td width="10%">
					<a href="<?=SECTION_URI.Q?>profile_drop/<?=$index?>"><img alt="delete" src="<?=ROOT_URI?>files/icon/stop.png" border="0"/></a>
				</td>
				<?php endif?>
			</tr>
			<?php endforeach?>
			</tbody></table>
		<?php endforeach?>
	<?php endif?>
	<?php foreach($additional_profile as $key => $value):?>
		<?=$value?>
	<?php endforeach?>
</div>
