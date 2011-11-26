<?php
require_once('freetag.tool.inc');
require_once(INCLUDE_PATH.'class/column/AutoCompleteColumn.class.inc');

class FreeTagColumn extends AutoCompleteColumn{
	private $rawData;
	private $contentID;
	private $moduleName;
	
	function FreeTagColumn($moduleName){
		AutoCompleteColumn::AutoCompleteColumn(tt('tags'), 'freetag_get_ajax');
		$this->setType('shorttext');
		$this->isReachable = true;
		$this->help = tt('Use comma for multiple tags.');
		$this->moduleName = $moduleName;
	}
	
	public function triggerSelect(){
		$this->rawData = $this->data;
		$this->data = input_split_comma($this->data);
	}
	
	public function triggerPostInsert(){
		$this->vadiladeTag();
	}
	
	public function triggerPostUpdate(){
		$this->vadiladeTag();
	}
	
	public function triggerGetID($id){
		$this->contentID = $id;
	}
	
	public function triggerPostDBInsert(){
		freetag_update($this->data, $this->moduleName, $this->contentID);
	}
	
	public function triggerPostDBUpdate(){
		freetag_update($this->data, $this->moduleName, $this->contentID);
	}
	
	public function triggerPostDBDrop(){
		db_drop_many('freetag', "mode='".$this->moduleName."' AND content_id=".$this->contentID);
	}
	
	private function vadiladeTag(){
		$tagArray = input_split_comma($this->data);
		if(count($tagArray)) $this->data = join(', ', $tagArray).', ';
		else $this->data = '';
	}
}
?>
