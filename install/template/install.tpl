<?='<?xml version="1.0" encoding="UTF-8"?>';?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
	<head>
		<title><?=$title;?> : X-BLC Installation</title>
		<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
		<link rel="shortcut icon" href="<?=INSTALL_ROOT_URI?>files/xblc.png" type="image/png" />
		<link rel="stylesheet" type="text/css" href="<?=INSTALL_ROOT_URI?>install/css/main.css" />
	</head>
	<body>
		<div id="personal_bar">
			<form action="<?=ROOT_URI?>install" method="get">
				<?=tt('Select language');?>
				<?=$select_langauge?>
			</form>
		</div>
		<div id="head">
			<div id="install_label">
				<h1><?=$title;?></h1>
				<h1><?=tt('XimpleCMS Installation');?></h1>
			</div>
			<div id="install_icons">
				<p><?=tt('Installation Step');?></p>
				<?=$step?>
			</div>
		</div>
		<div id="main">
			<div id="body">
				<div id="content">
					<?=$body;?>
				</div>
			</div>
			<div id="foot">
				Thank for choosing <a href="http://www.ximplesoft.com/">XimpleCMS</a>. OpenSource under <a href="http://www.gnu.org/licenses/gpl-2.0.html">GPL</a>.
			</div>
		</div>
	</body>
</html>
