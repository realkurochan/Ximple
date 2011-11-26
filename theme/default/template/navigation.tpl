<?php if(!defined('XIMPLE_CMS')) die();?>
<ul>
	<?php foreach($data as $key => $value):?>
		<?php if($key != 'home'):?>
			<li><a href="<?=SECTION_URI.Q.$key?>"><?=tt($value)?></a></li>
		<?php else:?>
			<li><a href="<?=SECTION_URI?>"><?=tt($value)?></a></li>
		<?php endif?>
	<?php endforeach?>
</ul>
