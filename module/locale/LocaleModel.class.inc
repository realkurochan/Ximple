<?php
class LocaleModel extends Model{
	function LocaleModel(){
		$this->directory = $this->title(tt('directory'));
		$this->directory->help = tt('Related to ROOT_PATH');
		$this->language = $this->select(tt('lanaguage'), translate_get_interface_language());
		$this->locale = $this->plain('result');
	}
}
?>
