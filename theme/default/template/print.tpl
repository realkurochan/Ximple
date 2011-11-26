<?php if(!defined('XIMPLE_CMS')) die();?>
<?='<?xml version="1.0" encoding="UTF-8"?>'?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
	<head>
		<?=$head?>
	</head>	
	<body onload="window.print();">
		<h1><?=$section_name?> : <?=$section_slogan?></h1>
		<p>
			<a href="#" onclick="window.print();return false;">
				<img src="<?=ROOT_URI?>files/icon/print.png" border="0" />
			</a>
		</p>
		<?=$body?>
	</body>
</html>
