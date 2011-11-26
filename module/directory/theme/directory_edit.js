$(document).ready(function (){
	$('.directory_element_label').show();
});

var showLabel = true;

function toggleLabel(){
	if(showLabel){
		$('.directory_element_label').hide();
		$('.directory_element').css({border:'none'});
		showLabel = false;
	}else{
		$('.directory_element_label').show();
		$('.directory_element').css({border:'1px solid #490'});
		showLabel = true;
	}
}
