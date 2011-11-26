<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Manage Forum')?></h3>
<label><?=tt('Legend')?></label>
<ul>
	<li><img src="<?=ROOT_URI?>files/icon/move.png"/> : <?=tt('Sort and select category by drag-and-drop')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/edit.png"/> : <?=tt('Edit Forum')?></li>
</ul>
<p>
	<?=tt('You can use this form to sort and manage forums by drag-and-drop. Please, do not forget to click submit.')?>
</p>
<p>
	<?=render_write_icon('category_write/module/forum', tt('Create new Category'))?>
</p>
<p>
	<?=render_write_icon('forum_write', tt('Create new Forum'))?>
</p>
<form action="<?=SECTION_URI.Q?>forum_save_sort" method="post" onsubmit="getCategory()">
<?php $sortable_id = "";?>
<?php foreach($category as $reference => $title):?>
<a name="category_<?=$reference?>" />
<fieldset>
	<legend><label><?=tt('Category').' : '.$title?></label></legend>
	<?php $sortable_id .= '#forum_sort_'.$reference.', ';?>
	<div id="forum_sort_<?=$reference?>" class="sortable">
	<?php if(isset($data[$reference])):?>
		<?php foreach($data[$reference] as $forum):?>
		<div class="sortable_item">
			<a name="forum_<?=$forum['id']?>" />
			<img src="<?=ROOT_URI?>files/icon/move.png" align="left"/> 
			<a href="<?=SECTION_URI.Q.'forum_edit/'.$forum['id']?>" style="text-decoration:none;">
				<img src="<?=ROOT_URI?>files/icon/edit.png" align="left" border="0"/>
			</a>
			<?=$forum['name']?>
			<input type="hidden" name="category[]" class="category_input" value=""/>
			<input type="hidden" name="forum[]" value="<?=$forum['id']?>"/>
		</div>
		<?php endforeach?>
	<?php endif?>
	</div>
</fieldset>
<?php endforeach?>

<input type="submit" value="<?=tt('save forums')?>" />
</form>

<script type="text/javascript">
$(document).bind("ready", function(){
	$("<?=substr($sortable_id, 0, -2)?>").sortable({
		connectWith: '.sortable',
		dropOnEmpty: true,
	});
	$("<?=substr($sortable_id, 0, -2)?>").disableSelection();
});

function getCategory(){
	<?php foreach($category as $reference => $title):?>
		$('#forum_sort_<?=$reference?> div .category_input').attr('value', "<?=$reference?>");
	<?php endforeach?>
}
</script>

