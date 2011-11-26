<?php if(!defined('XIMPLE_CMS')) die();?>
<div style="float:left;">
	<form action="<?=SECTION_URI.Q?>" method="get">
		<input type="hidden" value="<?=$meta->mode?>" name="mode" />
		<select name="col">
		<?php foreach($col as $key => $value):?>
			<?php if($value == $search_col):?>
				<option value="<?=$value?>" selected="selected"><?=$label[$value]?></option>
			<?php else:?>
				<option value="<?=$value?>"><?=$label[$value]?></option>
			<?php endif?>
		<?php endforeach?>
		</select>
		<?php foreach($param as $key => $value):?>
			<input type="hidden" name="<?=$key?>" value="<?=$value?>"/>
		<?php endforeach?>
		<input style="width:200px;" name="search" value="<?=$search?>"/>
		<input type="submit" value="<?=tt('search')?>"/>
		<?php if(strlen($search)):?>
			<?php $mode=$meta->mode?>
			<?php foreach($param as $key => $value):?>
				<?php $mode.= '/'.$key.'/'.$value?>
			<?php endforeach?>
			<a href="<?=SECTION_URI.Q.$mode?>" style="margin-left:20px;"><?=tt('clear search')?></a>
		<?php endif?>
	</form>
</div>
<p style="clear:both;">
