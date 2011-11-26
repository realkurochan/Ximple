<?php if(!defined('XIMPLE_CMS')) die();?>
<?='<?xml version="1.0" encoding="UTF-8"?>'?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
	<head>
		<title><?=$title?></title>
	</head>
	<style>
		#message_container{
			text-align : center;
		}

		#message{
			width : 680px;
			border : 3px solid #999 ;
			margin:120px auto auto auto;
			padding: 50px 20px 50px 20px;
			text-align:left;
			background: #729bd3 url(<?=ROOT_URI?>theme/default/image/xhead.png) 50px 50px no-repeat;
			min-height:120px;
			-moz-border-radius: 8px;
			border-radius:8px;
		}

		#message h1, h2, p{
			color :white;
			margin-top:0px;
			margin-left:180px;
		}

		#message h1{
			text-decoration:underline;
		}
	</style>
	<body>
		<div id="message_container">
			<div id="message">
				<h1><?=$title?></h1>
				<p><?=$message?></p>
			</div>
		</div>
	</body>
</html>
