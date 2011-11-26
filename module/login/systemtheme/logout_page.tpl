<?php if(!defined('XIMPLE_CMS')) die();?>
<div id="login_form">
	<h2 class="list_label"><?=tt('Logout')?></h2>
	<form action="<?= SECTION_URI?>" method="post">
		<input name="login_name" type="hidden" value=""/>
		<input name="redirect" value="" type="hidden" />
		<input name="login_password" type="hidden" />
		<input name="logout" type="hidden" value="logout"/>
		<input name="login_salt" type="hidden" value="nothing"/>
		<input type="submit" id="loginsubmit" value="logout" style="display:block;margin:5px;" />
	</form>
	<br />
</div>
