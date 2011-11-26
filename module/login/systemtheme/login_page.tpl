<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="form" id="login_page">
	<h2 class="list_label"><?=tt('Login')?></h2>
	<?=$text?>
	<form action="<?=SECTION_URI.Q?>login_submit" method="post" onsubmit="crypt()">
		<p><label><?=tt('name')?></lable></p>
		<input name="login_name" size="35"/>
		<input name="redirect" value="<?=ORIG_REQUEST?>" type="hidden" />
		<p><label><?=tt('password')?></lable></p>
		<input name="login_prepassword" type="password"  size="35"/>
		<input name="login_password" type="hidden" />
		<input name="login_salt" type="hidden" />
		<input name="login" type="hidden" value="login"/>
		<input type="submit" id="loginsubmit" value="login" style="display:block;margin:5px;"/>
	</form>
	<br />
	<?php if($forget_password):?>
		<p><a href="<?= SECTION_URI.Q?>forgot_password"><?=tt('forget password')?></a></p>
	<?php endif?>
	<?php if($registable):?>
		<p><a href="<?= SECTION_URI.Q?>registration"><?=tt('registration')?></a></p>
	<?php endif?>
</div>
