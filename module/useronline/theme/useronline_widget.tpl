<?php if(!defined('XIMPLE_CMS')) die();?>
<?php foreach($user as $key => $value):?>
	<?=render_profile_link($value)?></li>
<?php endforeach?>
