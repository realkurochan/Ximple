<?php if(!defined('XIMPLE_CMS')) die();?>
<div class="review">
	<h3 class="title_label"> Total score of <?=$reviewee_name?> : <?=$total?> </h3>
	<b><a href="review_write/<?=$reviewee?>">new review</a></b>
	<b><?=tt('Review for reviewee: ')?></b>
	<?php foreach($review_data as $key => $value) :?>
		<h3 class="title_label"> review by : <?=$value['reviewerLink']?> </h3>
		<h3 class="title_label"> star : <?=$value['star']?> </h3>
		<h3 class="title_label"> create date : <?=$value['createdate']?> </h3>
		<?php if($value['canReview']) :?>
			<a href="review_rating/<?=$reviewee?>/reviewId/<?=$value['id']?>/scorer/<?=$reviewer?>/point/1">+1</a>
			<a href="review_rating/<?=$reviewee?>/reviewId/<?=$value['id']?>/scorer/<?=$reviewer?>/point/-1">-1</a>
		<?php endif?>
		<h3 class="title_label"> point1 : <?=$value['point1']?> </h3>
		<h3 class="title_label"> point2 : <?=$value['point2']?> </h3>
		<p> <?=$value['description']?> </p>
		<b> ---------------------------- </b>
	<?php endforeach?>
</div>

