<script>
var isStop = new Array();
var pictures = new Array();
var current = new Array();
<?php foreach($data as $key => $value):?>
	pictures[<?=$key?>] = new Array();
	<?php $i=0;?>
	<?php foreach($value['picture'] as $pictureID => $pictureName):?>
		pictures[<?=$key?>][<?=$i?>] = "<?=ROOT_URI?>files/gallery/<?=$key?>/thumb_<?=$pictureName?>";
		<?php $i++;;?>
	<?php endforeach?>
	current[<?=$key?>] = 0;
	isStop[<?=$key?>] = false;
<?php endforeach?>

function changeThumb(gid){
	var img = document.getElementById("thumb_"+gid);
	img.src = pictures[gid][current[gid]];
	current[gid] ++;
	if(current[gid] >= pictures[gid].length){
		current[gid] = 0;
	}
	if(!isStop[gid]){
		setTimeout("changeThumb("+gid+")", 1000);
	}else{
		isStop[gid] = false;
	}
}

function setStop(gid){
	isStop[gid] = true;
	current[gid] = 0;
	var img = document.getElementById("thumb_"+gid);
	img.src = pictures[gid][0];
}
</script>

<div class="gallery_list">
<h3 class="list_label"><?=$title?></h3>
<p><?=$description?></p>
<table>
<?php foreach($data as $key => $value):?>
	<?php $pictureName = array_values($value['picture']);?>
	<tr>
		<td class="tumbnail" valign="center" align="center" onmouseover="changeThumb(<?=$key?>)"" onmouseout="setStop(<?=$key?>)">
			<?php if(count($value['picture'])):?>
			<a href="<?=render_uri('gallery', $value)?>">
				<img src="<?=ROOT_URI?>files/gallery/<?=$key?>/thumb_<?=$pictureName[0]?>" border="0" id="thumb_<?=$key?>"/>
			</a>
			<?php endif?>
		</td>
		<td class="preview_info" valign="top">
			<h4><a href="<?=render_uri('gallery', $value)?>"><?=$value['topic']?></a></h4>
			<p>
				<?=tt('by')?> <?=render_profile_link($value['writer'])?> @ <?=$value['publish_time']?>
			</p>
			<p>
				 <?=tt('tags')?>
				<?php foreach($value['freetag'] as $tag):?>
					<a href="<?=SECTION_URI.Q?>gallery/tag/<?=$tag?>" class="tag"><?=$tag?></a>
				<?php endforeach?>
			</p>
			<p>
				<?=tt('read')?> <!--{ximple_read/gallery/<?=$key?>}--> |
				<?php if(isset($value['comment_no'])):?>
					<?=tt('comments')?> <?=$value['comment_no']?>
				<?php else:?>
					<?=tt('no comment')?>
				<?php endif?>
			</p>
		</td>
	</tr>
<?php endforeach?>
</table>
</div>
