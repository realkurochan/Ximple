<?php if(!defined('XIMPLE_CMS')) die();?>
<script type="text/javascript" src="<?=ROOT_URI?>include/external/cleditor/jquery.cleditor.js" ></script>
<h3 class="list_label"><?=tt('Edit Group-Permission').' : '.$name?></h3>
<p>
	<?=tt('You can use this page to add Users into the User-Group and to give the User-Group the permissions via Roles.')?>
	<?php if($gid == 1):?>
		<?=tt('Admin group has all permission in the Section. You are not alowed to edit the permission of this User-Group.')?>
	<?php endif?>
	<?php if($gid == 3):?>
		<?=tt('Every logged-in person is a user. You are not alowed to edit user-list of this User-Group.')?>
	<?php endif?>
</p>
<div class="list">
	<form enctype="multipart/form-data" method="post" action="<?=SECTION_URI.Q?>group_role_save" name="ximpleform" onsubmit="return formCheck(notNullmessage, labelmessage)" id="role">
	<input type="hidden" name="id" value="<?=$gid?>"/>
	<?php if($gid != 3):?>
		<select style="display:none;position:absolute;" id="complete_option" multiple="multiple" size="5"></select>
		<p><label style="font-weight:bold;"><?=tt('add new users')?></label></p>
		<input name="insert_user" value="" size="35" maxlength="80" class="auto_complete" id="message_receiver" rel="userinfo_get_ajax_name"/>
		<p class="form_help"><?=
			tt('Type beginning letters of the user name. ').
			tt('The full name will be automatically completed. ').
			tt('Multiple names are possible using comma.')
		?></p>
		<input type="hidden" name="drop_user" id="drop_user" value=""/>
		<div id="role_user_list"><ul>
			<?php foreach($user as $id => $value):?>
				<li id="user_item_<?=$id?>">
					<a href="javascript:dropRoleUser(<?=$id?>)">
						<img src="<?=ROOT_URI?>files/icon/drop.png" border="0" align="left">
					</a>
					<?=$value['login_name']?>
				</li>
			<?php endforeach?>
			<li id="holder">&#160;</li>
		</ul></div>
	<?php endif?>
	<?php if($gid != 1):?>
	<table width="100%"><tbody>
		<tr>
			<th><?=tt('Role name')?></th>
			<th><?=tt('Description')?></th>
			<th><?=tt('Permission')?></th>
		</tr>
		<tr>
			<td colspan="3">
				<strong><?=tt('System-Role')?> : </strong>
				<?=tt('Be careful with the System-Role. User-Groups with this permissions have the strong influence on the behavior of the Section.')?>
			</td>
		</tr>
		<?php $module_label=true?>
		<?php foreach($role as $key => $value):?>
			<?php if($value['is_system']):?>
				<tr class="system_role">
			<?php elseif($module_label):?>
				<?php $module_label=false;?>
				<tr>
					<td colspan="3">
						<strong><?=tt('Module-Role')?> : </strong>
						<?=tt('Roles from the initialized Modules.')?>
					</td>
				</tr>
			<?php else:?>
				<tr>
			<?php endif?>
			<td><?=$value['name']?></td>
			<td><?=$value['description']?></td>
			<td align="center">
			<?php if($value['is_role']):?>
				<input type="checkbox" name="role[]" value="<?=$key?>" checked="checked"/>
			<?php else:?>
				<input type="checkbox" name="role[]" value="<?=$key?>"/>
			<?php endif?>
			</td>
		</tr>
		<?php endforeach?>
	</tbody></table>
	<?php endif?>
	<input type="submit" value="submit permission configuration"/>
	</form>
</div>
<script>
	function dropRoleUser(uid){
		var drop = $('#drop_user');
		var val = drop.val();
		drop.val(val+uid+',');
		$('#user_item_'+uid).hide('blind');
	}
</script>
