<?php
require_once('AutoCompleteColumn.class.inc');

class UserColumn extends AutoCompleteColumn{
	function UserColumn($label, $help=null){
		AutoCompleteColumn::AutoCompleteColumn($label, 'userinfo_get_ajax_name');
		if(empty($help)){
			$this->help = $help.tt('Type beginning letters of the user name. ');
			$this->help.= tt('The full name will be automatically completed. ');
			$this->help.= tt('Multiple names are possible using comma.');
		}
		$this->isNotNull = true;
	}
	
	public function triggerPostInsert(){
		$this->getUID();
	}
	
	public function triggerPostUpdate(){
		$this->getUID();
	}
	
	private function getUID(){
		$this->data = input_split_comma($this->data);
		$this->data = userinfo_name_to_id($this->data);
	}
}
?>
