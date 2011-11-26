addReady(function(){
	for(i in gridInitFunction){
		gridInitFunction[i]();
	}
});

function addGridRow(tableID){
	$('#'+tableID).append(gridRowArray[tableID]);
	$(document).trigger('ready');
	for(i in gridInitFunction){
		gridInitFunction[i]();
	}
}

function delGridRow(anchor, dropMode){
	$(anchor).parent().parent().remove();
	if(dropMode.length){
		$.get(sectionURI+dropMode, function(){}, 'html');
	}
}

