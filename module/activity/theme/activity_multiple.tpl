<?php if(!defined('XIMPLE_CMS')) die();?>
<?php foreach($data as $key=>$value):?>
	<div class="activity">
		<h3 class="title_label"><a href="<?=render_uri('activity', $value)?>" class="label_link"><?=$value['topic']?></a></h3>
		<table width="100%" ><tbody><tr>
			<td>
				<?php if(strlen($value['icon'])):?>
					<img src="<?=ROOT_URI.$value['icon']?>" align="left" style="margin:10px;" alt="Activity Icon" />
				<?php endif?>
				<?=$value['content']?>
			</td>
			<?php if($show_category):?>
				<td valign="top" align="center">
					<?=render_category_icon($value['category'], 'activity')?>
				</td>
			<?php endif?>
		</tr></tbody></table>
		<p class="activity_info">
			<?=$value['publish_time']?> | <?=tt('by')?> <?=render_profile_link($value['writer'])?> | <?=tt('tags')?>
			<?=render_tag($value['category'], $value['freetag'], 'activity')?>
		</p>
		<p class="operation_bar">
			<span>read <!--{ximple_read/activity/<?=$value['id']?>}--></span>
			<?php if(isset($value['comment_no'])):?>
				<span><?=tt('comments')?> <?=$value['comment_no']?></span>
			<?php else:?>
				<span><?=tt('no comment')?></span>
			<?php endif?>
			<?php if($value['editable']):?>
				<?=render_operation_link('activity_edit', $value['id'], tt('edit'))?>
				<?=render_operation_link('activity_drop', $value['id'], tt('drop'))?>
			<?php endif?>
			<?=render_operation_link('activity', $value['id'], tt('read more'))?>
		</p>
	</div>
<?php endforeach?>
