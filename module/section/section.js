$(document).bind("ready", function(){
	$('#section_path').keyup(function(){
		formLock = true;
		var reg = /^[a-zA-Z0-9_\-]+$/;
		
		if($(this).val().length < 4){
			$('#info_section_path').css({color:'red'});
			$('#info_section_path').html(imgWrong + pathTooShort);
		}else if(!$(this).val().match(reg)){
			$('#info_section_path').css({color:'red'});
			$('#info_section_path').html(imgWrong + pathNotConformed);
		}else{
			var uri = sectionURI+'sectioninfo_check_path_exists/';
			if(sectionInsert) uri += 'path/';
			else uri += 'sid/'+sectionUpdateID+'/path/';
			$.get(uri+$(this).val(), function(data){
				if(data != 'not exist'){
					$('#info_section_path').html(imgWrong + pathExists);
				}else{
					$('#info_section_path').html(imgCorrect);
					formLock = false;
				}
			});
		}
	});

	$('#section_name').keyup(function(){
		formLock = true;
		if($(this).val().length < 4){
			$('#info_section_name').css({color:'red'});
			$('#info_section_name').html(imgWrong + nameTooShort);
		}else{
			var uri = sectionURI+'sectioninfo_check_name_exists/';
			if(sectionInsert) uri += 'name/';
			else uri += 'sid/'+sectionUpdateID+'/name/';
			$.get(uri+$(this).val(), function(data){
				if(data != 'not exist'){
					$('#info_section_name').html(imgWrong + nameExists);
				}else{
					$('#info_section_name').html(imgCorrect);
					formLock = false;
				}
			});
		}
	});
	
	$('#section_uri').keyup(function(){
		var uriMatch = /http:\/\/[A-Za-z0-9\.-]{3,}\.[A-Za-z]{3}\//;
		if(uriMatch.test($(this).val())){
			$('#info_section_uri').html(imgCorrect);
			formLock = false;
		}else{
			$('#info_section_uri').css({'color':'red'});
			$('#info_section_uri').html(imgWrong + uriNotMatch);
			formLock = true;
		}
	});
});
