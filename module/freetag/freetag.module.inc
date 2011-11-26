<?php
execute_add_function('freetag_get_ajax');

function freetag_get_ajax(){
	if(!authority_is_guest()){
		$wildcard = addslashes((string) gg('wildcard'));
		if(strlen($wildcard)){
			$clause = "WHERE freetag LIKE '".$wildcard."%' GROUP BY freetag";
			$freetag = db_select_single_column('freetag', 'freetag', $clause, false);
			execute_set_ajax();
			return '<option>'.join('</option><option>', $freetag).'</option>';
		}
	}
}
?>
