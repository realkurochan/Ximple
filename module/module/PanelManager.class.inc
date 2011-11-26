<?php
class PanelManager{
	private $name;
	private $control = array();
	private $admin = array();
	private $item = array();
	private $role = array();
	
	function PanelManager($name){
		$this->name = $name;
	}
	
	public function appendControl($control){
		$this->control[] = $control;
		$this->admin[] = $control->getAdmin();
	}
	
	public function append($mode, $label){
		$this->item[$mode] = $label;
	}
	
	public function appendRole($roleManager){
		$role = $roleManager->getRole();
		if($role != $this->admin) $this->role[] = $role;
	}
	
	public function submit(){
		foreach($this->control as $key => $control){
			$admin = $this->admin[$key];
			foreach($this->item as $mode => $label){
				module_add_panel($this->name, $admin, $mode, $label);
				$method = $control->getAction($mode);
				foreach($this->role as $role){
					if($control->hasAccessMethod($role, $method)){
						module_add_panel($this->name, $role, $mode, $label);
					}
				}
			}
		}
	}
	
	public function drop(){
		module_drop_panel($this->name);
	}
}
?>
