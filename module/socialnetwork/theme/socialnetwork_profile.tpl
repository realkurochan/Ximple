<?php if(!defined('XIMPLE_CMS')) die();?>
<?php $appearance = cc('socialnetwork_profile_appearance')?>
<script src="http://widgets.twimg.com/j/2/widget.js"></script>
<script>
new TWTR.Widget({
	version: 2,
	type: 'profile',
	rpp: <?=cc('socialnetwork_profile_number')?>,
	interval: 6000,
	width: <?=cc('socialnetwork_profile_width')?>,
	height: <?=cc('socialnetwork_profile_height')?>,
	theme: {
		shell: {
			background: '<?=cc('socialnetwork_profile_color_shell_background')?>',
			color: '<?=cc('socialnetwork_profile_color_shell_text')?>',
		},
		tweets: {
			background: '<?=cc('socialnetwork_profile_color_tweet_background')?>',
			color: '<?=cc('socialnetwork_profile_color_tweet_text')?>',
			links: '<?=cc('socialnetwork_profile_color_link')?>',
		}
	},
	features: {
		scrollbar: <?=in_array('scrollbar', $appearance) ?'true':'false'?>,
		loop: <?=in_array('loop', $appearance) ?'true':'false'?>,
		live: <?=in_array('live', $appearance) ?'true':'false'?>,
		hashtags: <?=in_array('hashtags', $appearance) ?'true':'false'?>,
		timestamp: <?=in_array('timestamp', $appearance) ?'true':'false'?>,
		avatars: <?=in_array('avatars', $appearance) ?'true':'false'?>,
		behavior: 'all'
	}
}).render().setUser('<?=cc('socialnetwork_profile_user_name')?>').start();
</script>
