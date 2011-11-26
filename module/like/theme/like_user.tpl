<?php if(!defined('XIMPLE_CMS')) die();?>
<?php foreach($data as $id => $value):?>
<p>
	<img src="<?=ROOT_URI.$value['avatar']?>" class="like_avatar" align="baseline"/>
	<?=render_profile_link($value)?>
</p>
<?php endforeach?>
<a href="#" onclick="return hideLikePeople()"><?=tt('close')?></a>
