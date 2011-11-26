<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=$title?></h3>
<p><?=$body?></p>
<?php if($is_admin):?>
<p class="operation_bar">
	<?=render_operation_link('about_edit', SECTION, tt('edit'))?>
</p>
<?php endif?>
