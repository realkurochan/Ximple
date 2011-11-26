<?php if(!defined('XIMPLE_CMS')) die();?>
<ul id="menu">
<?php foreach($data as $key => $item):?>
	<?php if($item['is_called_module']):?>
		<li><a href="<?=$item['uri']?>" id="active_page" rel="submenu_<?=$key?>"><?=$item['title']?></a></li>
	<?php else:?>
		<li><a href="<?=$item['uri']?>" rel="submenu_<?=$key?>"><?=$item['title']?></a></li>
	<?php endif?>
<?php endforeach?>
</ul>
<?php foreach($data as $key => $item):?>
	<?php if(!empty($item['widget'])):?>
		<div class="submenu" id="submenu_<?=$key?>">
			<?=$item['widget']?>
		</div>
	<?php endif?>
<?php endforeach?>
<script>
	var hideLock = false;
	function hideSubMenu(){
		if(!hideLock) $('.submenu:visible').hide();
	}
	
	$("#menu li a").mouseover(function (){
		$('.submenu:visible').hide();
		var pos = $(this).offset();
		var height = $(this).parent().height();
		$('#'+$(this).attr('rel')).css({
			 top: pos.top + height, left: pos.left
		});
		$('#'+$(this).attr('rel')+':hidden').show('blind');
		hideLock = true;
	});
	
	$("#menu li a, .submenu").mouseout(function (){
		hideLock = false;
		setTimeout("hideSubMenu()", 1000);
	});
	
	$(".submenu").mouseover(function (){
		hideLock = true;
	});
	
</script>

