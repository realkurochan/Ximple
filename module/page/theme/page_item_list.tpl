<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="directory_element_label" style="display:none;"><?=tt('list of pages')?></div>
<div class="directory_element">
	<div id="page_item_list">
		<h4><?=$label?></h4>
		<ul>
		<?php foreach($item as $id => $value):?>
			<li><a href="<?=render_uri('page', $value)?>"><?=$value['topic']?></a></li>
		<?php endforeach?>
		</ul>
	</div>
</div>
