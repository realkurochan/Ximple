<?php
abstract class DocumentGenerator extends Control{
	public function index(){
		$this->id = (int) gg('id');
		if($this->id) return $this->renderStructure();
		else return $this->contributorListPage();
	}
	
	public function generatePDF(){
		$this->id = (int) gg('id');
		if($this->id){
			$this->checkDirectory();
			$this->createContent();
			$this->createODT();
			$this->convertODT2PDF();
			$this->sendPDF();
		}else{
			error_not_found();
		}
	}
	
	public function generateODT(){
		$this->id = (int) gg('id');
		if($this->id){
			$this->checkDirectory();
			$this->createContent();
			$this->createODT();
			$this->sendODT();
		}else{
			error_not_found();
		}
	}
	
	public function generateDOC(){
		$this->id = (int) gg('id');
		if($this->id){
			$this->checkDirectory();
			$this->createContent();
			$this->createODT();
			$this->convertODT2DOC();
			$this->sendDOC();
		}else{
			error_not_found();
		}
	}
	
	public function preview(){
		$this->id = (int) gg('id');
		if($this->id){
			document_set_path(tt('Document Generator'), $this->moduleName);
			document_set_path($this->moduleTitle, $this->fullModuleName);
			document_set_path(tt('Preview'));
			$this->checkDirectory();
			$this->createContent();
			$this->createODT();
			$this->convertODT2PDF();
			$previewFile = 'files/'.$this->moduleName.'/'.$this->moduleExtension.'/'.USER_ID.'/'.$this->id.'.pdf';
			$render_array = array(
				'fullModuleName' => $this->fullModuleName,
				'id' => $this->id,
				'previewFile' => $previewFile,
			);
			return render($render_array, 'documentgenerator_preview.tpl', 'documentgenerator');
		}else{
			error_not_found();
		}
	}
	
	private function checkDirectory(){
		$this->workDirectory = FILES_PATH.'documentgenerator/';
		if(!is_dir($this->workDirectory)) mkdir($this->workDirectory);
		$this->workDirectory .= $this->moduleExtension.'/';
		if(!is_dir($this->workDirectory)) mkdir($this->workDirectory);
		$this->workDirectory .= USER_ID.'/';
		if(!is_dir($this->workDirectory)) mkdir($this->workDirectory);
		$this->contentDirectory = $this->workDirectory."$this->id/";
		if(!is_dir($this->contentDirectory)) mkdir($this->contentDirectory);
	}
	
	private function createContent(){
		$this->selectByID($this->column);
		$content = render($this->dataByID, $this->fullModuleName.'_xml_content.tpl', $this->fullModuleName.'_xml_content');
		io_write_file($this->contentDirectory.'content.xml', $content);
	}
	
	private function createODT(){
		$this->includeDirectory = dirname(__FILE__).'/odtTemplate/'.$this->moduleExtension.'/';
		$this->odtName = $this->workDirectory.$this->id.'.odt';
		require_once($this->includeDirectory.'dZip.inc.php'); // include Class
		$zip = new dZip($this->odtName); // New Class
		$zip->addDir($this->includeDirectory.'Configurations2', 'Configurations2'); // Add Folder
		$zip->addDir($this->includeDirectory.'META-INF', 'META-INF');
		$zip->addDir($this->includeDirectory.'Pictures', 'Pictures');
		$zip->addDir($this->includeDirectory.'Thumbnails', 'Thumbnails');
		$zip->addFile($this->contentDirectory.'content.xml', 'content.xml');
		$zip->addFile($this->includeDirectory.'manifest.rdf', 'manifest.rdf');
		$zip->addFile($this->includeDirectory.'meta.xml', 'meta.xml');
		$zip->addFile($this->includeDirectory.'mimetype', 'mimetype');
		$zip->addFile($this->includeDirectory.'settings.xml', 'settings.xml');
		$zip->addFile($this->includeDirectory.'styles.xml', 'styles.xml');
		$zip->save();
	}
	
	private function convertODT2PDF(){
		$this->pdfName = $this->workDirectory.$this->id.'.pdf';
		ob_start();
		$ch = curl_init("http://localhost:4097/$this->odtName&$this->pdfName");
		curl_exec($ch);
		curl_close($ch);
		$result = ob_get_contents();
		ob_end_clean();
	}
	
	private function convertODT2DOC(){
		$this->docName = $this->workDirectory.$this->id.'.doc';
		ob_start();
		$ch = curl_init("http://localhost:4097/$this->odtName&$this->docName");
		curl_exec($ch);
		curl_close($ch);
		$result = ob_get_contents();
		ob_end_clean();
	}
	
