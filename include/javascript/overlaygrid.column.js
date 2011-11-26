var currentOverlayDialog;
var currentFormID;
var currentModuleName;
var insertedOverlayDataID = new Array();

function overlayWrite(formID, moduleName){
	var uri = sectionURI+moduleName+'_overlay_write';
	var overlayDialog = overlayOpenDialog(formID, moduleName);
	
	if(overlayParentID) uri += '/parent/'+overlayParentID;
	overlayDialog.css({
		'text-align' : 'left',
	});
	currentOverlayDialog = overlayDialog;
	currentFormID = formID;
	currentModuleName = moduleName;
	$.get(uri, function(data){
		overlayDialog.html(data);
		for(i in readyFunction){
			readyFunction[i]();
		}
	}, 'html');
	return false;
}

function overlayInsertForm(form, notNull, label){
	if(checkForm(notNull, label)){
		$(form).ajaxSubmit({success : function(data){
			data = data.replace(/(<([^>]+)>)/ig,"");
			data = data.replace(/\s*/ig,"").split('\/');
			var dataID = data[2];
			var inputName = 'overlay_grid_'+data[1];
			$('#form_'+currentFormID).append('<input type="hidden" name="'+inputName+'[]" value="'+dataID+'" />');
			if(insertedOverlayDataID[data[0]] == undefined){
				insertedOverlayDataID[data[0]] = new Array();
			}
			insertedOverlayDataID[data[0]].push(data[2]);
			currentOverlayDialog.dialog('close');
			overlayRefresh(currentFormID, currentModuleName);
		}});
		formChange = true;
	}
	for(i in readyFunction){
		readyFunction[i]();
	}
	return false;
}

function overlayEdit(formID, moduleName, moduleID){
	var overlayDialog = overlayOpenDialog(formID, moduleName);
	var uri = sectionURI+moduleName+'_overlay_edit/'+moduleID+'';
	overlayDialog.css({
		'text-align' : 'left',
	});
	currentFormID = formID;
	currentModuleName = moduleName;
	currentOverlayDialog = overlayDialog;
	$.get(uri, function(data){
		overlayDialog.html(data);
		for(i in readyFunction){
			readyFunction[i]();
		}
	}, 'html');
	return false;
}

function overlayUpdateForm(form, notNull, label){
	if(checkForm(notNull, label)){
		$(form).ajaxSubmit({success : function(data){
			currentOverlayDialog.dialog('close');
			overlayRefresh(currentFormID, currentModuleName);
		}});
		formChange = true;
	}
	return false;
}


function overlayDrop(formID, moduleName, moduleID){
	var uri = sectionURI+moduleName+'_drop'+'/'+moduleID+'/confirm/true';
	$.get(uri, function(data){
		overlayRefresh(formID, moduleName);
	}, 'html');
	return false;
}

function overlayRefresh(formID, moduleName){
	var overlayTable = $('#overlay_'+formID+'_'+moduleName+'_table');
	var uri = sectionURI+moduleName+'_overlay_refresh'+'/form_id/'+formID+'/';
	var idArray = insertedOverlayDataID[moduleName];
	
	if(overlayParentID) uri += 'parent/'+overlayParentID+'/';
	if(idArray != undefined) uri += 'refresh_id/'+idArray.join(':');
	
	overlayTable.html('<img src="'+rootURI+'files/icon/loader.gif"/></div>');
	$.get(uri, function(data){
		overlayTable.html(data);
	}, 'html');
}

function overlayOpenDialog(formID, moduleName){
	var overlayDialog = $('#overlay_'+formID+'_'+moduleName+'_dialog');
	overlayDialog.dialog({
		bgiframe : true,
		autoOpen : false,
		modal : true,
		width : 520,
	});
	overlayDialog.dialog('open');
	return overlayDialog;
}
