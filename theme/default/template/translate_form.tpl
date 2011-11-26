<?php if(!defined('XIMPLE_CMS')) die();?>
<p>
	<?=tt('Translate to : ')?>
	<?php foreach($language as $key => $value):?>
		<a href="#" onclick="return selectTranslateLanguage('<?=$value?>', '<?=$module_name?>', '<?=$module_id?>')"><img src="<?=ROOT_URI.'files/flag/'.$value?>.png"></a>
	<?php endforeach?>
</p>
