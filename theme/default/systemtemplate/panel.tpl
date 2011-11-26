<?php if(!defined('XIMPLE_CMS')) die();?>
<?php $i=0;?>
<h4><?=sprintf(tt('Welcome %s.'), USER_NAME)?></h4>
<?php foreach($panel as $module => $items) :?>
	<h4 class="panel_head"><a href="javascript:togglePanel('panel_<?=$i?>')"><?=$module?></a></h4>
	<ul class="panel_body" id="panel_<?=$i?>" style="display:none;">
	<?php foreach($items as $name => $href) :?>
		<li><a href="<?=SECTION_URI.Q.$href?>"><?= $name?></a></li>
	<?php endforeach?>
	</ul>
	<?php $i++;?>
<?php endforeach?>
