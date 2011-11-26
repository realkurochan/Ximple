<?php
execute_add_extended_class('documentgenerator', 'commanddocument', 'CommandDocument');
include_once('DocumentGenerator.class.inc');
include('CommandDocumentModel.class.inc');

class CommandDocument extends DocumentGenerator{
	function CommandDocument(){
		$this->moduleTitle = tt('CommandDocument');
		$this->moduleName = 'documentgenerator';
		$this->moduleExtension = 'commanddocument';
		$this->tableName = 'documentgenerator_commanddocument';
		$this->listLabel = tt('List of My CommandDocument');
		$this->init(new CommandDocumentModel());
		$this->setAccess();
		$this->setAction();
		document_set_path(tt('Document Generator'), $this->moduleName);
		document_set_path($this->moduleTitle, $this->fullModuleName);
	}
}
?>
