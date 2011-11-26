<?php if(!defined('XIMPLE_CMS')) die();?>
<?php $appearance = cc('socialnetwork_faves_appearance')?>
<script src="http://widgets.twimg.com/j/2/widget.js"></script>
<script>
new TWTR.Widget({
	version: 2,
	type: 'faves',
	rpp: <?=cc('socialnetwork_faves_number')?>,
	interval: 6000,
	width: <?=cc('socialnetwork_faves_width')?>,
	height: <?=cc('socialnetwork_faves_height')?>,
	title: '<?=cc('socialnetwork_faves_title')?>',
	subject: '<?=cc('socialnetwork_faves_caption')?>',
	theme: {
		shell: {
			background: '<?=cc('socialnetwork_faves_color_shell_background')?>',
			color: '<?=cc('socialnetwork_faves_color_shell_text')?>',
		},
		tweets: {
			background: '<?=cc('socialnetwork_faves_color_tweet_background')?>',
			color: '<?=cc('socialnetwork_faves_color_tweet_text')?>',
			links: '<?=cc('socialnetwork_faves_color_link')?>',
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
}).render().setUser('<?=cc('socialnetwork_faves_user_name')?>').start();
</script>
