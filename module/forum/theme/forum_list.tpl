<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="forum">
<div class="list">
	<h4 class="list_label"><?=sprintf(tt('List of Forum Entry in %s'), $forum['name'])?></h4>
	<p><?=$forum['description']?></p>
	<?php if($writable):?>
		<p><?=render_write_icon('forum_entry_write/forum/'.gg('forum'), sprintf(tt('Write new Forum Entry in %s'), $forum['name']))?></p>
	<?php endif?>
	<table width="100%">
		<tr>
			<th></th>
			<th><?=tt('topic')?></th>
			<th><?=tt('reads')?></th>
			<th><?=tt('comments')?></th>
			<th><?=tt('last commented')?></th>
		</tr>
	<?php foreach($data as $key => $value):?>
		<tr>
			<td style="padding:0;">
				<img src="<?=ROOT_URI?>module/forum/theme/<?=$value['icon']?>.png" style="padding:0;"/>
			</td>
			<td>
				<a href="<?=SECTION_URI.Q.'forum_entry/'.$value['id']?>"><?=$value['topic']?></a>
				<br />
				<?=render_profile_link($value['writer'])?> @ <?=$value['write_time']?>
			</td>
			<td>
				<!--{ximple_read/forum_entry/<?=$value['id']?>}-->
			</td>
			<td>
				<?php if(isset($value['comment_no'])):?>
					<?=$value['comment_no']?>
				<?php else:?>
					<?=tt('no comment')?>
				<?php endif?>
			</td>
			<td>
				<?php if(strlen($value['last_commentator'])):?>
					<?=$value['last_commentator']?>
					<br />
					<?=$value['comment_time']?>
				<?php else:?>
					-
				<?php endif?>
			</td>
		</tr>
	<?php endforeach?>
	</table>
	<?=$pager?>
	<ul style="float:left;">
		<li><img src="<?=ROOT_URI?>module/forum/theme/default.png" /> : <?=tt('regular topic')?></li>
		<li><img src="<?=ROOT_URI?>module/forum/theme/stick.png" /> : <?=tt('stick topic')?></li>
		<li><img src="<?=ROOT_URI?>module/forum/theme/new.png" /> : <?=tt('new topic')?></li>
	</ul>
	<ul style="float:left;">
		<li><img src="<?=ROOT_URI?>module/forum/theme/lock.png" /> : <?=tt('locked topic (uncommentable)')?></li>
		<li><img src="<?=ROOT_URI?>module/forum/theme/hot.png" /> : <?=tt('hot topic (more than 15 comments)')?></li>
		<li><img src="<?=ROOT_URI?>module/forum/theme/veryhot.png" /> : <?=tt('very hot topic (more than 25 comments)')?></li>
	</ul>
	<p style="clear:both;"></p>
</div>
</div>
