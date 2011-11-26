addReady(function(){
	initForm($(".timeinterval_day_form"), function(form){
		form.spinbox({ 
			min : 0,
			step : 1
		});
	});
	
	initForm($(".timeinterval_hour_form"), function(form){
		form.spinbox({ 
			min : 0,
			max : 24,
			step : 1
		});
	});
	
	initForm($(".timeinterval_minute_form"), function(form){
		form.spinbox({ 
			min : 0,
			max : 60,
			step : 1
		});
	});
	
	initForm($(".timeinterval_second_form"), function(form){
		form.spinbox({ 
			min : 0,
			max : 60,
			step : 1
		});
	});
});

$(document).bind("submit", function(){
	$(".timeinterval_form").each(function(){
		if($(this).attr("rel") != "processed"){
			var formID = $(this).attr('id');
			var value = 0;
			var eachValue = 0;
			eachValue = $('#'+formID+'_day').val()*86400;
			if(!isNaN(eachValue)) value += eachValue;
			eachValue = $('#'+formID+'_hour').val()*3600;
			if(!isNaN(eachValue)) value += eachValue;
			eachValue = $('#'+formID+'_minute').val()*60;
			if(!isNaN(eachValue)) value += eachValue;
			eachValue = $('#'+formID+'_second').val()*1;
			if(!isNaN(eachValue)) value += eachValue;
			$(this).val(value);
		}
		$(this).attr("rel", "processed");
	});
});
