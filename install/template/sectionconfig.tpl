<form enctype="multipart/form-data" method="post" action="index.php?mode=step5" name="xblcform">
	<label><?=tt('section name')?></label>
	<input name="section_name" value="" size="40" />
	<label><?=tt('section description')?></label>
	<input name="section_slogan" value="" size="40" />
	<label><?=tt('section pre-configuration')?></label>
	<?php $check='checked="checked"';?>
	<?php foreach($preconf as $key => $value):?>
		<input type="radio" name="preconfig" value="<?=$key?>" <?=$check?> /> <?=$value['label']?>
		<p class="help_text"><?=$value['description']?></p>
		<?php $check='';?>
	<?php endforeach?>
	<input type="button" value="<?=tt('Back')?>" onclick="back()" style="display:inline;">
	<input type="submit" value="<?=tt('Next')?>" style="display:inline;"/>
</form>
