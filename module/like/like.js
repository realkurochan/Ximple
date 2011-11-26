function like(module, moduleID, owner){
	var uri = sectionURI+'like/module/'+module+'/module_id/'+moduleID+'/owner/'+owner;
	$.get(uri, function(data){
		var like = $('#like_'+module+'_'+moduleID);
		var show = $('#like_'+module+'_'+moduleID+'_show');
		var parent = like.parent();
		like.remove();
		show.remove();
		parent.append(data);
	});
	return false;
}

function unlike(module, moduleID, owner){
	var uri = sectionURI+'like_unlike/module/'+module+'/module_id/'+moduleID+'/owner/'+owner;
	$.get(uri, function(data){
		var like = $('#like_'+module+'_'+moduleID);
		var show = $('#like_'+module+'_'+moduleID+'_show');
		var parent = like.parent();
		like.remove();
		show.remove();
		parent.append(data);
	});
	return false;
}

function showLikePeople(module, moduleID, owner){
	var like = $('#like_'+module+'_'+moduleID+'_show');
	var likeDialog = $('#like_dialog');
	var uri = sectionURI+'like_users/module/'+module+'/module_id/'+moduleID;
	if(likeDialog.length == 0){
		$('body').append('<div id="like_dialog" class=""><img src="'+rootURI+'files/icon/load.gif"></div>');
		likeDialog = $('#like_dialog');
	}
	likeDialog.show();
	$.get(uri, function(data){
		var pos = like.offset();
		likeDialog.html(data);
		likeDialog.css({
			top : pos.top - likeDialog.height() - like.height() - 15,
			left : pos.left - likeDialog.width() + like.width(),
		});
	});
	return false;
}

function hideLikePeople(){
	$('#like_dialog').hide();
	return false;
}
