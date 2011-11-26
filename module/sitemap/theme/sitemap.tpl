<?php if(!defined('XIMPLE_CMS')) die();?>
<ul>
<?php foreach($sitemap as $key => $value):?>
	<li><a href="<?=SECTION_URI.$value['uri']?>"><?=$value['label']?></a></li>
	<?php if(isset($children[$key])):?>	
		<?=$children[$key]?>
	<?php endif?>
<?php endforeach?>
</ul>
