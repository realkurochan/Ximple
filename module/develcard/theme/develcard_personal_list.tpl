<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Personals for DevelCard')?></h3>
<h4 class="title_label"><?=tt('List of Resolvers')?></h4>
<ul>
<?php foreach($resolver as $uid => $name):?>
	<li><?=render_operation_link('develcard_personal', $uid, $name)?></li>
<?php endforeach?>
</ul>

<h4 class="title_label"><?=tt('List of Reporters')?></h4>
<ul>
<?php foreach($reporter as $uid => $name):?>
	<li><?=render_operation_link('develcard_personal', $uid, $name)?></li>
<?php endforeach?>
</ul>