	private function sendPDF(){
		$filename = $this->pdfName;
		header('Content-type: '.'pdf');
		header('Content-Disposition: attachment; filename="'.$this->dataByID['subject'].'.pdf'.'"');
		readfile($filename);
		execute_set_file();
	}
	
	private function sendODT(){
		$filename = $this->odtName;
		header('Content-type: '.'odt');
		header('Content-Disposition: attachment; filename="'.$this->dataByID['subject'].'.odt'.'"');
		readfile($filename);
		execute_set_file();
	}
	
	private function sendDOC(){
		$filename = $this->docName;
		header('Content-type: '.'doc');
		header('Content-Disposition: attachment; filename="'.$this->dataByID['subject'].'.doc'.'"');
		readfile($filename);
		execute_set_file();
	}
	
	public function renderStructure(){
		document_set_path(tt('Document Generator'), $this->moduleName);
		document_set_path($this->moduleTitle, $this->fullModuleName);
		document_set_path(tt('Structure of '.$this->moduleTitle));
		$this->selectByID($this->column);
		return render($this->dataByID, $this->fullModuleName.'_structure.tpl', 'documentgenerator_structure');
	}
	
	public function contributorListPage(){
		$previewIcon = ROOT_URI.'module/documentgenerator/theme/preview.png';
		$pdfIcon = ROOT_URI.'files/icon/pdf.png';
		$odtIcon = ROOT_URI.'module/documentgenerator/theme/odt.png';
		$wordIcon = ROOT_URI.'files/icon/word.png';
		document_set_path($this->listLabel);
		$this->listMeta->description = 
		"<img src='$previewIcon' />".'Preview document'.'<br>'.
		"<img src='$pdfIcon' />".'Generate PDF'.'<br>'.
		"<img src='$odtIcon' />".'Generate ODT'.'<br>'.
		"<img src='$wordIcon' />".'Generate DOC';
		$this->listMeta->addOperation($this->fullModuleName.'_preview', tt('generate PDF'), 'preview.png');
		$this->listMeta->addOperation($this->fullModuleName.'_genpdf', tt('generate PDF'), 'pdf.png');
		$this->listMeta->addOperation($this->fullModuleName.'_genodt', tt('generate ODT'), 'odt.png');
		$this->listMeta->addOperation($this->fullModuleName.'_gendoc', tt('generate DOC'), 'word.png');
		$this->listMeta->title = $this->listLabel;
		$this->isOwnerMode = true;
		return $this->renderListPage('WHERE (owner='.USER_ID.')');
	}
	
	protected function renderListPage($clause){
		$this->listMeta->defaultOrder = 'category';
		$this->setListWriteIcon(tt('Write new Document'));
		$this->setListEditIcon();
		$this->setListDropIcon();
		$this->selectListPage(20, $clause);
		return Control::renderListPage();
	}
	
	protected function setAccess(){
		$this->setAdmin($this->moduleName.'_admin');
		$this->ownerColumn = 'owner';
		$this->addAccessMethod('guest', 'index');
		$this->addAccessMethod($this->moduleName.'_contributor', 'write');
		$this->addAccessMethod($this->moduleName.'_contributor', 'insert');
		$this->addAccessMethod($this->moduleName.'_contributor', 'contributorListPage');
		$this->addAccessMethod($this->moduleName.'_contributor', 'generatePDF');
		$this->addAccessMethod($this->moduleName.'_contributor', 'generateODT');
		$this->addAccessMethod($this->moduleName.'_contributor', 'generateDOC');
		$this->addAccessMethod($this->moduleName.'_contributor', 'preview');
		$this->addAccessMethod($this->moduleName.'_contributor', 'renderStructure');
	}
	
	protected function setAction(){
		$this->addAction($this->fullModuleName.'_contributor_list', 'contributorListPage');
		$this->addAction($this->fullModuleName.'_genpdf', 'generatePDF');
		$this->addAction($this->fullModuleName.'_genodt', 'generateODT');
		$this->addAction($this->fullModuleName.'_gendoc', 'generateDOC');
		$this->addAction($this->fullModuleName.'_preview', 'preview');
	}
	
	protected function renderWriteIcon(){
		if($this->isCallable('write')){
			return render_write_icon($this->fullModuleName.'_write', tt('Write new document.'));
		}
	}
}
?>
