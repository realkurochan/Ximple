<?php if(!defined('XIMPLE_CMS')) die();?>
<ul style="list-style:circle;">
<?php foreach($data as $id => $value):?>
	<?php $label = sprintf('%s (%s, %s, %d workload)', $value['topic'], $value['deadline'], $value['priority'], $value['work_load']);?>
	<li><?=render_operation_link('develcard', $id, $label)?></li>
<?php endforeach?>
</ul>
