<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Whos is online')?></h3>
<ul>
<?php foreach($user as $key => $value):?>
	<li><?=render_profile_link($value)?></li>
<?php endforeach?>
</ul>
