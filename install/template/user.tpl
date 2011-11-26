<?=head_define_js_var();?>

<script type="text/javascript" src="<?=ROOT_URI?>include/external/jquery/jquery.js"></script>
<script type="text/javascript" src="<?=ROOT_URI?>include/external/jquery/jquery-ui.js"></script>
<script type="text/javascript" src="<?=ROOT_URI?>include/javascript/main.js"></script>
<script type="text/javascript" src="<?=ROOT_URI?>include/javascript/form.js"></script>
<script type="text/javascript" src="<?=ROOT_URI?>include/javascript/email.column.js"></script>
<script type="text/javascript" src="<?=ROOT_URI?>module/user/user.js"></script>
<script type="text/javascript" src="<?=ROOT_URI?>include/external/sha1.js"></script>
<script type="text/javascript" src="<?=ROOT_URI?>include/external/cleditor/jquery.cleditor.js" ></script> 
<div class="form">
<a name="edit"></a>
<form enctype="multipart/form-data" method="post" action="index.php?mode=step4" name="xblcform" onsubmit="return checkForm()">
<h2><?=$title?></h2>
	<p>
	<?=tt('<b>Important</b> : Please, remember this information for root\'s loging-in. Root is the person, who can access and manage everything in system.')?>
	</p>
	<label><?=tt('login name')?></label>
	<input name="login_name" value="" size="40" id="user_login_name" rel="install"/>
	<span id="info_user_login_name"></span>
	<label><?=tt('email')?></label>
	<input name="email" value="" size="40" class="email_field" id="user_email"/>
	<span id="info_user_email"></span>
	<label><?=tt('password')?></label>
	<input name="prepassword" value="" size="40" type="password" id="user_password"/>
	<span id="info_user_password"></span>
	<label><?=tt('confirm password')?></label>
	<input name="confirm_password" value="" size="40" type="password" id="user_confirm_password"/>
	<span id="info_user_confirm_password"></span>
	<input name="password" value="" type="hidden"/>
	<input name="section" value="1" type="hidden"/>
	<br /><br />
	<input type="button" value="<?=tt('Back')?>" onclick="back()" style="display:inline;">
	<input type="submit" value="<?=tt('Next')?>" style="display:inline;"/>
</form></div>
