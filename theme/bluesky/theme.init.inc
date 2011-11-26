<?php

function theme_get_info_bluesky(){
	return array(
		'name' => 'Blue Sky',
		'description' => tt('Fixed width, 2 columns, tableless, blue color, not configurable, designed for web-blog.'),
		'preview' => 'preview.png',
		'widget_position' => array(
			'left' => 'Left Sidebar',
			'foot' => 'Foot',
		),
		'widget_primary_position' => 'left',
		'menu_include_widget' => true,
	);
}

?>
