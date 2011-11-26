<?php if(!defined('XIMPLE_CMS')) die();?>
<?php foreach($data as $key=>$value):?>
	<div class="video">
		<h3 class="title_label"><a href="<?=render_uri('video', $value)?>"><?=$value['topic']?></a></h3>
		<table width="100%" ><tbody><tr>
			<?php if(strlen($value['icon'])):?>
				<td>
					<img src="<?=ROOT_URI.$value['icon']?>" style="padding:10px;"/>
				</td>
			<?php endif?>
			<td valign="top">
				<?=$value['content']?>
			</td>
			<?php if(cc('video_show_category')):?>
				<?=render_category_icon($value['category'], 'video')?>
			<?php endif?>
			</td>
		</tr></tbody></table>
		<p class="video_info">
			<?=$value['publish_time']?> | <?=tt('by')?> <?=render_profile_link($value['writer'])?> | <?=tt('tags')?>
			<?php foreach($value['freetag'] as $tag):?>
				<a href="<?=SECTION_URI.Q?>video/tag/<?=$tag?>" class="tag"><?=$tag?></a>
			<?php endforeach?>
		</p>
		<p class="operation_bar">
			<span>read <!--{ximple_read/video/<?=$value['id']?>}--></span>
			<?php if(isset($value['comment_no'])):?>
				<span><?=tt('comments')?> <?=$value['comment_no']?></span>
			<?php else:?>
				<span><?=tt('no comment')?></span>
			<?php endif?>
			<a href="<?=ROOT_URI.$value['file']?>"><?=tt('download')?></a>
			<?php if($value['editable']):?>
				<?=render_operation_link('video_edit', $value['id'], ('edit'))?>
				<?=render_operation_link('video_drop', $value['id'], ('drop'))?>
			<?php endif?>
			<a href="<?=SECTION_URI.Q?>video/<?=$value['id']?>"><?=tt('read more')?></a>
		</p>
	</div>
<?php endforeach?>
