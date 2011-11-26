<div class="note" >
<h3 class="list_label"><?=tt('My Short Note')?></h3>
<?=render_write_icon('shortnote_write', tt('Write new Short Note'))?>
<?php $i=0;?>
<a name="top_note" />
<?php foreach($data as $key => $value) :?>
	<a name="note_<?=$value['id']?>" />
	<?php if($i%2 == 0):?>
	<div class="note_item_even">
	<?php else:?>
	<div class="note_item_odd">
	<?php endif?>
		<div class="note_head"></div>
		<div class="note_container">
			<div class="note_body">
				<?=$value['note']?>
			</div>
		</div>
		<div class="note_foot">
			<?=$value['write_time']?>
			<a href="<?=SECTION_URI.Q?>shortnote_drop/<?=$value['id']?>">
				<img src="<?=ROOT_URI?>files/icon/stop.png" border="0" style="margin-left:10px;"/>
			</a>
			<a href="<?=SECTION_URI.Q?>shortnote_edit/<?=$value['id']?>">
				<img src="<?=ROOT_URI?>files/icon/edit.png" border="0" style="margin-left:10px;"/>
			</a>
		</div>
	</div>
	<?php $i++;?>
<?php endforeach?>
<?=render_write_icon('shortnote_write', tt('Write new Short Note'))?>
</div>
