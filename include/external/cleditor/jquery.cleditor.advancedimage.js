imageDialog =
'<fieldset style="width:400px;height:200px;overflow:auto;">'+
'	<legend>Image</legend>'+
'	<p>Click on image to upload or chage image</p>'+
'	<a href="#" onclick="showCLEImageLoader(this);return false">'+
'		<img src="'+rootURI+'files/icon/loader.png" border="0">'+
'	</a>'+
'</fieldset>'+
'<fieldset style="width:400px;height:50px;">'+
'	<legend>Properties</legend>'+
'	<label>description</label>'+
'	<input class="image_description" style="width:50px;"/>'+
'	<label style="padding-left:10px;">margin</label>'+
'	<input class="image_margin" style="width:30px;"/>'+
'	<label style="padding-left:10px;">align</label>'+
'	<select class="image_align">'+
'		<option value="baseline">Baseline</option>'+
'		<option value="left">Left</option>'+
'		<option value="right">Right</option>'+
'		<option value="top">Top</option>'+
'		<option value="bottom">Bottom</option>'+
'		<option value="middle">Middle</option>'+
'	</select>'+
'</fieldset>'+
'<input class="code_submit" type="button" value=Submit>';

var currentImageParent = null;
var currentImagePopup = null;
var selectedImageSource = rootURI+'files/icon/loader.png';

$.cleditor.buttons.advancedimage = {
	name: "advancedimage",
	image: "image.png",
	title: "Advance Image",
	command: "inserthtml",
	popupName: "advancedimage",
	popupClass: "cleditorPrompt",
	popupContent: imageDialog,
	buttonClick: advanceImageClick
};

function advanceImageClick(e, data){
	var popup = $(data.popup);
	currentImagePopup = popup;
	popup.find(".code_submit").unbind("click").bind("click", function(e) {
		var html = '';
		var editor = data.editor;
		var description = popup.find(".image_description").val();
		var align = popup.find(".image_align").val();
		var margin = popup.find(".image_margin").val();
		if(description == null || description.length == 0) description = "Image";
		if(margin == null || margin.length == 0) margin = 0;
		popup.find(".image_description").val('');
		popup.find(".image_align").val('');
		popup.find(".image_margin").val('');
		currentImageParent.innerHTML = '<img src="'+rootURI+'files/icon/loader.png" border="0">';
		if(align == null || align.length == 0){
			html = '<img alt="'+description+'" style="margin:'+margin+'px;" src="'+selectedImageSource+'"/>';
		}else{
			html = '<img alt="'+description+'" align="'+align+'" style="margin:'+margin+'px;" src="'+selectedImageSource+'"/>';
		}
		editor.execCommand(data.command, html, null, data.button);
		editor.hidePopups();
		editor.focus();
	});
}

function showCLEImageLoader(imageParent){
	currentImageParent = imageParent;
	var loader = $("#cle_image_loader");
	var maxZ = Math.max.apply(null,$.map($('body > *'), function(e,n){
	   if($(e).css('position')=='absolute')
		    return parseInt($(e).css('z-index'))||1 ;
	   })
	);
	loader.dialog({
		bgiframe : true,
		autoOpen : false,
		height : 560,
		width :480,
		modal : true,
		title : 'Image Loader',
		zIndex : maxZ*2
	});
	var uri = sectionURICommon+'?mode=imageloader_cle';
	$.get(uri, function(data){
		loader.html(data);
	});
	loader.dialog('open');
}

function submitImageLoaderCLE(){
	$('#image_upload_form').ajaxSubmit({success : refreshImageLoaderCLE});
	blockHidePopups = true;
	return false;
}

function refreshImageLoaderCLE(){
	var loader = $("#cle_image_loader");
	var uri = sectionURICommon+'?mode=imageloader_cle';
	$.get(uri, function(data){
		loader.html(data);
		$('#loader_info').html(uploadSuccess);
		loader.dialog('open');
	});
}

function loaderChangeImageCLE(src){
	currentImageParent.innerHTML = '<img src="'+src+'" border="0" style="max-height:145px;"/>';
	$("#cle_image_loader").dialog('close');
	selectedImageSource = src;
	currentImagePopup.show();
	blockHidePopups = true;
}
