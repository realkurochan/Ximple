addReady(function(){
	initForm($(".add_multi_files"), function(form){
		form.click(function(){
			var rel = $(this).attr("rel");
			var tab = $("#multi_files_"+rel);
			tab.append('<input name="'+rel+'[]" size="40" type="file" style="margin-bottom:10px;"/><br />');
		});
	});
});

function addFileInput(formID, formName){
	$('#'+formID+' ul').append('<li><input type="file" name="'+formName+'[]" size="25" ></li>');
	return false;
}
