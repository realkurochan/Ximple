<?php
execute_add_extended_class('documentgenerator', 'announcedocument', 'AnnounceDocument');
include_once('DocumentGenerator.class.inc');
include('AnnounceDocumentModel.class.inc');

class AnnounceDocument extends DocumentGenerator{
	function AnnounceDocument(){
		$this->moduleTitle = tt('AnnounceDocument');
		$this->moduleName = 'documentgenerator';
		$this->moduleExtension = 'announcedocument';
		$this->tableName = 'documentgenerator_announcedocument';
		$this->listLabel = tt('List of My AnnounceDocument');
		$this->init(new AnnounceDocumentModel());
		$this->setAccess();
		$this->setAction();
		document_set_path(tt('Document Generator'), $this->moduleName);
		document_set_path($this->moduleTitle, $this->fullModuleName);
	}
}
?>
