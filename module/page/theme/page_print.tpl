<?php if(!defined('XIMPLE_CMS')) die();?>
<h2><?=$parent_name?></h2>
<?php foreach($sort as $key => $value):?>
	<h3><?=$data[$value]['topic']?></h3>
	<?=$data[$value]['content']?>
<?php endforeach?>
