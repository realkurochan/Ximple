addReady(function(){
	initForm($(".calendar_form"), function(form){
		form.datepicker({ 
			dateFormat: "d MM yy"
		});
	});
});
