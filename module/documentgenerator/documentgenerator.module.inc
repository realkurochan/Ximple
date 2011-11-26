<?php
execute_add_function('documentgenerator');

function documentgenerator(){
	document_set_path(tt('Document Generator'));
	$announcedocument = db_select('documentgenerator_announcedocument' , array('id', 'subject'), 'WHERE owner='.USER_ID);
	$commanddocument = db_select('documentgenerator_commanddocument' , array('id', 'subject'), 'WHERE owner='.USER_ID);
	$externaldocument = db_select('documentgenerator_externaldocument' , array('id', 'subject'), 'WHERE owner='.USER_ID);
	$internaldocument = db_select('documentgenerator_internaldocument' , array('id', 'subject'), 'WHERE owner='.USER_ID);
	$render_array = array(
		'announcedocument' => $announcedocument,
		'commanddocument' => $commanddocument,
		'externaldocument' => $externaldocument,
		'internaldocument' => $internaldocument,
	);
	
	return render($render_array, 'documentgenerator.tpl', 'documentgenerator');
}
?>
