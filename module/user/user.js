addReady(function(){
	$('#user_login_name').keyup(function(){
		formLock = true;
		if($(this).val().length < 4){
			$('#info_user_login_name').css({color:'red'});
			$('#info_user_login_name').html(imgWrong + nameTooShort);
		}else if($(this).attr('rel') == 'install'){
			$('#info_user_login_name').html(imgCorrect);
			formLock = false;
		}else{
			var uri = '';
			if(insertUser) uri = 'userinfo_check_user_exists/login_name/';
			else uri = 'userinfo_check_user_exists/uid/'+uid+'/login_name/';
			$.get(sectionURI+uri+$(this).val(), function(data){
				if(data != 'not exist'){
					$('#info_user_login_name').html(imgWrong + nameExists);
				}else{
					$('#info_user_login_name').html(imgCorrect);
					formLock = false;
				}
			}, 'html');
		}
	});
	
	$('#user_password').keyup(function(){
		var len = $(this).val().length;
		
		var tooShort = (len > 0 && len < 8) || (len <= 0 && insertUser);
		if(tooShort){
			$('#info_user_password').css({color:'red'});
			$('#info_user_password').html(imgWrong + passwdTooShort);
			formLock = true;
		}else{
			$('#info_user_password').html(imgCorrect);
			formLock = false;
		}
		
		if(!tooShort && len){
			checkPasswdStrength();
			checkPasswdConfirm();
		}
		
	});
	$('#user_confirm_password').keyup(checkPasswdConfirm);
});

$(document).bind("submit", function(){
	/*
	if($('#user_password').val().length == 0 && !insertUser){
		alert(passwdNotChange);
	}
	*/
	if($('#user_password').val() == $('#user_confirm_password').val()){
		var passwd = $('#user_password').val();
		if(passwd.length){
			$('#user_password').val(hex_sha1(passwd));
			$('#user_confirm_password').val('');
		}
		$('#user_login_name_hash').val($('#user_login_name').val());
	}
});

function checkPasswdStrength(){
	var len = $('#user_password').val().length;
	var passwd = $('#user_password').val();
	var strength = 0;
	var matchNumber = passwd.match(/\d+/);
	var matchBigChar = passwd.match(/[A-Z]+/);
	var matchSmallChar = passwd.match(/[a-z]+/);
	var matchSpecial = passwd.match(/\W/);
	if(matchNumber && matchBigChar && matchSmallChar && matchSpecial){
		if(len >= 12){
			$('#info_user_password').css({color:'#490'});
			$('#info_user_password').append(passwdVeryStrong);
		}else{
			$('#info_user_password').css({color:'#25a'});
			$('#info_user_password').append(passwdStrong);
		}
	}else if(matchNumber && matchBigChar && matchSmallChar){	
		$('#info_user_password').css({color:'#a80'});
		$('#info_user_password').append(passwdMedium);
	}else{
		$('#info_user_password').css({color:'#d40'});
		$('#info_user_password').append(passwdWeak);
	}
}

function checkPasswdConfirm(){
	if($('#user_password').val() != $('#user_confirm_password').val()){
		$('#info_user_confirm_password').css({color:'red'});
		$('#info_user_confirm_password').html(imgWrong + passwdMismatch);
		formLock = true;
	}else{
		$('#info_user_confirm_password').html(imgCorrect);
		formLock = false;
	}
}
