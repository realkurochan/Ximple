<?php

class PagerRender{
	private $pageLength;
	private $rowsNumber;
	private $nextPage;
	
	function PagerRender($pageLength=null, $rowsNumber=null){
		$this->pageLength = $pageLength;
		$this->rowsNumber = $rowsNumber;
	}
	
	public function setPageLength($pageLength){
		$this->pageLength = (int)$pageLength;
	}
	
	public function setRowsNumber($rowsNumber){
		$this->rowsNumber = (int)$rowsNumber;
	}
	
	public function render($uri){
		if(empty($this->rowsNumber) or empty($this->pageLength)) return null;
		$current = MODE_PAGE;
		$last = (ceil($this->rowsNumber/$this->pageLength)-1);
		$begin = ($current-5 >= 0) ? $current - 5 : 0 ;
		$next = (($current+1)*$this->pageLength < $this->rowsNumber) ? $current + 1 : -1;
		$previous = ($current > 0) ? $current - 1 : -1;
		$pagesNumber = $this->rowsNumber/$this->pageLength;
		if($begin+10 < $pagesNumber){
			$end = $begin+10;
		}else{
			$end = (int) ceil($pagesNumber);
			$begin = ($end-10 >= 0) ? $end-10 : 0;
		}
		
		if($this->rowsNumber%$this->pageLength != 0) $this->nextPage = $last;
		else $this->nextPage = $last+1;
		
		$renderArray = array(
			'current' => $current,
			'last' => $last,
			'next' => $next,
			'previous' => $previous,
			'begin' => $begin,
			'end' => $end,
			'uri' => $uri,
		);
		
		head_add_css('pager.css');
		return render($renderArray, 'pager.tpl', 'render_pager');
	}
	
	public function getNextPage(){
		if($this->nextPage == null) return 0;
		else return $this->nextPage;
	}
}

?>
