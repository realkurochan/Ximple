addReady(function(){
	initForm($(".timestamp_date_form"), function(form){
		form.datepicker({ 
			dateFormat: "d MM yy"
		});
	});
	
	initForm($(".timestamp_hour_form"), function(form){
		form.spinbox({ 
			min : 0,
			max : 24,
			step : 1
		});
	});
	
	initForm($(".timestamp_minute_form"), function(form){
		form.spinbox({ 
			min : 0,
			max : 60,
			step : 1
		});
	});
});

$(document).bind("submit", function(){
	$(".timestamp_date_form").each(function(){
		if($(this).attr("rel") != "processed"){
			var formID = $(this).attr('id');
			var value = $(this).val();
			value += ' ';
			value += $('#'+formID+'_hour').val()+':';
			value += $('#'+formID+'_minute').val()+':00';
			$(this).val(value);
		}
		$(this).attr("rel", "processed");
	});
});
