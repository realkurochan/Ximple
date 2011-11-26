<?php
class RoleManager{
	private $reference;
	private $name;
	private $description;
	
	function RoleManager($reference){
		$this->reference = $reference;
	}
	
	public function setName($name){
		$this->name = $name;
	}
	
	public function getRole(){
		return $this->reference;
	}
	
	public function setDescription($description){
		$this->description = $description;
	}
	
	public function submit(){
		module_add_role($this->reference, $this->name, $this->description);
	}
	
	public function drop(){
		module_drop_role($this->reference);
	}
}
?>
