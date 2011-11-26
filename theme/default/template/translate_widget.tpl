<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="translate_widget">
	<?=tt('Translate to : ')?>
	<?php foreach($language as $key => $value):?>
		<a href="<?=SECTION_URI.Q.'translate_set_language/language/'.$value?>"><img src="<?=ROOT_URI.'files/flag/'.$value.'.png'?>" border="0"/></a>
	<?php endforeach?>
	<a href="<?=SECTION_URI.Q.'translate_unset_language'?>">[<?=tt('reset language')?>]</a>
</div>
