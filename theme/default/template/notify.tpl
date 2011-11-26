<?php if(!defined('XIMPLE_CMS')) die();?>
<?php if($data['number']):?>
	<?php foreach($data as $key => $value):?>
		<?php if(strlen($value) and $key != 'number'):?>
		<div id="notify_<?=$key?>" class="notify">
			<h4>
				<img src="<?=ROOT_URI.'files/icon/notify_'.$key.'.png'?>" align="left"/>
				<?=$title[$key]?>
			</h4>
			<ul>
				<?=$value?>
			</ul>
			<?php if($show_close):?>
				<p align="right" style="margin-right:15px;"><a href="javascript:closeNotifyEntry('<?=$key?>')">close</a></p>
			<?php endif?>
		</div>
		<?php endif?>
	<?php endforeach?>
	<script>
		function closeNotifyEntry(id){
			$('#notify_'+id).hide('blind');
		}
	</script>
<?php endif?>
