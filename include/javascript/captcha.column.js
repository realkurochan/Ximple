function changeCaptcha(captchaKey, captchaID){
	captchaLock = true;
	generateTime = generateTime+10;
	var img = document.getElementById("captcha_img_"+captchaKey);
	img.src = sectionURI+'captcha/gentime/'+(generateTime);
	$("#"+captchaID).attr('value', '');
	$("#captcha_info_"+captchaKey).html('');
}

function checkCaptcha(captchaKey, captchaID){
	var uri = sectionURI+'captcha_check/value/';
	uri += ($('#'+captchaID).val());
	uri += '/gentime/'+generateTime;
	$.get(uri, function(data){
		if(data.substring(0,6) == "<code>"){
			$('#captcha_info_'+captchaKey).html(imgCorrect + captchaCorrect);
			$('#captcha_info_'+captchaKey).attr('color', 'green');
			$('#captcha_code_'+captchaKey).attr('value', data);
			$('#captcha_gentime_'+captchaKey).attr('value', generateTime);
			captchaLock = false;
		}else{
			$('#captcha_info_'+captchaKey).html(imgWrong + captchaWrong);
			$('#captcha_info_'+captchaKey).attr('color', 'red');
			captchaLock = true;
		}
	}, 'html')
}

