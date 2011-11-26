var currentPath = null;
var currentLoaderID = null;
var currentSize = null;

function showImageLoader(loaderID, path, size){
	currentLoaderID = loaderID;
	currentPath = path;
	currentSize = size;
	var loader = $('#loader_'+loaderID);
	loader.dialog({
		bgiframe: true,
		autoOpen: false,
		height: 560,
		width:520,
		modal: true,
		title: 'Image Loader'
	});
	var uri = sectionURICommon+'?mode=imageloader_simple&dir='+path+'&loader_id='+loaderID+'&size='+size;
	$.get(uri, function(data){
		loader.html(data);
	}, 'html');
	loader.dialog('open');
}

function submitImageLoader(){
	$('#image_upload_form').ajaxSubmit({success : refreshImageLoader});
	return false;
}

function refreshImageLoader(){
	var loader = $('#loader_'+currentLoaderID);
	var uri = sectionURICommon+'?mode=imageloader_simple&dir='+currentPath+'&loader_id='+currentLoaderID+'&size='+currentSize;
	$.get(uri, function(data){
		loader.html(data);
		$('#loader_info').html(uploadSuccess);
		loader.dialog('open');
	}, 'html');
}

function loaderChangeImage(loaderID, src, rel){
	$('#input_'+loaderID).val(rel);
	$('#image_'+loaderID).attr('src', src);
	$('#loader_'+loaderID).dialog('close');
}
