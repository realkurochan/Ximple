<?php if(!defined('XIMPLE_CMS')) die();?>
<ul style="list-style:circle;">
<?php foreach($item as $key => $value):?>
	<li><a href="<?=SECTION_URI.Q.$mode.'/'.$key?>"><?=$value?></a></li>
<?php endforeach?>
</ul>
