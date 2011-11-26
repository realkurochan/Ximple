var gridInitFunction = new Array();

var readyFunction = new Array();

function addReady(item){
	$(document).bind("ready", item);
	readyFunction.push(item);
}

function checkNotify(){
	if(!isGuest){
		$.get(sectionURI+'notify_check', function(data){
			if(data.length){
				$('#notify_check').remove();
				$('#notify_container').remove();
				$('body').append(data);
				$('#notify_container').dialog({
					bgiframe: true,
					autoOpen: false,
					width:450,
					modal: true,
					title: 'Notification',
					close : function (event, ui){
						$('#notify_check').remove();
						$('#notify_container').remove();
						$.get(sectionURI+'notify_mark_as_read', function(data){}, 'html');
					}
				});
			}
			if(checkNotifyInterval < 5000) checkNotifyInterval = 500;
			setTimeout("checkNotify()", checkNotifyInterval);
		}, 'html');
	}
}

setTimeout("checkNotify()", checkNotifyInterval);

function showNotify(){
	$('#notify_container').dialog('open');
}

function randomString() {
	var chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXTZabcdefghiklmnopqrstuvwxyz";
	var string_length = 32;
	var randomstring = "";
	for (var i=0; i<string_length; i++) {
		var rnum = Math.floor(Math.random() * chars.length);
		randomstring += chars.substring(rnum,rnum+1);
	}
	return randomstring;
}

function crypt(){
	var sessionKey;
	var cookies;
	var paraname;
	sessionKey = randomString();
	pass = document.getElementsByName("login_prepassword")[0].value;
	crypted = hex_sha1(pass);
	crypted = hex_sha1(crypted+sessionKey);
	document.getElementsByName("login_password")[0].value=crypted;
	document.getElementsByName("login_salt")[0].value=sessionKey;
	document.getElementsByName("login_prepassword")[0].value="";
}

function hideSub(subConf){
	$('#'+subConf).toggle('blind');
}

function togglePanel(panelID){
	$('.panel_body:visible').hide('blind');
	$('#'+panelID+':hidden').show('blind');
}

function showPlainCode(codeId){
	$('#code_'+codeId).hide();
	$('#plaincode_'+codeId).show();
}

function hidePlainCode(codeId){
	$('#code_'+codeId).show();
	$('#plaincode_'+codeId).hide();
}

function ximplePrint(link){
	window.open(link.href, 'print','toolbar=no,scrollbars=yes,titlebar=no,menubar=no,resizable=yes,width=800,height=600');
}

function isIE7(){
	return ($.browser.msie && (parseInt($.browser.version, 10) <= 7));
}
