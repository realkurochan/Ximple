<?php
class GridFormRender{
	private $data;
	private $memberColumn;
	private $dropMode;
	private $name;
	private $isLink;
	private $isEmptyForm;
	private $isWritable;
	
	function GridFormRender(){
	}
	
	public function setGridColumn($gridColumn){
		$this->data = $gridColumn->data;
		$this->memberColumn = $gridColumn->memberColumn;
		$this->dropMode = $gridColumn->dropMode;
		$this->name = $gridColumn->name;
		$this->isLink = $gridColumn->isLink;
		$this->isEmptyForm = $gridColumn->isEmptyForm;
		$this->isWritable = $gridColumn->isWritable;
	}
	
	public function renderForm($formID){
		head_add_css('list.css');
		head_add_js('include/javascript/grid.column.js');
		$id = $formID.'_'.$this->name;
		$render  = $this->renderHead($id);
		$render .= $this->renderDataRow($id);
		$gridRow = $this->renderRow($id);
		$this->addGridRow($id, $gridRow);
		$render .= $this->renderFoot($id, $gridRow);
		return $render;
	}
	
	private function renderHead($id){
		$render  = '<div class="list">';
		$render .= '<span id="info_'.$id.'"></span>';
		$render .= '<table width="100%" id="'.$id.'"><tr>';
		foreach($this->memberColumn as $key => $value){
			if($value->isGridForm and $value->isFormLabel){
				$render .= '<th>'.$value->label.'</th>';
			}
		}
		$render .= '<th width="36">&#160;</th>';
		return $render;
	}
	
	private function renderFoot($id, $gridRow){
		$render  = '';
		if(count($this->data) == 0 and !$this->isEmptyForm){
			$render .= $gridRow;
		}
		$render .= '</tr></table>';
		$render .= '</div>';
		$render .= $this->renderAddIcon($id);
		return $render;
	}
	
	private function renderDataRow($id){
		$render = '';
		foreach($this->data as $key => $data){
			$render .= '<tr valign="top">';
			foreach($this->memberColumn as $name => $member){
				$member->data = $data[$name];
				$form = $this->renderRowForm($id, $member);
				if($this->isLink){
					$form = str_replace('[]', '[id_'.$key.']', $form);
				}
				$render .= $form;
			}
			$render .= $this->renderDropIcon($key);
			$render .= '</tr>';
		}
		return $render;
	}
	
	private function renderRowForm($id, $model){
		if($model->isGridForm){
			if(method_exists($model, 'triggerGetJavaScript')) $model->triggerGetJavaScript();
			if($model->isFormLabel){
				return '<td>'.$model->renderGridForm($id).'</td>';
			}else{
				return $model->renderGridForm($id);
			}
		}else{
			return null;
		}
	}
	
	private function renderRow($id){
		$render = '<tr valign="top">';
		foreach($this->memberColumn as $key => $value){
			$value->data = null;
			$render .= $this->renderRowForm($id, $value);
		}
		$render.= $this->renderDropIcon($this->dropMode);
		$render.= '</tr>';
		$render = str_replace("\n", "", $render);
		return $render;
	}
	
	private function renderDropIcon($id=null){
		if($id) $dropMode = $this->dropMode.'/'.$id.'/confirm/true';
		else $dropMode = '';
		return '
			<td>
				<a onclick="delGridRow(this, \''.$dropMode.'\');return false;" href="#">
				<img src="'.ROOT_URI.'files/icon/drop.png" border="0" align="left" />
				</a>
			</td>';
	}
	
	private function renderAddIcon($id){
		if($this->isWritable){
			return '
			<a onclick="addGridRow(\''.$id.'\');return false;" href="#" class="write_icon">
				<img src="'.ROOT_URI.'files/icon/add.png" border="0" align="left" />
				'.tt('add new row').'
			</a>';
		}else{
			return null;
		}
	}
	
	private function addGridRow($id, $gridRow){
		FormRender::$gridRowArray[$id] = str_replace('"', "\\\"", $gridRow);
	}
}
?>
