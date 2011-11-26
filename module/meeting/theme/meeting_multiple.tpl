<?php if(!defined('XIMPLE_CMS')) die();?>
<?php foreach($data as $key=>$value):?>
	<div class="meeting">
		<h3 class="title_label"><a href="<?=render_uri('meeting', $value)?>" class="label_link"><?=$value['topic']?></a></h3>
		<table width="100%" ><tbody><tr>
			<td>
				<?php if(strlen($value['icon'])):?>
					<img src="<?=ROOT_URI.$value['icon']?>" align="left" style="margin:10px;" alt="Meeting Icon" />
				<?php endif?>
				<?=$value['content']?>
			</td>
			<?php if($show_category):?>
				<td valign="top" align="center">
					<?=render_category_icon($value['category'], 'meeting')?>
				</td>
			<?php endif?>
		</tr></tbody></table>
		<p class="meeting_info">
			<?=$value['publish_time']?> | <?=tt('by')?> <?=render_profile_link($value['writer'])?> | <?=tt('tags')?>
			<?=render_tag($value['category'], $value['freetag'], 'meeting')?>
		</p>
		<p class="operation_bar">
			<span>read <!--{ximple_read/meeting/<?=$value['id']?>}--></span>
			<?php if(isset($value['comment_no'])):?>
				<span><?=tt('comments')?> <?=$value['comment_no']?></span>
			<?php else:?>
				<span><?=tt('no comment')?></span>
			<?php endif?>
			<?php if($value['editable']):?>
				<?=render_operation_link('meeting_edit', $value['id'], tt('edit'))?>
				<?=render_operation_link('meeting_drop', $value['id'], tt('drop'))?>
			<?php endif?>
			<?=render_operation_link('meeting', $value['id'], tt('read more'))?>
		</p>
	</div>
<?php endforeach?>
