$(document).bind("ready", function(){
	$('#customuri_mapped_URI').keyup(function(){
		var uri = sectionURI + 'customuri_check_mapped_exist/mapped_URI/'+$(this).val();
		var exist = false;
		if($(this).val().length > 7){
			if(customURIUpdate){
				uri += '/URI_ID/'+customURIID;
			}
			$.get(uri, function(data){
				if(data != 'not exist'){
					exist = true;
					formLock = true;
					$('#info_customuri_mapped_URI').css({color:'red'});
					$('#info_customuri_mapped_URI').html(imgWrong + customURIExists);
				}
			});
			if(!exist){
				$('#info_customuri_mapped_URI').html(imgCorrect);
				formLock = false;
			}else{
				formLock = true;
			}
		}else{
			formLock = true;
			$('#info_customuri_mapped_URI').css({color:'red'});
			$('#info_customuri_mapped_URI').html(imgWrong + customURITooShort);
		}
	});
});
