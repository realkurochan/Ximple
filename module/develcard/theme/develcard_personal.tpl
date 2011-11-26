<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('DevelCard : ').$user['login_name']?></h3>
<?=render_avatar($user)?>
<?=$write_icon?>
<h4 class="title_label"><?=tt('List of DevelCards')?></h4>
<ul>
	<li>
		<?=render_operation_link('develcard_resolver_list', $user['id'], tt('In My Duty'))?>
		[<?=render_operation_link('develcard_resolver_list', $user['id'].'/status/0', tt('created'))?>,
		<?=render_operation_link('develcard_resolver_list', $user['id'].'/status/1', tt('in progress'))?>,
		<?=render_operation_link('develcard_resolver_list', $user['id'].'/status/2', tt('closed'))?>,
		<?=render_operation_link('develcard_resolver_list', $user['id'].'/status/3', tt('failed'))?>,
		<?=render_operation_link('develcard_resolver_list', $user['id'].'/status/4', tt('rejected'))?>]
	</li>
	<li>
		<?=render_operation_link('develcard_reporter_list', $user['id'], tt('Reported By Me'))?>
		[<?=render_operation_link('develcard_reporter_list', $user['id'].'/status/0', tt('created'))?>,
		<?=render_operation_link('develcard_reporter_list', $user['id'].'/status/1', tt('in progress'))?>,
		<?=render_operation_link('develcard_reporter_list', $user['id'].'/status/2', tt('closed'))?>,
		<?=render_operation_link('develcard_reporter_list', $user['id'].'/status/3', tt('failed'))?>,
		<?=render_operation_link('develcard_reporter_list', $user['id'].'/status/4', tt('rejected'))?>]
	</li>
</ul>
<h4 class="title_label"><?=tt('Deadline Calendar')?></h4>
<?=$calendar?>
<?=$point?>
