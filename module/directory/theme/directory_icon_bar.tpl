<?php if(!defined('XIMPLE_CMS')) die();?>
<div id="directory_icon_bar">
	<ul>
	<?php foreach($icon as $key => $value):?>
		<li><?=$value?></li>
	<?php endforeach?>
	</ul>
	<div style="clear:both;height:1px;padding:0;margin:0;"></div>
</div>
<p><a href="javascript:toggleLabel()"><?=tt('toggle label')?></a></p>
