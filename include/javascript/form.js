var captchaLock = false;
var initedJQuery = false;
var imgCorrect = '<img class="warning_icon" src="'+rootURI+'files/icon/correct.png" />';
var imgWrong = '<img class="warning_icon" src="'+rootURI+'files/icon/stop.png" />';
var formLock = false;
var formChange = false;

if(isForm){
	addReady(function(){
		$('input, textarea').keypress(function(){
			formChange = true;
		});
		initForm($('a'), function(form){ form.click(function(){
			if($(this).attr('href').substring(0, 1) != '#' &&  $(this).attr('href').length){
				if(formChange){
					return confirm(changeLocationMessage);
				}else{
					return true;
				}
			}
		})});
	});
}

function checkForm(notNull, label){
	if(formLock){
		alert(formNotCorrect);
		return false;
	}
	var ok = true;
	ok = ok && verifyCaptcha();
	if(!ok) return false;
	ok = ok && checkNotNull(notNull, label);
	$(document).trigger("submit");
	return ok;
}

function initForm(form, initFunction){
	form.each(function(){
		if($(this).attr("rel") != "initialized"){
			initFunction($(this));
		}
		$(this).attr("rel", "initialized");
	});
}

function checkNotNull(notNull, label){
	var ok = true;
	var dataIn = null;
	try {
		for(var i in notNull){
			dataIn = $('#'+notNull[i]);
			if(dataIn.val().length == 0){
				ok=false;
				dataIn.css({'border':'2px solid red'});
				alert(pleaseFill+label[dataIn.attr('name')]);
				break;
			}
		}
	}
	catch (e){
	}
	return ok;
}

function verifyCaptcha(){
	var ok = true;
	try{
		if(captchaLock == true && ok == true){
			ok = false;
			alert(captchaMessage);
		}
	}
	catch (e){
	}
	return ok;
}
