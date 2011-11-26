<?php
function array_sort($array, $on, $order=SORT_ASC){
	$new_array = array();
	$sortable_array = array();

	if(count($array) > 0){
		foreach($array as $k => $v){
			if(is_array($v)){
				foreach($v as $k2 => $v2){
					if($k2 == $on){
						$sortable_array[$k] = $v2;
					}
				}
			}else{
				$sortable_array[$k] = $v;
			}
		}

		switch($order){
			case SORT_ASC:
				asort($sortable_array);
			break;
			case SORT_DESC:
				arsort($sortable_array);
			break;
		}

		foreach($sortable_array as $k => $v){
			$new_array[$k] = $array[$k];
		}
	}

	return $new_array;
}
$announcedocument = array_sort($announcedocument, 'id', SORT_DESC);
$commanddocument = array_sort($commanddocument, 'id', SORT_DESC);
$externaldocument = array_sort($externaldocument, 'id', SORT_DESC);
$internaldocument = array_sort($internaldocument, 'id', SORT_DESC);
?>

<h3 class="title_label"><?=tt('Announce Document')?></h3>
<table width="100%">
	<tr>
		<td>	
			<a href=<?=SECTION_URI.Q.'documentgenerator_announcedocument_write'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/add.png" border="0" >
				Write new Document
			</a>
		</td>
		<td>
			<a href=<?=SECTION_URI.Q.'documentgenerator_announcedocument_contributor_list'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/folder.png" border="0" >
				List all Documents
			</a>
		</td>
	</tr>	
</table>
<table width="100%" border="1" cellpadding="8" cellspacing="0" style="border-collapse:collapse;"><tbody align="center">
	<tr>
		<th style="min-width:100px;">
			เรื่อง
		</th>
		<th colspan="6">
			operation
		</th>
	</tr>
	<?php 
		$i=0;
		foreach($announcedocument as $key => $value):
			$i=$i+1;
			if($i<=5):
	?>
	<tr>
		<td align="left">
			<a href="http://localhost/ximple/?/documentgenerator_announcedocument/<?=$value['id']?>"><?=$value['subject']?></a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_announcedocument_preview/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/module/documentgenerator/theme/preview.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_announcedocument_genpdf/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/files/icon/pdf.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_announcedocument_genodt/<?=$value['id']?>/">
			<img alt="generate ODT" src="http://localhost/ximple/module/documentgenerator/theme/odt.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_announcedocument_gendoc/<?=$value['id']?>/">
			<img alt="generate DOC" src="http://localhost/ximple/files/icon/word.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_announcedocument_edit/<?=$value['id']?>/">
			<img alt="edit" src="http://localhost/ximple/files/icon/edit.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_announcedocument_drop/<?=$value['id']?>/">
			<img alt="drop" src="http://localhost/ximple/files/icon/drop.png" border="0"/>
			</a>
		</td>
	</tr>
	<?php 
		endif;
		endforeach;
	?>
</tbody></table><br><br>

<h3 class="title_label"><?=tt('Command Document')?></h3>
<table width="100%">
	<tr>
		<td>	
			<a href=<?=SECTION_URI.Q.'documentgenerator_commanddocument_write'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/add.png" border="0" >
				Write new Document
			</a>
		</td>
		<td>
			<a href=<?=SECTION_URI.Q.'documentgenerator_commanddocument_contributor_list'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/folder.png" border="0" >
				List all Documents
			</a>
		</td>
	</tr>	
</table>
<table width="100%" border="1" cellpadding="8" cellspacing="0" style="border-collapse:collapse;"><tbody align="center">
	<tr>
		<th style="min-width:100px;">
			เรื่อง
		</th>
		<th colspan="6">
			operation
		</th>
	</tr>
	<?php 
		$i=0;
		foreach($commanddocument as $key => $value):
			$i=$i+1;
			if($i<=5):
	?>
	<tr>
		<td align="left">
			<a href="http://localhost/ximple/?/documentgenerator_commanddocument/<?=$value['id']?>"><?=$value['subject']?></a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_commanddocument_preview/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/module/documentgenerator/theme/preview.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_commanddocument_genpdf/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/files/icon/pdf.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_commanddocument_genodt/<?=$value['id']?>/">
			<img alt="generate ODT" src="http://localhost/ximple/module/documentgenerator/theme/odt.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_commanddocument_gendoc/<?=$value['id']?>/">
			<img alt="generate DOC" src="http://localhost/ximple/files/icon/word.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_commanddocument_edit/<?=$value['id']?>/">
			<img alt="edit" src="http://localhost/ximple/files/icon/edit.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_commanddocument_drop/<?=$value['id']?>/">
			<img alt="drop" src="http://localhost/ximple/files/icon/drop.png" border="0"/>
			</a>
		</td>
	</tr>
	<?php 
		endif;
		endforeach;
	?>
