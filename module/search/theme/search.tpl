<?php if(!defined('XIMPLE_CMS')) die();?>
<h2 class="list_label"><?=sprintf(tt('Search Result %d-%d of %d'), 10*MODE_PAGE, 10*MODE_PAGE+10, $result_number)?></h2>
<?php foreach($data as $key=>$value):?>
	<h3>
		<a href="<?=SECTION_URI.Q.$value['content_mode'].'/'.$value['content_id'].'/search_keyword/'.$value['keyword'].'/search_id/'.$key?>"><?=$value['topic']?></a>
	</h3>
	<h4><?=$value['keyword'].' : '.sprintf(tt('%d points, %d keywords'), $value['points'], $value['keywords_no'])?></h4>
	<p><?=$value['rendered']?></p>
<?php endforeach?>
