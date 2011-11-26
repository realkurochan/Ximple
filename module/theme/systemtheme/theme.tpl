<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Manage Theme')?></h3>
<p><?=tt('You can select theme of Section with this page. Afterwards, you can customize your theme by :')?></p>
<ul>
	<li><a href="<?=SECTION_URI.Q?>theme_config">config theme</a></li>
	<li><a href="<?=SECTION_URI.Q?>theme_css">custom css</a></li>
	<li><a href="<?=SECTION_URI.Q?>theme_image">custom image</a></li>
</ul>
<p>
	<?=sprintf(tt('Or you can <a href="%s"><strong>reset your theme</strong></a>.'), SECTION_URI.Q.'theme_reset/page/'.pid())?>
	<?=sprintf(tt('For theme development, you should <a href="%s"><strong>clear custom theme directories</strong></a> to simplify your work.'), SECTION_URI.Q.'theme_clear_custom/page/'.pid())?>
</p>
<div class="theme">
	<table width="100%"><tbody>
	<?php $begin = pid()*THEME_PAGE_LENGTH;?>
	<?php $end = pid()*THEME_PAGE_LENGTH + THEME_PAGE_LENGTH;?>
	<?php $end = ($end < count($data)) ? $end: count($data);?>
	<?php $names = array_keys($data);?>
	<?php for($i = $begin; $i < $end; $i++):?>
		<?php $name = $names[$i];?>
		<?php $value = $data[$name];?>
		<?php if($i%THEME_ROW_LENGTH == 0):?>
			<tr>
		<?php endif?>
		<?php if($value['current']):?>
			<td align="center" valign="top" class="current_theme" width="50%">
		<?php else:?>
			<td align="center" valign="top" class="select_theme" width="50%">
		<?php endif?>
				<div class="theme_head">
					<?php if($value['current']):?>
						<h4><?=$value['name']?></h4>
						<a name="selected_theme" />
						<a href="<?=SECTION_URI.Q?>theme_reset/"><b>reset <?=tt('current theme')?></b></a>
					<?php else:?>
						<a href="<?=SECTION_URI.Q?>theme_save/theme/<?=$name?>/page/<?=pid()?>">
							<h4><?=$value['name']?></h4>
							<?=tt('use this theme')?>
						</a>
					<?php endif?>
				</div>
				<div style="height:120px;">
					<img src="<?=$value['preview']?>" class="theme_preview_image" alt="theme preview image" />
				</div>
				<p><?=$value['description']?></p>
			</td>
		<?php if($i%THEME_ROW_LENGTH == THEME_ROW_LENGTH-1):?>
			</tr>
		<?php endif?>
	<?php endfor?>
	</tbody></table>
</div>