</tbody></table><br><br>

<h3 class="title_label"><?=tt('External Document')?></h3>
<table width="100%">
	<tr>
		<td>	
			<a href=<?=SECTION_URI.Q.'documentgenerator_externaldocument_write'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/add.png" border="0" >
				Write new Document
			</a>
		</td>
		<td>
			<a href=<?=SECTION_URI.Q.'documentgenerator_externaldocument_contributor_list'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/folder.png" border="0" >
				List all Documents
			</a>
		</td>
	</tr>	
</table>
<table width="100%" border="1" cellpadding="8" cellspacing="0" style="border-collapse:collapse;"><tbody align="center">
	<tr>
		<th style="min-width:100px;">
			เรื่อง
		</th>
		<th colspan="6">
			operation
		</th>
	</tr>
	<?php 
		$i=0;
		foreach($externaldocument as $key => $value):
			$i=$i+1;
			if($i<=5):
	?>
	<tr>
		<td align="left">
			<a href="http://localhost/ximple/?/documentgenerator_externaldocument/<?=$value['id']?>"><?=$value['subject']?></a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_externaldocument_preview/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/module/documentgenerator/theme/preview.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_externaldocument_genpdf/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/files/icon/pdf.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_externaldocument_genodt/<?=$value['id']?>/">
			<img alt="generate ODT" src="http://localhost/ximple/module/documentgenerator/theme/odt.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_externaldocument_gendoc/<?=$value['id']?>/">
			<img alt="generate DOC" src="http://localhost/ximple/files/icon/word.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_externaldocument_edit/<?=$value['id']?>/">
			<img alt="edit" src="http://localhost/ximple/files/icon/edit.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_externaldocument_drop/<?=$value['id']?>/">
			<img alt="drop" src="http://localhost/ximple/files/icon/drop.png" border="0"/>
			</a>
		</td>
	</tr>
	<?php 
		endif;
		endforeach;
	?>
</tbody></table><br><br>

<h3 class="title_label"><?=tt('Internal Document')?></h3>
<table width="100%">
	<tr>
		<td>	
			<a href=<?=SECTION_URI.Q.'documentgenerator_internaldocument_write'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/add.png" border="0" >
				Write new Document
			</a>
		</td>
		<td>
			<a href=<?=SECTION_URI.Q.'documentgenerator_internaldocument_contributor_list'?> class="write_icon">
				<img src="http://localhost/ximple/files/icon/folder.png" border="0" >
				List all Documents
			</a>
		</td>
	</tr>	
</table>
<table width="100%" border="1" cellpadding="8" cellspacing="0" style="border-collapse:collapse;"><tbody align="center">
	<tr>
		<th style="min-width:100px;">
			เรื่อง
		</th>
		<th colspan="6">
			operation
		</th>
	</tr>
	<?php 
		$i=0;
		foreach($internaldocument as $key => $value):
			$i=$i+1;
			if($i<=5):
	?>
	<tr>
		<td align="left">
			<a href="http://localhost/ximple/?/documentgenerator_internaldocument/<?=$value['id']?>"><?=$value['subject']?></a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_internaldocument_preview/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/module/documentgenerator/theme/preview.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_internaldocument_genpdf/<?=$value['id']?>/">
			<img alt="generate PDF" src="http://localhost/ximple/files/icon/pdf.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_internaldocument_genodt/<?=$value['id']?>/">
			<img alt="generate ODT" src="http://localhost/ximple/module/documentgenerator/theme/odt.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_internaldocument_gendoc/<?=$value['id']?>/">
			<img alt="generate DOC" src="http://localhost/ximple/files/icon/word.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_internaldocument_edit/<?=$value['id']?>/">
			<img alt="edit" src="http://localhost/ximple/files/icon/edit.png" border="0"/>
			</a>
		</td>
		<td style="width:20px;">
			<a href="http://localhost/ximple/?/documentgenerator_internaldocument_drop/<?=$value['id']?>/">
			<img alt="drop" src="http://localhost/ximple/files/icon/drop.png" border="0"/>
			</a>
		</td>
	</tr>
	<?php 
		endif;
		endforeach;
	?>
</tbody></table>
