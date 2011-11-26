<?php if(!defined('XIMPLE_CMS')) die();?>
<?=$data['content']?>
<?php $module = $data['module'];?>
<p>
<?php if(cc($module.'_socialnetwork_like_type') != 'disable'):?>
	<script src="http://connect.facebook.net/en_US/all.js#xfbml=1"></script>
	<fb:like
		href="<?=render_uri($module, $data)?>"
		layout="<?=cc($module.'_socialnetwork_like_type')?>"
		colorscheme="<?=cc($module.'_socialnetwork_like_color')?>"
		width="<?=cc($module.'_socialnetwork_like_width')?>"
	>
	</fb:like>
<?php endif?>
</p>
<p>
	<?php if(cc($module.'_socialnetwork_tweet_type') != 'disable'):?>
		<a href="<?=render_uri($module, $data)?>" class="twitter-share-button" data-count="<?=cc($module.'_socialnetwork_tweet_type')?>">Tweet</a>
		<script type="text/javascript" src="http://platform.twitter.com/widgets.js"></script>
	<?php endif?>
</p>
