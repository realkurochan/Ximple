<form action="index.php" method="get">
	<input type="hidden" name="mode" value="step<?=$next_step?>"/>
	<input type="button" value="<?=tt('Back')?>" onclick="back()">
	<input type="submit" value="<?=tt('Next')?>" <?=$next_enable?> >
	<?=$try?>
</form>
