<?php if(!defined('XIMPLE_CMS')) die();?>
<h2 class="list_label"><?=$meta->title?></h2>
<?=$meta->description?>
<p><label style="font-weight:bold;"><?=$captcha['label']?></label></p>
<?=$captcha['form']?>
<p class="form_help"><?=$captcha['help']?></p>
<?php foreach($form as $key => $value):?>
	<?php if(isset($label[$key])):?>
		<p>
			<label style="font-weight:bold;"><?=$label[$key]?></label>
			<?php if(in_array($key, $not_null)):?>
				<span style="color:red;font-weight:bold;">*</span>
			<?php endif?>
		</p>
	<?php endif?>
	<?=$value?>
	<?php if(isset($help[$key])): ?>
		<p class="form_help"><?=$help[$key]?></p>
	<?php endif?>
<?php endforeach?>
<?php foreach($meta->additionalForm as $key => $value):?>
	<?=$value?>
<?php endforeach?>
<?=$config?>
