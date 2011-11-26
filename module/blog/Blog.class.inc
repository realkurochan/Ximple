<?php
execute_add_class('blog', 'Blog');
include_once(MODULE_PATH.'content/Content.class.inc');
include_once('blog.tool.inc');
include_once('BlogModel.class.inc');

class Blog extends Content{
	function Blog(){
		$this->tableName = 'blog';
		$this->moduleName = 'blog';
		$this->moduleTitle = tt('Blog');
		$this->addAction('blog_promote_widget', 'renderPromoteWidget');
		$this->addAccessMethod('guest', 'renderPromoteWidget');
		$this->init(new BlogModel());
		document_set_path($this->moduleTitle, $this->moduleName);
	}
	
	public function index(){
		if($this->isSingleEntry()) return $this->renderSingleBlog();
		else return $this->renderMultipleBlogs();
	}
	
	public function printPage(){
		$this->id = MODE_ID;
		$this->selectByID($this->column);
		execute_set_print();
		document_set_title($this->dataByID['topic']);
		$this->dataByID['content'] = $this->dataByID['intro'].$this->dataByID['content'];
		return render($this->dataByID, 'content_print.tpl', 'blog_print');
	}
	
	public function renderPromoteWidget(){
		$column = array('id', 'topic', 'icon', 'intro', 'uri');
		$clause = 'WHERE section='.SECTION.' AND publishing_status=4';
		$data = db_select('blog', $column, $clause);
		$renderArray = array('data' => array_values($data));
		head_add_css('blog.css');
		return render($renderArray, 'blog_promote.tpl', 'blog_propmote');
	}
	
	protected function getSummary(){
		return $this->dataByID['intro'];
	}
	
	protected function getPublishingStatus(){
		return blog_get_publishing_status();
	}
	
	protected function isReadable(){
		$status = $this->dataByID['publishing_status'];
		$promote = blog_get_publishing_status();
		return (Content::isReadable() or ($status == $promote[4]));
	}
	
	private function renderSingleBlog(){
		$this->id = mid();
		$this->selectByID($this->column);
		$this->publishDate = date('Y-m-d H:i:s', strtotime($this->dataByID['publish_time']));
		if($this->isReadable()) return $this->executeRender();
		else $this->accessDeniedEntry();
	}
	
	private function renderMultipleBlogs(){
		head_add_css('blog.css');
		$column = $this->modelConnector->getColumnNameExcepted(array('content'));
		$this->selectFirstPage($column, cc('blog_entries_number'));
		$this->getCommentNumber();
		$this->getDataEditable();
		$this->setPathMutiplePage();
		$this->translateData();
		$appearance = cc('blog_appearance');
		if(!is_array($appearance)) $appearance = array();
		$renderArray = array(
			'data' => $this->data,
			'show_category' => in_array('category_icon', $appearance),
		);
		$render = $this->renderWriteIcon();
		$render.= render($renderArray, 'blog_multiple.tpl', 'blog_multiple');
		$render.= $this->pager->render('blog'.$this->getPageMode());
		return $render;
	}
	
	private function executeRender(){
		head_add_css('blog.css');
		$this->prepareRender();
		$render = $this->renderWriteIcon();
		$render.= render($this->dataByID, 'blog.tpl', 'blog');
		$render.= $this->comment->render();
		$this->setDocumentData();
		return $render;
	}
	
	private function prepareRender(){
		$this->translateDataByID();
		$this->handlePublishingStatus();
		$this->getSibling();
		$this->initComment();
		$this->insertTracker('read');
		$this->addMetaData();
		$this->getAppearanceConfiguration();
		$this->dataByID['editable'] = $this->isEditable($this->dataByID['writer']['id']);
		$operation = $this->getExtendedOperation();
		$this->dataByID['addition_operation'] = $operation[$this->id];
	}
	
	private function getSibling(){
		$time = "'".$this->publishDate."'";
		$clause = "WHERE publish_time > $time AND id!=".$this->id;
		$clause.= " AND (section=".SECTION." AND publishing_status < 2) ";
		$clause.= 'ORDER BY publish_time LIMIT 1';
		$next = db_select('blog', array('id', 'topic', 'uri'), $clause);
		$clause = "WHERE publish_time < $time AND id!=".$this->id;
		$clause.= " AND (section=".SECTION." AND publishing_status < 2) ";
		$clause.= 'ORDER BY publish_time DESC LIMIT 1';
		$back = db_select('blog', array('id', 'topic', 'uri'), $clause);
		$this->dataByID['next'] = array_values($next);
		$this->dataByID['back'] = array_values($back);
	}
	
	private function getAppearanceConfiguration(){
		$appearance = cc('blog_appearance');
		if(!is_array($appearance)) $appearance = array();
		$this->dataByID['show_avatar'] = in_array('avatar', $appearance);
		$this->dataByID['show_category'] = in_array('category_icon', $appearance);
		$this->dataByID['show_sibling'] = in_array('sibling', $appearance);
	}
}
?>
