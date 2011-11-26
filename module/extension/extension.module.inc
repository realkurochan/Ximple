<?php
execute_add_function('extension');
execute_add_function('extension_save');

if(!authority_group('module_admin')) error_denied();

function extension(){
	require_once(INCLUDE_PATH.'class/QuickFormRender.class.inc');
	require_once(INCLUDE_PATH.'class/FormMetaData.class.inc');
	require_once(INCLUDE_PATH.'class/Model.class.inc');
	
	document_set_path(tt('Mange Extension'));
	
	$form = new QuickFormRender('extension');
	$meta = new FormMetaData();
	$meta->mode = 'extension_save';
	$meta->title = tt('Mange Extension');
	$meta->description = tt('You can enable and sort Extensions in the Section with this form.');
	
	list($enabled_extension, $disabled_extension, $all_extension) = extension_get_all();
	
	foreach($all_extension as $reference => $enabled){
		if(!isset($enabled_extension[$reference])) $enabled = array();
		else $enabled = $enabled_extension[$reference];
		if(count($enabled)) $enabled = array_combine($enabled, $enabled);
		
		if(!isset($disabled_extension[$reference])) $disabled = array();
		else $disabled = $disabled_extension[$reference];
		if(count($disabled)) $disabled = array_combine($disabled, $disabled);
		
		$option = array_merge($enabled, $disabled);
		
		if(count($option)){
			$model = Model::checkbox($reference, $option);
			$model->name = $reference;
			$model->data = $enabled;
			$model->isSortable = true;
			$form->append($reference, $model);
		}
	}
	return $form->renderEntireForm($meta);
}

function extension_save(){
	execute_set_db();
	document_set_redirect('extension');
	list($enabled_extension, $disabled_extension, $all_extension) = extension_get_all();
	
	$enabled_extension = array();
	$disabled_extension = array();
	
	$post = pp();
	foreach($all_extension as $reference => $extension){
		if(!isset($post[$reference]) and count($extension)){
			$disabled_extension[$reference] = $extension;
		}
	}
	
	foreach($post as $reference => $extension){
		foreach($extension as $key => $name){
			if(in_array($name, $post[$reference])){
				$enabled_extension[$reference][] = $name;
			}else{
				$disabled_extension[$reference][] = $name;
			}
		}
	}
	config_set('extension', $enabled_extension);
	config_set('disabled_extension', $disabled_extension);
	config_save();
}

function extension_get_all(){
	$enabled_extension = cc('extension');
	if(!is_array($enabled_extension)) $enabled_extension = array();
	$disabled_extension = cc('disabled_extension');
	if(!is_array($disabled_extension)) $disabled_extension = array();
	$all_extension = array_merge($enabled_extension, $disabled_extension);
	return array($enabled_extension, $disabled_extension, $all_extension);
}
?>
