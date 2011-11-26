<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="forum">
	<h3 class="title_label"><?=$topic?></h3>
	<table style="width:100%;"><tbody>
	<tr>
		<td>
			<?=$content?>
		</td>
		<?php if($show_avatar or $show_level):?>
			<td valign="top" align="center">
			<?php if($show_avatar):?>
				<?=render_avatar($writer)?>
			<?php endif?>
			<?php if($show_level):?>
				<?=forum_render_level($writer['id'])?>
			<?php endif?>
			</td>
		<?php endif?>
	</tr>
	</tbody></table>
	<p class="forum_info">
		<?=$write_time?> | <?=tt('by')?> <?=render_profile_link($writer)?>
	</p>
	<p class="operation_bar">
		<span>read <!--{ximple_read/forum_entry/<?=$id?>}--></span>
		<?=render_print_link('forum_print/'.$id)?>
		<?php if($editable):?>
			<?=render_operation_link('forum_entry_edit', $id, ('edit'))?>
			<?=render_operation_link('forum_entry_drop', $id, ('drop'))?>
		<?php endif?>
		<?php foreach($addition_operation as $link):?>
			<?=$link?>
		<?php endforeach?>
	</p>
</div>
