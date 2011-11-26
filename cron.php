<?php
	if(PHP_SAPI === 'cli'){
		define('SECTION', 1);
		define('SECTION_PATH', dirname(__FILE__).'/');
		define('XIMPLE_CRON_PROCESS', 'Cron Process');
		require('include/tool/boot.tool.inc');
		boot_cron();
	}else{
		die('Cron Job can only be started from CLI.');
	}
?>
