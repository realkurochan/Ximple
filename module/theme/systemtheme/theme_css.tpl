<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Custom CSS')?></h3>
<p><?=tt('You can select and edit CSS file from the followed list to customize your theme.')?></p>
<ul>
	<?php foreach($data as $value):?>
		<li style="line-height:35px;">
			<a href="<?=SECTION_URI.Q?>theme_edit_css/file/<?=$value?>"><?=$value?></a>
		</li>
	<?php endforeach?>
</ul>
