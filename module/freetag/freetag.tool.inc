<?php
function freetag_update($raw_tag, $mode, $id){
	db_drop_many('freetag', "mode='".$mode."' AND content_id=".$id);
	$data = array(
		'mode' => $mode,
		'content_id' => $id,
	);
	$tags = input_split_comma($raw_tag);
	foreach($tags as $key => $value){
		if(strlen($value)){
			$data['freetag'] = $value;
			db_insert('freetag', $data);
		}
	}
}

function freetag_get($mode){
	$clause = "WHERE mode='$mode' GROUP BY freetag";
	return db_select('freetag', array('freetag', 'count(freetag) as number'), $clause);
}
?>
