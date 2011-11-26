<?php if(!defined("XIMPLE_CMS")) die();?>
<?php if(!defined('XIMPLE_CMS')) die();?>
<script>
	function videoShowEmbed(){
		var embedDialog = $('#embed_dialog');
		embedDialog.show();
		embedDialog.dialog({
			bgiframe: true,
			autoOpen: true,
			title: 'Embed Code',
			width:540,
			height:250,
		});
	}
</script>
<div class="video">
	<h3 class="title_label"><?=$topic?></h3>
	<table><tbody>
	<tr>
		<?php if(cc('video_show_avatar') or cc('video_show_category')):?>
			<td colspan="2">
		<?php else:?>
			<td>
		<?php endif?>
			<div style="text-align:center;padding:20px;">
				<?php ob_start();?>
				<object width="<?=cc('video_width')?>" height="<?=cc('video_height')?>" id="flvPlayer">
					<embed
						src="<?=ROOT_URI?>module/video/osflv/OSplayer.swf?movie=<?=ROOT_URI.$file?>&btncolor=0x333333&accentcolor=0x31b8e9&txtcolor=0xdddddd&volume=30&autoload=off&autoplay=off&vTitle=<?=$topic?>&showTitle=yes"
						width="<?=cc('video_width')?>"
						height="<?=cc('video_height')?>"
						allowFullScreen="true"
						type="application/x-shockwave-flash"
						allowScriptAccess="always"
					>
				</object>
				<?php $embed = ob_get_contents();?>
				<?php ob_end_clean();?>
				<?=$embed?>
			</div>
			<?=$content?>
		</td>
	</tr>
	</tbody></table>
	<p class="video_info">
		<?=$publish_time?> | <?=tt('by')?> <?=render_profile_link($writer)?> | <?=tt('tags')?>
		<?php foreach($freetag as $key=>$tag):?>
			<a href="<?=SECTION_URI.Q?>video/tag/<?=$tag?>" class="tag"><?=$tag?></a>
		<?php endforeach?>
	</p>
	<p class="operation_bar">
		<span>read <!--{ximple_read/video/<?=$id?>}--></span>
		<a href="<?=ROOT_URI.$file?>"><?=tt('download')?></a>
		<a href="#" onclick="videoShowEmbed();return false;"><?=tt('embed')?></a>
		<?php if($editable):?>
			<?=render_operation_link('video_edit', $id, ('edit'))?>
			<?=render_operation_link('video_drop', $id, ('drop'))?>
		<?php endif?>
		<?php foreach($addition_operation as $link):?>
			<?=$link?>
		<?php endforeach?>
	</p>
	<div id="embed_dialog" style="display:none;">
		<textarea style="width:500px;height:180px;"><?=input_trim($embed);?></textarea>
	</div>
</div>
