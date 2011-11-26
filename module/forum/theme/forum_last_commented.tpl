<?php if(!defined('XIMPLE_CMS')) die();?>
<ul>
<?php foreach($data as $key => $value):?>
	<li>
		<a href="<?=SECTION_URI.Q.'forum_entry/'.$key?>"><?=$value['topic']?></a>
		<?=tt('by')?> <?=render_profile_link($value['writer'])?> @ <?=$value['write_time']?>
		<?=tt('in')?> <a href="<?=SECTION.Q.'forum/'.$value['forum']['id']?>"><?=$value['forum']['name']?></a>
		<?php if(strlen($value['last_commentator'])):?>
			<?=tt('commented by').' '.render_profile_link($value['last_commentator_info'])?>
		<?php endif?>
	</li>
<?php endforeach?>
</ul>
