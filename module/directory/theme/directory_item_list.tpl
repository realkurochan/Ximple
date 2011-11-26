<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="directory_element_label" style="display:none;"><?=tt('list of sub-directories')?></div>
<div class="directory_element">
	<div id="directory_item_list">
		<h4><?=$label?></h4>
		<ul>
		<?php foreach($item as $id => $value):?>
			<li><a href="<?=render_uri('directory', $value)?>"><?=$value['name']?></a></li>
		<?php endforeach?>
		</ul>
	</div>
</div>
