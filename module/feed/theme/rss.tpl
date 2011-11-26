<?php if(!defined('XIMPLE_CMS')) die();?>
<?='<?xml version="1.0" encoding="UTF-8"?>'?>
<rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:wfw="http://wellformedweb.org/CommentAPI/">
<channel>
	<title><?=cc('section_name')?></title>
    <link><?=SECTION_URI?></link>
    <description><?=cc('slogan')?></description>
    <language><?=cc('lang')?></language>
    <pubDate><?=$data[0]['update_time']?></pubDate>
    <lastBuildDate><?=$data[0]['update_time']?></lastBuildDate>
    <generator>Ximple-0.9</generator>
    <webMaster><?=SITE_MAIL?></webMaster>
   <?php foreach($data as $key => $value):?>
    <item>
		<title><?=$value['topic']?></title>
		<dc:creator><?=$value['writer']?></dc:creator>
		<link><?=render_uri($value['mode'], $value)?></link>
		<pubDate><?=$value['publish_time']?></pubDate>
		<description>
			<![CDATA[<?=$value['summary']?>]]>
        </description>
        <?php foreach($value['freetag'] as $tag):?>
			<category><?=$tag?></category>
		<?php endforeach?>
	</item>
	<?php endforeach?>
</channel>
</rss>

