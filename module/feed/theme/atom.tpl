<?php if(!defined('XIMPLE_CMS')) die();?>
<?='<?xml version="1.0" encoding="UTF-8"?>'?>
<feed xmlns="http://www.w3.org/2005/Atom" xml:lang="<?=cc('lang')?>" xmlns:thr="http://purl.org/syndication/thread/1.0">
	<title><?=cc('section_name')?></title>
	<subtitle><?=cc('slogan')?></subtitle>
	<link rel="self" type="application/atom+xml" href="<?=ROOT_URI?>files/xml/<?=SECTION?>/atom.xml"/>
	<id><?=SECTION_URI?></id>
	<updated><?=$data[0]['update_time']?></updated>
	<?php foreach($data as $key => $value):?>
	<entry>
		<title><?=$value['topic']?></title>
		<link rel="alternate" type="text/html" href="<?=render_uri($value['mode'], $value)?>"/>
		<id><?=render_uri($value['mode'], $value)?></id>
		<published><?=$value['publish_time']?></published>
		<pubDate><?=$value['publish_time']?></pubDate>
		<updated><?=$value['update_time']?></updated>
		<author><name><?=$value['writer']?></name></author>
		<?php foreach($value['freetag'] as $tag):?>
			<category term="<?=$tag?>"/>
		<?php endforeach?>
		<summary type="html">
			<![CDATA[
				<?=$value['summary']?>
			]]>
		</summary>
		<content type="html">
			<![CDATA[
				<?=$value['summary']?>
			]]>
		</content>
	</entry>
	<?php endforeach?>
</feed>
