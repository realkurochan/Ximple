<?php if(!defined('XIMPLE_CMS')) die();?>
<h3 class="list_label"><?=tt('Rearrange FAQ')?></h3>
<label><?=tt('Legend')?></label>
<ul>
	<li><img src="<?=ROOT_URI?>files/icon/move.png"/> : <?=tt('Sort and select category by drag-and-drop')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/edit.png"/> : <?=tt('Edit FAQ')?></li>
	<li><img src="<?=ROOT_URI?>files/icon/drop.png"/> : <?=tt('Drop FAQ')?></li>
</ul>
<p>
	<?=tt('You can use this form to sort and set FAQ arrangement by drag-and-drop. Please, do not forget to click submit.')?>
</p>

<?=$write_icon?>

<form action="<?=SECTION_URI.Q?>faq_save_arrange" method="post" onsubmit="getCategory()">
<?php $sortable_id = "";?>
<?php foreach($category as $cid => $title):?>
<a name="category_<?=$cid?>" />
<fieldset>
	<legend><label><?=$title?></label></legend>
	<?php $sortable_id .= '#faq_sort_'.$cid.', ';?>
	<ul id="faq_sort_<?=$cid?>" class="sortable">
	<?php if(isset($data[$cid])):?>
		<?php foreach($data[$cid] as $faq):?>
		<li>
			<a name="faq_<?=$faq['id']?>" />
			<img src="<?=ROOT_URI?>files/icon/move.png" align="left"/> 
			<a href="<?=SECTION_URI.Q.'faq_edit/'.$faq['id']?>" style="text-decoration:none;">
				<img src="<?=ROOT_URI?>files/icon/edit.png" align="left" border="0"/>
			</a>
			<a href="<?=SECTION_URI.Q.'faq_drop/'.$faq['id']?>" style="text-decoration:none;">
				<img src="<?=ROOT_URI?>files/icon/drop.png" align="left" border="0"/>
			</a>
			<?=$faq['question']?>
			<input type="hidden" name="category[]" class="category_input" value=""/>
			<input type="hidden" name="faq[]" value="<?=$faq['id']?>"/>
		</li>
		<?php endforeach?>
	<?php endif?>
	</ul>
</fieldset>
<?php endforeach?>

<input type="submit" value="<?=tt('save FAQ arrage')?>" />
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
	<?php foreach($category as $cid => $title):?>
		$('#faq_sort_<?=$cid?> li .category_input').attr('value', "<?=$cid?>");
	<?php endforeach?>
}
</script>

