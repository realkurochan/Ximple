addReady(function(){
	initForm($('.auto_complete'), function(form){
		form.keypress(function(){
			var words = $(this).val().split(',');
			var lastWord = jQuery.trim(words[words.length-1]);
			if(lastWord.length){
				var option = $('#complete_option');
				var pos = $(this).offset();
				option.css({
					top: pos.top + $(this).height() + 4 + autoCompleteTop,
					left: pos.left + autoCompleteLeft,
					width: $(this).width() + 2,
				});
				$('#complete_option option').remove();
				option.attr('rel', $(this).attr('id'));
				var uri = sectionURI+$(this).attr('alt')+'/wildcard/'+lastWord;
				$.get(uri, function(data){
					if(data.length){
						option.append(data);
						option.show();
					}
				}, 'html');
			}
		});
	});
	
	initForm($('#complete_option'), function(form){
		form.click(function(){
			var input = $('#'+$(this).attr('rel'));
			var value = '';
			var words = input.val().split(',');
			for(var i=0; i< words.length -1 ;i++){
				value += words[i]+', '
			}
			value += $(this).val();
			input.val(value);
			$(this).hide();
		});
	});
	
	$('body').click(function(){
		$('#complete_option').hide();
	});
});
