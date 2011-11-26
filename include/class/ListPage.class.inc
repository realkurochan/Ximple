<?php
class ListPage{
	private $modelConnector;
	private $addtionalParameter;
	private $listMeta;
	private $data;
	private $column;
	private $label;
	
	function ListPage($modelConnector, $listMeta){
		$this->modelConnector = $modelConnector;
		$this->listMeta = $listMeta;
		$this->column = $modelConnector->getColumnName();
	}
	
	public function render($data){
		head_add_css('list.css');
		$this->prepareRender();
		$renderArray = array(
			'data' => $data,
			'meta' => $this->listMeta,
			'sort_mode' => $this->getSortMode(),
			'column' => $this->listMeta->column,
			'label' => $this->label,
			'is_sort' => $this->modelConnector->getColumnNameByBoolean('isOrderable'),
			'mode' => $this->modelConnector->getColumnAttribute('listMode'),
		);
		return render($renderArray, 'list.tpl', 'list');
	}
	
	public function getClause($where){
		$clause = $this->getWhereClause($where);
		if(!empty($this->listMeta->groupBy)){
			$clause .= ' GROUP BY '.$this->listMeta->groupBy;
		}
		$order = gg('order');
		$order_desc = gg('order_desc');
		if(!empty($order) and in_array($order, $this->column)){
			$clause .= ' ORDER BY '.$order;
		}elseif(!empty($order_desc) and in_array($order_desc, $this->column)){
			$clause .= ' ORDER BY '.$order_desc.' DESC';
		}elseif(!empty($this->listMeta->defaultOrder)){
			$clause .= ' ORDER BY '.$this->listMeta->defaultOrder;
		}elseif(!empty($this->listMeta->defaultOrderDesc)){
			$clause .= ' ORDER BY '.$this->listMeta->defaultOrderDesc.' DESC';
		}
		return $clause;
	}
	
	public function getPageMode(){
		$mode  = $this->listMeta->mode;
		$mode .= $this->getSearchMode();
		$mode .= $this->getOrderMode();
		return $mode;
	}
	
	public function renderSearchForm(){
		list($searchColumn, $search) = $this->getSearchRequest();
		$reachableColumn = $this->modelConnector->getColumnNameByBoolean('isReachable');
		if(count($reachableColumn)){
			$renderArray = array(
				'request' => gg(),
				'meta' => $this->listMeta,
				'param' => $this->listMeta->getParameter(),
				'col' => $reachableColumn,
				'search_col' => $searchColumn,
				'search' => $search,
				'label' => $this->label,
			);
			return render($renderArray, 'list_search.tpl', 'list_search');
		}
	}
	
	private function prepareRender(){
		if(empty($this->label)){
			$this->label = $this->modelConnector->getColumnAttribute('label');
		}
	}
	
	private function getSortMode(){
		$mode  = $this->listMeta->mode;
		$mode .= $this->getSearchMode();
		$mode .= '/page/'.$this->listMeta->page;
		return $mode;
	}
	
	private function getSearchMode(){
		list($col, $search) = $this->getSearchRequest();
		if(!empty($search)){
			return '/col/'.$col.'/search/'.$search;
		}
	}
	
	private function getOrderMode(){
		$order = gg('order');
		$order_desc = gg('order_desc');
		if(!empty($order) and in_array($order, $this->column)){
			return '/order/'.$order;
		}elseif(!empty($order_desc) and in_array($order_desc, $this->column)){
			return'/order_desc/'.$order_desc;
		}
	}
	
	private function getSearchRequest(){
		$search = addslashes(str_replace(' ', '', trim((string)gg('search'))));
		$col = gg('col');
		if(strlen($search) and strlen($col) and in_array($col, $this->column)){
			return array($col, $search);
		}
		return array('', '');
	}
	
	private function getWhereClause($where){
		list($col, $search) = $this->getSearchRequest();
		if(strlen($search) == 0) return $where;
		
		$isUser = $this->modelConnector->getColumnNameByBoolean('isUser');
		$columnType = $this->modelConnector->getColumnAttribute('type');
		$numeric = array('int', 'real');
		if(in_array($col, $isUser)){
			$uid = db_select_single_column('users', 'id', "WHERE login_name LIKE '%$search%'");
			if(count($uid)){
				$clause = $col.'='.join(' OR '.$col.'=', $uid);
				if(!empty($where)) $clause = $where.' AND '.$clause;
				else $clause = 'WHERE '.$clause;
			}else{
				$clause = $where.' AND false';
			}
		}elseif(!in_array($columnType[$col], $numeric)){
			if(!empty($where)){
				$clause = $where.' AND '.$col." LIKE '%".$search."%'";
			}else{
				$clause = 'WHERE '.$col." LIKE '%".$search."%'";
			}
		}else{
			if(!empty($where)){
				$clause = $where.' AND '.$col."=".$search;
			}else{
				$clause = 'WHERE '.$col."=".$search;
			}
		}
		if(empty($clause)) $clause = $where;
		return $clause;
	}
}
?>
