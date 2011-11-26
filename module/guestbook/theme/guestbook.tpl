<?php if(!defined('XIMPLE_CMS')) die();?>
<h2 class="list_label"><?=tt('Guestbook')?></h2>
<?=render_write_icon('guestbook_write', tt('Please sign on our website.'))?>
<?php foreach($data as $key => $value):?>
<div class="guestbook">
	<h3 class="title_label"><?=$value['writer']?></h3>
	<p>
		<?=$value['guestbook']?>
	</p>
	<p class="operation_bar">	
		<span><?=$value['write_time']?></span>
		<?php if($is_admin):?>
			<?=render_operation_link('guestbook_edit', $value['id'], ('edit'))?>
			<?=render_operation_link('guestbook_drop', $value['id'], ('drop'))?>
		<?php endif?>
	</p>
</div>
<?php endforeach?>
