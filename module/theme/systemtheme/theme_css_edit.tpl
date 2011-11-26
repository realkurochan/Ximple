<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Edit Custom CSS : ').$file_name?></h3>
<form action="<?=SECTION_URI.Q?>theme_save_css" method="post">
	<input name="file" value="<?=$file_name?>" type="hidden" />
	<textarea name="value" style="width:99%;border:1px solid #999;" rows="25"><?=$data?></textarea>
	<input type="submit" value="submit custom CSS" style="margin-top:15px;"/>
</form>
