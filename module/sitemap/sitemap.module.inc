<?php
execute_add_function('sitemap');
execute_add_function('sitemap_reset');
execute_add_function('sitemap_reset_table');
execute_add_function('sitemap_config');
execute_add_function('sitemap_config_save');

function sitemap(){
	document_set_path(tt('SiteMap'));
	$sitemap = io_unserialize(FILES_PATH.'sitemap/'.SECTION.'/map');
	if(!is_array($sitemap)) $sitemap = sitemap_update();
	$data = array(
		'sitemap' => sitemap_render($sitemap),
	);
	return render($data, 'sitemap_frame.tpl', 'sitemap_frame');
}

function sitemap_reset(){
	$file = FILES_PATH.'sitemap/'.SECTION.'/map.php';
	if(is_file($file)) unlink($file);
}

function sitemap_reset_table(){
	$file = FILES_PATH.'sitemap/'.SECTION.'/table.php';
	if(is_file($file)) unlink($file);
}

function sitemap_config($params, $result){
	$result[tt('SiteMap')] = array(
		'sitemap_enable_topic' => Model::enable(tt('enable SiteMap for all topics (time consuming)')),
	);
	return $result;
}

function sitemap_config_save(){
	config_set('sitemap_enable_topic', pp('sitemap_enable_topic'));
	sitemap_reset();
}

function sitemap_update(){
	$tables = io_unserialize(FILES_PATH.'sitemap/'.SECTION.'/table');
	if(!is_array($tables)) $tables = sitemap_update_table();
	$module = cc('module_name');
	$sitemap = sitemap_get_content_map($tables);
	$sitemap = sitemap_get_module_map($sitemap);
	if(isset($module['directory'])){
		$sitemap = sitemap_get_page_map($sitemap);
	}
	if(cc('sitemap_enable_topic')){
		$sitemap = sitemap_get_topic_map($sitemap, $tables);
	}
	io_serialize($sitemap, FILES_PATH.'sitemap/'.SECTION.'/map');
	return $sitemap;
}

function sitemap_update_table(){
	$tables = db_get_tables();
	$content_tables = array();
	$module = cc('module_name');
	foreach($tables as $key => $table){
		$is_topic = db_exist_column($table, 'topic');
		$is_category = db_exist_column($table, 'category');
		$is_uri = db_exist_column($table, 'uri');
		if($is_topic and $is_category and isset($module[$table]) and $table != 'page'){
			$content_tables[$table] = $is_uri;
		}
	}
	$directory  = FILES_PATH.'sitemap/';
	if(!is_dir($directory)) mkdir($directory);
	$directory .= SECTION.'/';
	if(!is_dir($directory)) mkdir($directory);
	io_serialize($content_tables, $directory.'table');
	return $content_tables;
}


function sitemap_get_content_map($tables){
	$sitemap = array();
	$module = cc('module_name');
	foreach($tables as $table => $is_uri){
		if($table != 'page'){
			$sitemap[$table] = array(
				'label' => $module[$table],
				'uri' => $table,
			);
			$sitemap = sitemap_get_category($sitemap, $table);
		}
	}
	return $sitemap;
}

function sitemap_get_module_map($sitemap){
	$module = cc('module_name');
	foreach($module as $key => $value){
		if(!isset($sitemap[$key]) and $key != 'page' and $key != 'directory'){
			$sitemap[$key] = array(
				'label' => $value,
				'uri' => $key,
			);
		}
	}
	return $sitemap;
}

function sitemap_get_page_map($sitemap){
	$clause = 'WHERE section='.SECTION;
	$column = array('id', 'name', 'configuration', 'parent');
	$directory = db_select('directory', $column, $clause);
	
	$directory_parent = array();
	foreach($directory as $id => $data){
		$directory_parent[$data['parent']][$id] = $data;
	}
	
	$column = array('id', 'topic', 'uri', 'parent');
	$page = db_select('page', $column, $clause);
	
	$page_parent = array();
	foreach($page as $id => $data){
		$page_parent[$data['parent']][$id] = $data;
	}
	
	sitemap_struct_directory($sitemap, $directory_parent, $page_parent);
	
	return $sitemap;
}

function sitemap_get_topic_map($sitemap, $tables){
	foreach($tables as $table => $is_uri){
		$clause = 'WHERE section='.SECTION;
		$column = array('id', 'topic', 'category');
		if($is_uri) $column[] = 'uri';
		$topic = db_select($table, $column, $clause);
		foreach($topic as $id => $data){
			$uri = $table.'/'.$id;
			if($is_uri and strlen($data['uri'])){
				$uri .= '/'.$data['uri'].'.html';
			} 
			$sitemap[$table]['children'][$data['category']]['children'][] = array(
				'label' => $data['topic'],
				'uri' => $uri,
			);
		}
	}
	return $sitemap;
}

function sitemap_get_category($sitemap, $table){
	$clause = 'WHERE section='.SECTION." AND mode='$table'";
	$category = db_select_single_column('category', 'category', $clause);
	foreach($category as $key => $value){
		$sitemap[$table]['children'][$key] = array(
			'label' => $value,
			'uri' => $table.'/tag/'.$value,
		);
	}
	return $sitemap;
}

function sitemap_struct_directory(&$sitemap, $directory, $page, $parent=0){
	if(isset($directory[$parent])){
		foreach($directory[$parent] as $id => $data){
			$did = 'directory/'.$id;
			$sitemap[$did] = array(
				'label' => $data['name'],
				'uri' => 'directory/'.$id,
				'children' => sitemap_get_child_page($page, $id),
			);
			sitemap_struct_directory($sitemap[$did]['children'], $directory, $page, $id);
		}
	}
}

function sitemap_get_child_page($page, $parent){
	if(isset($page[$parent])){
		$child = array();
		foreach($page[$parent] as $id => $data){
			$uri = 'page/'.$id;
			if(isset($data['uri']) and strlen($data['uri'])){
				$uri .= '/'.$data['uri'].'.html';
			}
			$child[] = array(
				'label' => $data['topic'],
				'uri' => $uri,
			);
		}
	}
	return $child;
}

function sitemap_render($sitemap){
	$children = array();
	foreach($sitemap as $key => $value){
		if(isset($value['children']) and is_array($value['children'])){
			$children[$key] = sitemap_render($value['children']);
		}
	}
	$data = array(
		'sitemap' => $sitemap,
		'children' => $children,
	);
	return render($data, 'sitemap.tpl', 'sitemap');
}
?>
