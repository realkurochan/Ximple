<?php
function directory_show_item_option(){
	return array(
		'index_page' => tt('index page'),
		'description' => tt('directory description'),
		'page_list' => tt('list of pages'),
		'page_description' => tt('description and icon of pages'),
		'subdirectory_list' => tt('list of sub-directories'),
		'subdirectory_description' => tt('description and icon of sub-directories'),
		'extension' => tt('extension'),
	);
}

function directory_default_configuration(){
	return array(
		'show_item' => array('index_page'),
		'extension_mode' => '',
		'page_list_label' => tt('List of pages in this directory'),
		'related_page_label' => tt('Another pages in this directory'),
		'page_render_item' => array('related', 'sibling'),
		'show_page' => array(),
		'directory_list_label' => tt('List of sub-directories in this directory'),
		'show_directory' => array(),
		'publishing_status' => array(2, 3),
		'publishing_status_default' => 3,
		'comment_write_label' => tt('Please, write us your opinion.'),
		'comment_permission' => array(0, 1, 2, 3),
		'comment_permission_default' => 3,
		'comment_page_length' => 10,
		'comment_show_avatar' => false,
	);
}

function directory_set_path($pid){
	include_once(INCLUDE_PATH.'tool/menu.tool.inc');
	$path = array();
	while($pid != 0){
		$parentData = db_select('directory', array('parent', 'name', 'uri'), 'WHERE id='.$pid, false);
		$parentData[0]['id'] = $pid;
		$parentData[0] = translate_content($parentData[0], 'directory', $pid);
		$path[] = $parentData[0];
		if(strlen($parentData[0]['uri'])) $uri = '/'.$parentData[0]['uri'].'.html';
		else $uri = '';
		menu_activate('directory/'.$pid.$uri);
		$pid = $parentData[0]['parent'];
	}
	$path = array_reverse($path);
	foreach($path as $key => $value){
		if(strlen($value['uri'])) $uri = '/'.$value['uri'].'.html';
		else $uri = '';
		document_set_path($value['name'], 'directory/'.$value['id'].$uri);
	}
}

function directory_get_page_category_id(){
	$clause = "WHERE mode='page' AND section=".SECTION;
	$category = db_select_single_column('category', 'id', $clause, false);
	return $category[0];
}

function directory_create_root($category){
	$exist = db_select_single_column('directory', 'id', 'WHERE section='.SECTION.' LIMIT 1');
	if(count($exist) == 0){
		$data = array(
			'name' => tt('ROOT Directory'),
			'permission' => serialize(array()),
			'configuration' => serialize(directory_default_configuration()),
			'parent' => 0,
			'category' => $category,
			'owner' => USER_ID,
			'create_time' => NOW,
			'section' => SECTION,
		);
		return db_insert('directory', $data);
	}else{
		return null;
	}
}

function directory_create_index_page($parent, $category, $directory_name, $introduction=null, $icon=null){
	$configuration = directory_default_configuration();
	if($introduction == null){
		$introduction = tt('This Page is automatically generated for Directory. You can edit it to make it more usable.');
	}
	$data = array(
		'writer' => USER_ID,
		'parent' => $parent,
		'topic' => $directory_name,
		'introduction' => $introduction,
		'icon' => $icon,
		'category' => $category,
		'freetag' => '',
		'write_time' => NOW,
		'publish_time' => NOW,
		'publishing_status' => $configuration['publishing_status_default'],
		'comment_permission' => $configuration['comment_permission_default'],
		'section' => SECTION,
	);
	return db_insert('page', $data);
}


?>
