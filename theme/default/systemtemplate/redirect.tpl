<?php if(!defined('XIMPLE_CMS')) die();?>
<?='<?xml version="1.0" encoding="UTF-8"?>'?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
	<head>
		<title>Ximple database redirect</title>
	</head>
	<body>
		<?php if($check_drop):?>
			<script>
				check = confirm("<?=tt('Do you really want to delete data')?>");
				if(check==false) history.back();
				else window.location.href="<?=$confirm_uri?>";
			</script>
		<?php else:?>
			<?php if($is_develop):?>
				<h1 style="text-align:center;">Ximple try to redirect to <a href="<?=$redirect?>">this page</a></h1>
			<?php else :?>
				<script> 
					window.location.href="<?=$redirect?>";
				</script>
			<?php endif?>
		<?php endif?>
	</body>
</html>
