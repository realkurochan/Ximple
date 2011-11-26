<?php
class ForgetPasswordModel extends Model{
	function ForgetPasswordModel(){
		$this->email = $this->email(tt('email'));
	}
}
?>
