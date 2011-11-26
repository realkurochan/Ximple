<?php if(!defined('XIMPLE_CMS')) die();?>
<h2 class="seminar"><?=tt('Seminar')?></h2>
<!--?=$owner?-->
<h3 class="title_label">Seminar Summarize</h3>
<table width="100%">
	<tr>
		<th><?=tt('Seminar');?></th>
		<th><?=tt('Number of seminars');?></th>
	</tr>
	<tr>
		<td><?=tt('test');?></td>
		<td><?=render_profile_link($data['id'])?></td>
	</tr>
	<tr>
		<td><?=tt('size');?></td>
		<td><?=$data['size']?></td>
	</tr>
</table><br>
<table width="100%">
	<tr>
		<th><?=tt('Size');?></th>
		<th><?=tt('Number of seminars');?></th>
	</tr>
	<tr>
		<td><?=tt('0 - High');?></td>
		<td><?=render_profile_link($data['owner'])?></td>
	</tr>
	<tr>
		<td><?=tt('priority');?></td>
		<td><?=$data['priority']?></td>
	</tr>
</table>
	<a href="<?=SECTION_URI.Q?>seminar_write"><?=tt('Submit new seminar')?></a><br>
	<a href="<?=SECTION_URI.Q?>seminar_mylist"><?=tt('List My seminars')?></a><br>
	<a href="<?=SECTION_URI.Q?>seminar_list"><?=tt('List All seminars')?></a><br>
	<a href="<?=SECTION_URI.Q?>category/module/seminar"><?=tt('Seminar Category')?></a>
