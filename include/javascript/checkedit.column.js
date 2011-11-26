addReady(function(){
	initForm($('.checkedit'), function(form){
		form.change(function(){
			var obj = $('#'+$(this).attr('rel'));
			obj.attr('disabled', !obj.attr('disabled'));
		});
	});
});
