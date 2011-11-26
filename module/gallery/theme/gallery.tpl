<?php if(!defined('XIMPLE_CMS')) die();?>

<div id="slide_container" style="background:url(<?=ROOT_URI?>/module/gallery/theme/bg.png);">
	<div id="slide_area">
		<img src="" id="slide_pic">
	</div>
</div>
<div id="slide_menu_container">
	<div id="slide_menu" style="background:url(<?=ROOT_URI?>/module/gallery/theme/toolbar.png) no-repeat;">
		<a href="" id="slide_back"><img src="<?=ROOT_URI?>/module/gallery/theme/back.png" border="0"></a>
		<a href="" id="slide_play"><img src="<?=ROOT_URI?>/module/gallery/theme/play.png" border="0"></a>
		<a href="" id="slide_next"><img src="<?=ROOT_URI?>/module/gallery/theme/next.png" border="0"></a>
		<a href="" id="slide_stop"><img src="<?=ROOT_URI?>/module/gallery/theme/stop.png" border="0"></a>
		<p id="slide_num"></p>
	</div>
</div>

<div class="gallery">
	<h3 class="title_label"><?=$topic?></h3>
	<table><tbody>
	<tr>
		<td>
			<?=$content?>
		</td>
		<?php if($show_avatar or $show_category):?>
			<td valign="top" style="text-align:center;">
				<?php if($show_avatar):?>
					<?=render_avatar($writer)?>
				<?php endif?>
				<?php if($show_category):?>
					<?=render_category_icon($category, 'gallery')?>
				<?php endif?>
			</td>
		<?php endif?>
	</tr>
	</tbody></table>
	<p class="gallery_info">
		<?=$publish_time?> | <?=tt('by')?> <?=render_profile_link($writer)?> | <?=tt('tags')?>
		<?php foreach($freetag as $key=>$tag):?>
			<a href="<?=SECTION_URI.Q?>gallery/tag/<?=$tag?>" class="tag"><?=$tag?></a>
		<?php endforeach?>
	</p>
	<p class="operation_bar">
		<span>read <!--{ximple_read/gallery/<?=$id?>}--></span>
		<?=render_print_link('gallery_print/'.$id)?>
		<?php if($editable):?>
			<?=render_operation_link('gallery_edit', $id, ('edit'))?>
			<?=render_operation_link('gallery_drop', $id, ('drop'))?>
		<?php endif?>
	</p>
</div>
<?php $pictureNumber=count($picture);?>
<?php $count=0;?>
<?php if($pictureNumber):?>
<table width="100%" id="gallery_picture"><tbody>
	<?php foreach($picture as $key => $value):?>
	<?php $count++;?>
	<?php if($count%3 == 1):?>
	<tr>
	<?php endif?>
		<td align="center">
			<a href="<?=ROOT_URI?>files/gallery/<?=$id?>/<?=$value?>" class="slide_link">
				<img src="<?=ROOT_URI?>files/gallery/<?=$id?>/thumb_<?=$value?>" alt="<?=$value?>" border="0"/>
			</a>
		<?php if($editable):?>
			<a href="<?=SECTION_URI.Q?>gallery_picture_drop/<?=$key?>/gallery/<?=$id?>">[<?=tt('drop')?>]</a>
		<?php endif?>
		</td>
	<?php if($count%3 == 0 or $count == $pictureNumber):?>
	</tr>
	<?php endif?>
	<?php endforeach?>
</tbody></table>
<?php endif?>
<script type="text/javascript">
	var currentLink = 0;
	var urlArray = new Array();
	var delayTime = 5000;
	var changeTime = 1000;
	var breakPlay = true;
	
	$(".slide_link").click(
		function(){
			currentSlide = this;
			initSide();
			$("#slide_container").show("drop", { direction: "up" }, 1500);
			$("#slide_area").show();
			$("#slide_menu_container").show();
			$("#slide_pic").attr('src', $(this).attr('href'));
			var m = $('.slide_link').length;
			var slide_link;
			for(var i=0;i < m;i++){
				slide_link = $('.slide_link:eq('+i+')');
				urlArray[i] = slide_link.attr('href');
				if(slide_link.attr('href') == $(this).attr('href')){
					currentLink=i;
				};
			}
			writeLabel();
			return false;
		}
	);
	$("#slide_stop").click(
		function(){
			$("#slide_menu_container").hide();
			$("#slide_area").hide();
			currentLink = 0;
			$("#slide_play img").attr('src', '<?=ROOT_URI?>/module/gallery/theme/play.png');
			breakPlay = true;
			$("#slide_container").hide("drop", { direction: "up" }, 1500);
			return false;
		}
	);
	$("#slide_play").click(
		function(){
			if(breakPlay){
				$("#slide_play img").attr('src', '<?=ROOT_URI?>/module/gallery/theme/pause.png');
				breakPlay = false;
				nextSlide(true);
			}else{
				$("#slide_play img").attr('src', '<?=ROOT_URI?>/module/gallery/theme/play.png');
				breakPlay = true;
			}
			return false;
		}
	)
	$("#slide_back").click(
		function(){
			if(currentLink > 0){
				currentLink--;
				breakPlay = true;
				$("#slide_play img").attr('src', '<?=ROOT_URI?>/module/gallery/theme/play.png');
				$("#slide_pic").attr('src', urlArray[currentLink]);
				writeLabel();
			}
			return false;
		}
	);
	$("#slide_next").click(
		function(){
			return nextSlide(false);
		}
	);
	
	function nextSlide(is_play){
		if(currentLink < urlArray.length -1){
			if(!(breakPlay && is_play)){
				currentLink ++;
				$("#slide_pic").attr('src', urlArray[currentLink]);
				writeLabel();
			}
			if(!is_play){
				breakPlay = true;
				$("#slide_play img").attr('src', '<?=ROOT_URI?>/module/gallery/theme/play.png');
			}
			if(is_play && ! breakPlay){
				setTimeout("nextSlide(true)", delayTime);
			}
		}else if(is_play){
			$("#slide_play img").attr('src', '<?=ROOT_URI?>/module/gallery/theme/play.png');
		}
		return false;
	}
	
	function initSide(){
		$("#slide_container").height($(document).height()+100);
		$("#slide_container").width($(document).width());
		$("#slide_area").css('top', $(window).scrollTop());
		$("#slide_menu_container").css('top', $(window).scrollTop());
		$("#slide_menu_container").width($(document).width());
	}
	
	function writeLabel(){
		$("#slide_num").html("pic "+(currentLink+1)+"/"+urlArray.length );
	}
	
</script>
<p>&#160;</p>
<?=$picture_form?>
