<?php if(!defined('XIMPLE_CMS')) die();?>
<?php foreach($form as $key => $value):?>
	<?php if(isset($label[$key])): ?>
		<p><label style="font-weight:bold;"><?=$label[$key]?>
		<?php if(in_array($key, $not_null)):?>
			<span style="color:red;font-weight:bold;">*</span>
		<?php endif?>
		</label></p>
	<?php endif?>
	<?=$value?>
	<?php if(isset($help[$key])): ?>
		<p class="form_help"><?=$help[$key]?></p>
	<?php endif?>
<?php endforeach?>
