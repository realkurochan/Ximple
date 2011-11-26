<?php if(!defined('XIMPLE_CMS')) die();?>
<h2 class="develcard"><?=tt('DevelCard')?></h2>
<!--?=$owner?-->
<h3 class="title_label">Develcard Summarize</h3>
<table width="100%">
	<tr>
		<th><?=tt('Type');?></th>
		<th><?=tt('Number of Develcard');?></th>
	</tr>
	<tr>
		<td><?=tt('bug');?></td>
		<td><?=render_profile_link($data['owner'])?></td>
	</tr>
	<tr>
		<td><?=tt('priority');?></td>
		<td><?=$data['priority']?></td>
	</tr>
</table><br>
<table width="100%">
	<tr>
		<th><?=tt('Priority');?></th>
		<th><?=tt('Number of Develcard');?></th>
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
	<a href="<?=SECTION_URI.Q?>develcard_write"><?=tt('Submit new Develcard')?></a><br>
	<a href="<?=SECTION_URI.Q?>develcard_mylist"><?=tt('List My Develcard')?></a><br>
	<a href="<?=SECTION_URI.Q?>develcard_list"><?=tt('List All Develcard')?></a><br>
	<a href="<?=SECTION_URI.Q?>category/module/develcard"><?=tt('Develcard Category')?></a>
