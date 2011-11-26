<?php if(!defined('XIMPLE_CMS')) die();?>
<h3><a href="<?=SECTION_URI.Q.'advertise_redirect/'.$id?>"><?=$name?></a></h3>
<?=$banner?>
<p><strong><?=tt('position')?></strong> : <?=$position?></p>
<p><strong><?=tt('clicks')?></strong> : <!--{ximple_read/advertise/<?=$id?>}--></p>
<p><strong><?=tt('advertising desciption')?></strong></p>
<p><?=$description?></p>
<p><strong><?=tt('customer description')?></strong></p>
<p><?=$customer?></p>
<p>
	<a href="<?=SECTION_URI.Q.'advertise_edit/'.$id?>"><?=tt('edit')?></a> |
	<a href="<?=SECTION_URI.Q.'advertise_drop/'.$id?>"><?=tt('drop')?></a>
</p>
