<?php
class ResolverModel extends Model{
	public $moduleName = 'develcard';
	public $tableName = 'develcard_resolver';
	public $dropMode = 'develcard_drop_resolver';
	
	function ResolverModel($develcardID=null){
		$option = userinfo_get_login_name_by_role('develcard_resolver');
		$this->resolver = $this->select(tt('user'), $option);
		$this->develcard = $this->none($develcardID, 'int');
		$this->develcard->isGridForm = false;
		$this->develcard->isParent = true;
		$this->status = $this->none(0, 'int');
		$this->point = $this->none(0, 'int');
		$this->end_date = $this->now();
		$this->end_date->defaultData = time() + 60*60*24*7;
		$this->end_date->isGridForm = false;
		$this->work_load = $this->spin(tt('work load'), 1, 2500);
	}
	
	public function triggerSetParent($parentID){
		$this->develcard->data = $parentID;
	}
}
?>
