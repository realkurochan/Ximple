<?php if(!defined('XIMPLE_CMS')) die();?>
<?php $last_key = count($data)-1;?>
<?php $i = 0;?>
<?php foreach($data as $key=>$value):?>
<?php if($i == $last_key):?>
	<a name="last_comment"></a>
<?php endif?>
<?php $i++;?>
<a name="comment_<?=$value['id']?>">
<div class="comment">
	<h3 class="title_label">
		<?php if($value['writer_id']['id'] == -1):?>
			<a href="<?=$value['homepage']?>"><?=$value['writer']?></a>
		<?php else:?>
			<?=render_profile_link($value['writer_id'])?>
		<?php endif?>
	</h3>
	<?php
		
	?>
	<?php if($show_signature):?>
		<?=render_signature($value['writer_id'])?>
	<?php endif?>
	<?php if($value['is_new']):?>
		<p>
			<img src="<?=ROOT_URI?>files/icon/new.png" border="0" style="margin-right:15px;" align="left"/>
			<span style="color:#f70;"><?=tt('new comment')?></span>
		</p>
	<?php endif?>
	<table width="100%">
		<tr>
			<td valign="top"><?=$value['comment']?></td>
			<td valign="top" align="center" style="min-width:120px;" width="10%">
			<?php if($show_avatar):?>
				<?=render_avatar($value['writer_id'], false)?>
				<?=forum_render_level($value['writer_id']['id'])?>
			<?php endif?>
			</td>
		</tr>
	</table>
	<p class="operation_bar">
		<span><?=$value['write_time']?></span>
		<?php if($value['editable']):?>
			<?=render_operation_link('comment_edit', $value['id'].'/module/'.$value['mode'].'/parent/'.$value['parent'], tt('edit'))?>
			<?=render_operation_link('comment_drop', $value['id'].'/module/'.$value['mode'].'/parent/'.$value['parent'], tt('drop'))?>
		<?php endif?>
		<?php foreach($addition_operation[$key] as $link):?>
			<?=$link?>
		<?php endforeach?>
	</p>
</div>
<?php endforeach?>
