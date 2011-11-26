<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="page_container">
	<?php foreach($item as $id => $value):?>
		<h4><a href="<?=render_uri('page', $value)?>"><?=$value['topic']?></a></h4>
		<?php if(strlen($value['icon'])):?>
			<img src="<?=ROOT_URI.$value['icon']?>" align="left" style="margin-right:10px;">
		<?php endif?>
		<div class="page_tag_content">
			<?=$value['introduction']?>
		</div>
		<p class="page_info">
			<?=$value['publish_time']?> | <?=tt('by')?> <?=render_profile_link($value['writer'])?> | <?=tt('tags')?>
			<?php foreach($value['freetag'] as $key=>$tag):?>
				<a href="<?=SECTION_URI.Q?>page/tag/<?=$tag?>" class="tag"><?=$tag?></a>
			<?php endforeach?>
		</p>
		<p class="operation_bar">
			<span>read <!--{ximple_read/page/<?=$id?>}--></span>
			<?php if(isset($value['comment_no'])):?>
				<span><?=tt('comments')?> <?=$value['comment_no']?></span>
			<?php else:?>
				<span><?=tt('no comment')?></span>
			<?php endif?>
			<?php if($value['editable']):?>
				<?=render_operation_link('page_edit', $id.'/parent/'.$value['parent'], ('edit'))?>
				<?=render_operation_link('page_drop', $id, ('drop'))?>
			<?php endif?>
		</p>
		<p style="clear:both;"></p>
	<?php endforeach?>
</div>
