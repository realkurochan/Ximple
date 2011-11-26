<?php
class ResetPasswordModel extends Model{
	function ResetPasswordModel(){
		$this->password = $this->password(tt('new password'));
		$this->confirm_password = $this->password(tt('confirm password'));
	}
}
?>
