<?php if(!defined('XIMPLE_CMS')) die();?>
<script src="http://connect.facebook.net/en_US/all.js#xfbml=1"></script>
<?php $appearance = cc('socialnetwork_activity_feed_appearance')?>
<fb:activity
	site="<?=cc('socialnetwork_activity_feed_uri')?>"
	width="<?=cc('socialnetwork_activity_feed_width')?>"
	height="<?=cc('socialnetwork_activity_feed_height')?>"
	colorscheme="<?=cc('socialnetwork_activity_feed_color')?>"
	recommendations="<?=in_array('recommendations', $appearance) ?'true':'false'?>"
	header="<?=in_array('header', $appearance) ?'true':'false'?>">
</fb:activity>
