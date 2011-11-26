<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="forum">
<?foreach ($category as $key => $value):?>
	<?php if(isset($data[$key]) and count($data[$key])):?>
	<h3 class="list_label"><?=$value['category']?></h3>
	<p>
		<?php if(strlen($value['pic'])):?>
		<img src="<?=ROOT_URI.$value['pic']?>" align="middle" alt="<?=$value['category']?>" />
		<?php endif?>
		<?=$value['description']?>
	</p>
	<div class="list">
	<table width="100%">
		<tr>
			<th style="width:70%;"><?=tt('Forum')?></th>
			<th style="min-width:80px;width:15%;"><?=tt('topics')?></th>
			<th style="min-width:80px;width:15%;"><?=tt('comments')?></th>
		</tr>
		<?php if(isset($data[$key])):?>
			<?foreach ($data[$key] as $forum):?>
			<tr>
				<td>
					<h4 class="title_label"><a href="<?=SECTION_URI.Q.'forum_entry_list/forum/'.$forum['id']?>"><?=$forum['name']?></a></h4>
					<p><?=$forum['description']?></p>
					<?=forum_render_last_entry($forum['last_entry'])?>
				</td>
				<td align="center">
					<?=$forum['topic_no']?>
				</td>
				<td align="center">
					<?=$forum['comment_no']?>
				</td>
			</tr>
			<?php endforeach?>
		<?php endif?>
	</table>
	</div>
	<?php endif?>
<?php endforeach?>
</div>
