<?php
/// @file
/// This file provides Authority class, which checks access permission
/// for called ximple_mode (see terminology).

/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

/// Authority class checks access permission for called ximple_mode.
/// Authority Policy
///		- Only registered methods are allowed to be called.
///		- The called ximple_mode is mapped to a registered method.
///		- Registered methods can be mapped to many ximple_modes.
///		- Admin-Role (see terminology) can access all methods of the given class
///		  (use setAdmin() method).
///		- Owner (see terminology) can access all methods of the given class with
///		  the condition that the logged-in user owns the accessed database record
/// 	  (use setOwner() method)..
///		- The residual methods can be accessed applying Role-System (see terminology).
///		- Only Users (see terminology) with the authorized Role for the called method
///		  can access the called method (use addAccessMethod() method).
abstract class Authority{

	/// Associated array of the called method to the allowed Roles.
	private $accesMethod = array();
	
	/// Associated array of mapped ximple_mode to the called method.
	private $action = array();
	
	/// Name of Admin-Role.
	private $adminGroup = 'admin';
	
	/// Uer-ID of the Owner.
	private $owner;
	
	/// Get method name from the given ximple_mode.
	/// @param $mode : name of given ximple_mode.
	/// @return Name of mapped method.
	public function getAction($mode){
		if(isset($this->action[$mode])){
			return $this->action[$mode];
		}else{
			return null;
		}
	}
	
	/// Check the authorization of User for called method.
	/// @param $methodName : name of the called method.
	/// @return TRUE, if User can access the called method.
	public function isCallable($methodName){
		if(authority_user($this->owner)){
			return true;
		}
		
		if(isset($this->accesMethod[$methodName])){
			$gidArray = $this->accesMethod[$methodName];
			if(in_array('guest', $gidArray)) return true;
			if(in_array('user', $gidArray) and !authority_is_guest()) return true;
			foreach($gidArray as $gid){
				if(authority_group($gid)) {
					return true;
				}
			}
		}
		
		if(!empty($this->adminGroup)){
			if(authority_group($this->adminGroup)) return true;
		}
		return false;
	}
	
	/// Prepare object before checking authorization.
	/// @param $methodName : name of the called method.
	public function prepareMethod($methodName){
	}
	
	/// Get Admin-Role of class.
	/// @return Admin-Role of class.
	public function getAdmin(){
		return $this->adminGroup;
	}
	
	public function hasAccessMethod($gid, $methodName){
		if(isset($this->accesMethod[$methodName])){
			if(in_array($gid, $this->accesMethod[$methodName])){
				return true;
			}
		}
		return false;
	}
	
	/// Set owner of object.
	/// @param $owner : User-ID of Owner.
	protected function setOwner($owner){
		$this->owner = $owner;
	}
	
	/// Set Admin-Role of class.
	/// @param $gid : name of Admin-Role.
	protected function setAdmin($gid){
		$this->adminGroup = $gid;
	}
	
	/// Map Role with method name to enable access permission.
	/// @param $gid : name of Role.
	/// @param $methodName : name of the mapped method.
	protected function addAccessMethod($gid, $methodName){
		$this->accesMethod[$methodName][] = $gid;
	}
	
	
	/// Drop Role from access permission.
	/// @param $methodName : name of the mapped method.
	protected function dropAccessMethod($methodName){
		unset($this->accesMethod[$methodName]);
	}
	
	/// Map ximple_mode with method name to enable access permission.
	/// @param $mode : name of ximple_mode.
	/// @param $methodName : name of the mapped method.
	protected function addAction($mode, $methodName){
		$this->action[$mode] = $methodName;
	}
	
	/// Drop ximple_mode from access permission.
	/// @param $mode : name of ximple_mode.
	protected function dropAction($mode){
		unset($this->action[$mode]);
	}
	
	/// Unset all defined Actions.
	protected function unsetAction(){
		$this->action = array();
	}
}
?>
