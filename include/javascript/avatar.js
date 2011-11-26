addReady(function(){
	initForm($('.avatar_container'), function(form){
		form.mouseover(function(){
			$(this).find('.avatar_icon').show();
		});
		
		form.mouseout(function(){
			$(this).find('.avatar_icon').hide();
		});
	});
});
