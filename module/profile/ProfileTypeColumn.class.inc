<?php
require_once(INCLUDE_PATH.'class/column/RadioColumn.class.inc');

class ProfileTypeColumn extends RadioColumn{
	function ProfileTypeColumn(){
		RadioColumn::RadioColumn(tt('profile type'), array());
		$this->setType('int');
		$this->data = 1;
	}
	
	public function triggerInsertForm(){
		$this->getSelect();
	}
	
	public function triggerUpdateForm(){
		$this->getSelect();
	}
	
	public function triggerSelect(){
		$data = db_select_single_column('profiletype', 'name', 'WHERE id='.(int)$this->data);
		$this->data = $data[$this->data];
	}
	
	private function getSelect(){
		$column = array('id', 'name', 'allowed_group');
		$allType = db_select('profiletype', $column, 'WHERE section='.SECTION);
		$this->option = array();
		foreach($allType as $key => $value){
			$groups = unserialize($value['allowed_group']);
			if(authority_check_users_groups(array(), $groups)){
				$this->option[$value['id']] = $value['name'];
				if(!isset($this->defaultData)){
					$this->defaultData = $value['id'];
				}
			}
		}
	}
	
}
?>
