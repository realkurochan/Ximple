<?php

include('FlexiFunConfig.class.inc');

function theme_execute_config_form(){
	$config = new FlexiFunConfig();
	return $config->render();
}

function theme_execute_config_save(){
	$config = new FlexiFunConfig();
	return $config->save();
}

?>
