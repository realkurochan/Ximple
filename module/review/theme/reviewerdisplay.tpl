<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="review">
	<b><?=tt('Review by : ')?></b>
	<?php foreach($review_data as $key => $value) :?>
		<h3 class="title_label"> reviewee : <?=$value['reviewee']?> </h3>
		<h3 class="title_label"> star : <?=$value['star']?> </h3>
		<h3 class="title_label"> star : <?=$value['createdate']?> </h3>
		<p> <?=$value['description']?> </p>
	<?php endforeach?>
</div>

