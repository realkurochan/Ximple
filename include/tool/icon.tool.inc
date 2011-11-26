<?php
/// @file
/// This file provides functions for get icon for files in file management system
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Icons of files sorted by type.
/// @return Array of file icons.
function icon(){
	$icons['.dir'] = 'folder.png';
	$icons['jpeg'] = 'image.png';
	$icons['jpg'] = 'image.png';
	$icons['png'] = 'image.png';
	$icons['gif'] = 'image.png';
	$icons['txt'] = 'text.png';
	$icons['doc'] = 'word.png';
	$icons['odf'] = 'document.png';
	$icons['odt'] = 'document.png';
	$icons['sxw'] = 'document.png';
	$icons['odp'] = 'present.png';
	$icons['sxi'] = 'present.png';
	$icons['ppt'] = 'present.png';
	$icons['ods'] = 'spread.png';
	$icons['xls'] = 'spread.png';
	$icons['sxc'] = 'spread.png';
	$icons['html'] = 'html.png';
	$icons['xhtml'] = 'html.png';
	$icons['htm'] = 'html.png';
	$icons['php'] = 'text.png';
	$icons['c'] = 'text.png';
	$icons['cpp'] = 'text.png';
	$icons['py'] = 'text.png';
	$icons['for'] = 'text.png';
	$icons['deb'] = 'deb.png';
	$icons['rpm'] = 'rpm.png';
	$icons['tar'] = 'tar.png';
	$icons['zip'] = 'tar.png';
	$icons['bz'] = 'tar.png';
	$icons['bz2'] = 'tar.png';
	$icons['gz'] = 'tar.png';
	$icons['tgz'] = 'tar.png';
	$icons['rar'] = 'tar.png';
	$icons['pdf'] = 'pdf.png';
	$icons['mp3'] = 'sound.png';
	$icons['mp4'] = 'vedio.png';
	$icons['mov'] = 'vedio.png';
	$icons['ogv'] = 'vedio.png';
	$icons['flv'] = 'vedio.png';
	$icons['unknown'] = 'unknown.png';
	return $icons;
}

/// Render icon for given file name.
/// @param $file_name : file name to render icon
function icon_render($file_name){
	$splitted = split('\.', basename($file_name));
	$type = $splitted[count($splitted) - 1];
	$icons = icon();
	if(isset($icons[$type])) $icon = $icons[$type];
	else $icon = 'unknown.png';
	return '<img src="'.ROOT_URI.'files/icon/'.$icon.'" border="0"/>';
}
?>
