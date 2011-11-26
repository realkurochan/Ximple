var translateForm;

function selectTranslateLanguage(language, moduleName, moduleID){
	if($('#translate_dialog').length == 0){
		$('body').append('<div id="translate_dialog"></div>');
	}
	var translate = $('#translate_dialog');
	translate.css({
		'text-align':'left'
	});
	var uri = sectionURI+moduleName+'_translate_form';
	uri += '/module_id/'+moduleID;
	uri += '/language/'+language;
	
	translate.dialog({
		bgiframe : true,
		autoOpen : false,
		modal : true,
		width : 520,
	});
	translate.dialog('open');
	translate.html('<div style="text-align:center;padding:50px;"><img src="'+rootURI+'files/icon/loader.gif"/></div>');
	$.get(uri, function(data){
		translate.html(data);
	}, 'html');
	return false;
}

function submitTranslateForm(form, notNull, label){
	if(checkForm(notNull, label)){
		$(form).ajaxSubmit({success : function(){
			$('#translate_dialog').dialog('close');
		}});
	}
	return false;
}
