<?php if(!defined('XIMPLE_CMS')) die();?>
<div id="pager">
	<?php if($end-$begin > 1) :?>
		<a href="<?=SECTION_URI.Q.$uri?>/page/0">&#171;</a>
		<?php for($i=$begin;$i < $end;$i++) :?>
			<?php if($current == $i) :?>
				<b><?=$i+1?></b>
			<?php else :?>
				<a href="<?=SECTION_URI.Q.$uri?>/page/<?=$i?>"><?=$i+1?></a>
			<?php endif?>
		<?php endfor?>
	<?php endif?>
	
	<?php if($previous != -1):?>
		<a href="<?=SECTION_URI.Q.$uri?>/page/<?=$previous?>">&#8249;</a>
	<?php endif?>
	
	<?php if($next != -1):?>
		<a href="<?=SECTION_URI.Q.$uri?>/page/<?=$next?>">&#8250;</a>
	<?php endif?>
	
	<?php if($end-$begin > 1) :?>
		<a href="<?=SECTION_URI.Q.$uri?>/page/<?=$last?>">&#187;</a>
	<?php endif?>
</div>
