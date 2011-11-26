<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="list">
	<h3 class="title_label"><?=$seminar['topic']?></h3>
	<ul>
	<?php foreach($attendant as $key => $value):?>
		<li><?=render_profile_link($value['attendant'])?></li>
	<?php endforeach?>
	</ul>
</div>
