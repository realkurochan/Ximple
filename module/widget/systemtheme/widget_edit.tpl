<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Widget Edit')?></h3>
<label><?=tt('Legend')?></label>
<ul>
	<li><img src="<?=ROOT_URI?>files/icon/move.png"/> : <?=tt('Sort and select position by drag-and-drop')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/edit.png"/> : <?=tt('Edit and Configure Widget')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/drop.png"/> : <?=tt('Drop Widget (Widget can be rebuilt by refreshing module)')?></li>
</ul>
<p>
	<?=tt('You can use this form to sort and set widgets position by drag-and-drop. Please, do not forget to click submit.')?>
</p>

<form action="<?=SECTION_URI.Q?>widget_save_sort" method="post" onsubmit="getPosition()">
<?php $sortable_id = "";?>
<?php foreach($position as $reference => $title):?>
<a name="position_<?=$reference?>" />
<fieldset>
	<legend><label><?=$title?></label></legend>
	<?php $sortable_id .= '#widget_sort_'.$reference.', ';?>
	<div id="widget_sort_<?=$reference?>" class="sortable">
	<?php if(isset($data[$reference])):?>
		<?php foreach($data[$reference] as $widget):?>
		<div class="sortable_item">
			<a name="widget_<?=$widget['id']?>" />
			<img src="<?=ROOT_URI?>files/icon/move.png" align="left"/> 
			<a href="<?=SECTION_URI.Q.'widget_edit/'.$widget['id']?>" style="text-decoration:none;">
				<img src="<?=ROOT_URI?>files/icon/edit.png" align="left" border="0"/>
			</a>
			<a href="<?=SECTION_URI.Q.'widget_drop/'.$widget['id']?>" style="text-decoration:none;">
				<img src="<?=ROOT_URI?>files/icon/drop.png" align="left" border="0"/>
			</a>
			<?=$widget['title']?>
			<input type="hidden" name="position[]" class="position_input" value=""/>
			<input type="hidden" name="widget[]" value="<?=$widget['id']?>"/>
			<input type="hidden" name="call_mode[]" value="<?=$widget['call_mode']?>"/>
		</div>
		<?php endforeach?>
	<?php endif?>
	</div>
	<a href="<?=SECTION_URI.Q?>widget_write/position/<?=$reference?>" style="text-decoration:none;">
		<img src="<?=ROOT_URI?>files/icon/add.png" align="left" border="0" style="margin-right:15px;"/>
		<?=sprintf(tt('Add new widget into %s'), $title)?>
	</a>
</fieldset>
<?php endforeach?>

<input type="submit" value="<?=tt('save widgets')?>" />
</form>

<script type="text/javascript">
$(document).bind("ready", function(){
	$("<?=substr($sortable_id, 0, -2)?>").sortable({
		connectWith: '.sortable',
		dropOnEmpty: true
	});
	$("<?=substr($sortable_id, 0, -2)?>").disableSelection();
});

function getPosition(){
	<?php foreach($position as $reference => $title):?>
		$('#widget_sort_<?=$reference?> div .position_input').attr('value', "<?=$reference?>");
	<?php endforeach?>
}
</script>

