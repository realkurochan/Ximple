function pollEmbedForm(){
	var embedForm = $('#poll_embed_form');
	var uri = sectionURI+'poll_embed_form/module/'+moduleName+'/module_id/'+modeID+'/is_embed/true';
	embedForm.dialog({
		bgiframe : true,
		autoOpen : false,
		height: 550,
		width:400,
		modal: true
	});
	embedForm.dialog('open');
	$.get(uri, function(data){
		embedForm.html(data);
	});
	return false;
}

function submitEmbedPollForm(form, notNull, label){
	if(checkForm(notNull, label)){
		$(form).ajaxSubmit({success : function(){
			$('#poll_embed_form').dialog('close');
		}});
	}
	return false;
}
