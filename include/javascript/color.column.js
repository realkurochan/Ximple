addReady(function(){
	initForm($('.color_picker'), function(form){
		formID = form.attr('id');
		$("#picker_"+formID).farbtastic("#"+formID);
		$("#picker_"+formID).dialog({
			bgiframe : true,
			autoOpen : false,
			modal : true,
			width : 220,
			height : 250,
			title : selectColorLabel
		});
		$("#"+formID).click(function(){
			$("#picker_"+form.attr('id')).dialog("open");
		});
	});
});
