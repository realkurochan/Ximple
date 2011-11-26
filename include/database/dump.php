<?php
require_once('../setup.php');
require_once('../tool/authority.tool.inc');
require_once('../tool/globalvar.tool.inc');
require_once('../tool/dump.tool.inc');
require_once(DB_TYPE.'_new.inc');

if(authority(array(), array(1))) die('Authorization failed!');
else echo('Authorization ok.');
dump();
?>
