<?php

function theme_get_info_default(){
	$description = tt('Default and reference theme for Ximple.');
	$description.= tt('Fixed or fluid width, 2 or 3 columns, tableless, recolorable, flexible and highly configurable.');
	$description.= tt('The theme can be applied for many purposes.');
	return array(
		'name' => 'FlexiFun',
		'description' =>  $description,
		'preview' => 'preview.png',
		'widget_position' => array(
			'left' => 'Left Sidebar',
			'right' => 'Right Sidebar',
			'header' => 'Header',
			'top' => 'Content Top',
			'bottom' => 'Content Bottom',
			'foot' => 'Foot',
		),
		'widget_primary_position' => 'left',
		'menu_include_widget' => true,
		'additional_config' => array(
			
		),
	);
}

?>
