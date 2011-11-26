<?php if(!defined('XIMPLE_CMS')) die();?>
<ul class="banner_list">
<?php foreach($data as $key => $value):?>
	<li>
		<a href="<?=SECTION_URI.Q.'advertise_redirect/'.$key?>">
			<img src="<?=ROOT_URI.$value['banner']?>" alt="<?=$value['name']?>" border="0"/>
		</a>
		<a href="<?=$value['uri']?>" class="direct_link"><?=$value['name']?></a>
	</li>
<?php endforeach?>
</ul>
