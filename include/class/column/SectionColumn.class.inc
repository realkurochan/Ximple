<?php
require_once('HiddenColumn.class.inc');
class SectionColumn extends HiddenColumn{
	function SectionColumn(){
		HiddenColumn::HiddenColumn(SECTION, 'int');
		$this->isNotNull = true;
	}
}
?>
