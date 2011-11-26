<?php if(!defined('XIMPLE_CMS')) die();?>
	<div class="list">
	<h3 class="list_label"><?=$title?></h3>
	<p><b>Used Space : <?=$user_used_space?>, Free Space : <?=$free_space?></b></p>
	<p><b>Path ::: </b><?=$dir_nav?></p>
	<p><img src="<?=ROOT_URI?>files/icon/help.png" border="0" alt="help"/> Click on type icon for file operation.</p>
	<?php $options=''?>
	<?php $j=0?>
	<table width="100%"><tbody>
		<tr>
			<th width="20px"></th>
			<th width="60px">
				<a href="<?=$uri?>&amp;order=type_desc">&lt;</a>
					<?=tt('type')?>
				<a href="<?=$uri?>&amp;order=type">&gt;</a>
			</th>
			<th>
				<a href="<?=$uri?>&amp;order=name_desc">&lt;</a>
					<?=tt('name')?>
				<a href="<?=$uri?>&amp;order=name">&gt;</a>
			</th>
			<th width="70px">
				<a href="<?=$uri?>&amp;order=time_desc">&lt;</a>
					<?=tt('time')?>
				<a href="<?=$uri?>&amp;order=time">&gt;</a>
			</th>
			<th width="60px">
				<a href="<?=$uri?>&amp;order=size_desc">&lt;</a>
					<?=tt('size')?>
				<a href="<?=$uri?>&amp;order=size">&gt;</a>
			</th>
		</tr>
		<?php for($i=0;$i<2;$i++):?>
			<?php foreach($filelist as $key => $value): ?>
				<?php if( ($value['is_dir'] and $i==0 ) or (!$value['is_dir'] and $i==1 )):?>
				<tr>
					<td align="center">
						<input type="checkbox" class="file_check" name="<?=$key?>" onchange="checkFile(this,'<?=$dir?><?=$key?>')" value="<?=$dir?><?=$key?>"/>
					</td>
					<td align="center">
						<a href="javascript:showOption(<?=$j?>)" title="copy" id="option_icon_<?=$j?>">
							<img src="<?=$value['icon']?>" border="0" alt="" title="<?=$value['type']?>"/>
						</a>
					</td>
					<td>
						<?=$value['href']?>
					</td>
					<td align="center">
						<?=$value['time']?>
					</td>
					<td align="center">
						<?=$value['size']?>
					</td>
					<?php ob_start();?>
					<div id="option_<?=$j?>" style="display:none;" class="operation_option">
						<a href="javascript:renameDialog('<?=$key?>')" title="rename" style="color:#222;">
							<img src="<?=ROOT_URI?>files/icon/edit.png" border="0" alt="rename"/>
							<?=tt('rename')?>
						</a>
						<a href="javascript:cutFile('<?=$dir?><?=$key?>')" title="cut" style="color:#222;">
							<img src="<?=ROOT_URI?>files/icon/editcut.png" border="0" alt="cut"/>
							<?=tt('cut')?>
						</a>
						<a href="javascript:copyFile('<?=$dir?><?=$key?>')" title="copy" style="color:#222;">
							<img src="<?=ROOT_URI?>files/icon/editcopy.png" border="0" alt="copy"/>
							<?=tt('copy')?>
						</a>
						<a href="<?=SECTION_URI?>?dir=<?=$dir?>&amp;file=<?=$key?>&amp;mode=file_drop&amp;next_mode=<?=$mode?>" title="delete" style="color:#222;">
							<img src="<?=ROOT_URI?>files/icon/stop.png" border="0" alt="delete"/>
							<?=tt('delete')?>
						</a>
						<a href="javascript:chmodDialog('<?=$key?>', <?=$value['permission']?>)" title="change permission" style="color:#222;">
							<img src="<?=ROOT_URI?>files/icon/lock.png" border="0" alt="permission"/>
							<?=tt('change permission')?>
						</a>
						<?php if(!$value['is_dir']):?>
							<span style="color:#222;">
								<img src="<?=ROOT_URI?>files/icon/html.png" border="0" alt="url"/>
								<?=tt('URI').' : '.$value['uri']?>
							</span>
						<?php endif?>
					</div>
					<?php $options .= ob_get_contents();?>
					<?php ob_end_clean();?>
				</tr>
				<?php endif?>
				<?php $j++?>
			<?php endforeach?>
		<?php endfor?>
	</tbody></table>
	</div>
	<?=$options?>
	<div class="form">
	
	<a href="javascript:checkAll()">check all</a> |
	<a href="javascript:checkNone()">check none</a>
	
	<strong style="padding-left:20px;">Operation to checked files</strong>
	
	<a href="javascript:cutfileList()" title="cut">
		<img src="<?=ROOT_URI?>files/icon/editcut.png" border="0" alt="cut" style="margin-right:15px;margin-left:15px;"/>
	</a>
	
	<a href="javascript:copyfileList()" title="copy">
		<img src="<?=ROOT_URI?>files/icon/editcopy.png" border="0" alt="copy" style="margin-right:15px;"/>
	</a>
	
	<a href="javascript:dropfileList()" title="delete">
		<img src="<?=ROOT_URI?>files/icon/stop.png" border="0" alt="delete" style="margin-right:15px;"/>
	</a>
	
	<h3 onclick="hideSub('copy_list')" class="config_head">Files to copy</h3>
	
	<div id="copy_list" class="subform" style="display:none;">
		<?=$file_copy_list?> <a href="javascript:clearCopy()"><b><?=tt('clear files to copy')?></b></a>
	</div>
	
	<h3 onclick="hideSub('cut_list')" class="config_head">Files to cut</h3>
	<div id="cut_list" class="subform" style="display:none;">
		<?=$file_cut_list?> <a href="javascript:clearCut()"><b><?=tt('clear files to cut')?></b></a>
	</div>
	
	<h3 onclick="hideSub('create_folder')" class="config_head">Create Folder</h3>
	<div id="create_folder" class="subform" style="display:none;">
		<form action="<?=SECTION_URI?>?mode=file_create_dir&amp;next_mode=<?=$mode?>&amp;dir=<?=$dir?>" method="post" enctype="multipart/form-data">
			<input name="new_dir" size="40"/>
			<input type="submit" name="submit" value="create folder"/>
		</form>	
	</div>
	
	<h3 onclick="hideSub('upload_file')" class="config_head">Upload File</h3>
	<div id="upload_file" class="subform" style="display:none;">
		<form action="<?=SECTION_URI?>?mode=file_upload&amp;next_mode=<?=$mode?>&amp;dir=<?=$dir?>" method="post" enctype="multipart/form-data">
			<div id="file_grid">
				<p><input name="file[]" size="40" type="file" /></p>
			</div>
			<p>
				<a href="#" onclick="return addFileRow();">
					<img src="<?=ROOT_URI?>files/icon/add.png" border="0"/>
					<?=tt('upload more file')?>
				</a>
			</p>
			<input name="submit" type="submit" value="upload file" />
		</form>	
	</div>
	</div>
	
	<div id="rename_dialog" title="<?=tt('File Rename')?>" style="text-align:left;">
		<p><label id="rename_head"></label></p>
		<form action="<?=SECTION_URI?>?mode=file_rename&amp;next_mode=<?=$mode?>" method="post">
			<input type="hidden" name="dir" value="<?=$dir?>" />
			<input type="hidden" name="file" id="rename_file_name" value=""/>
			<input size="40" name="new_name" />
			<input type="submit" value="<?=tt('rename')?>"/>
		</form>
	</div>
	
	<div id="image_dialog" title="<?=tt('Show Image')?>">
		<p id="image_label" style="font-size:10px;color:gray;"></p>
		<p align="center">
			<img src="" id="image" border="0"/>
		</p>
	</div>
	
	<div id="url_dialog" title="<?=tt('File URL')?>"></div>
	
	<div id="chmod_dialog" title="<?=tt('Permission Change Mode')?>" style="text-align:left;" class="list">
		<p><label id="chmod_head"></label></p>
		<p><label id="chmod_owner"></label></p>
		<form action="<?=SECTION_URI?>?mode=file_chmod&amp;next_mode=<?=$mode?>" method="post">
			<input type="hidden" name="dir" value="<?=$dir?>" />
			<input type="hidden" name="file" id="chmod_file_name" value="" />
			<table width="100%"><tbody>
				<tr>
					<td align="center"></td>
					<td align="center">Read</td>
					<td align="center">Write</td>
					<td align="center">Excute</td>
				</tr>
				<tr>
					<td><?=tt('Owner')?></td>
					<td align="center"><input type="checkbox" name="or" id="or" value="400"/></td>
					<td align="center"><input type="checkbox" name="ow" id="ow" value="200"/></td>
					<td align="center"><input type="checkbox" name="ox" id="ox" value="100"/></td>
				</tr>
				<tr>
					<td><?=tt('Group')?></td>
					<td align="center"><input type="checkbox" name="gr" id="gr" value="40"/></td>
					<td align="center"><input type="checkbox" name="gw" id="gw" value="20"/></td>
					<td align="center"><input type="checkbox" name="gx" id="gx" value="10"/></td>
				</tr>
				<tr>
					<td><?=tt('Other')?></td>
					<td align="center"><input type="checkbox" name="ar" id="ar" value="4"/></td>
					<td align="center"><input type="checkbox" name="aw" id="aw" value="2"/></td>
					<td align="center"><input type="checkbox" name="ax" id="ax" value="1"/></td>
				</tr>
			</tbody></table>
			<br />
			<input type="submit" value="<?=tt('Change Permission')?>"/>
		</form>
	</div>
	
	<script type="text/javascript">
	
	function showImage(src){
		var img = new Image();
		var w;
		var h;
		img.src = src;
		
		if(img.width+150 < 320) w= 320;
		else w = img.width+150;
		if(img.height+150 < 250) h=250;
		else h = img.height+150;
		
		$('#image_label').html(src);
		$('#image').attr('src', src);
		$('#image_dialog').dialog('option', 'width', w);
		$('#image_dialog').dialog('option', 'height', h);
		$('#image_dialog').dialog('open');
	}
	
	function renameDialog(fname){
		$('.operation_option').hide();
		$('#rename_file_name').attr('value', fname);
		$('#rename_head').html("<?=tt('File Rename')?> : "+fname);
		$('#rename_dialog').dialog('open');
	}
	
	function urlDialog(URI){
		$('#url_dialog').html('<h4>'+URI+'</h4>');
		$('#url_dialog').dialog('open');
	}
	
	function chmodDialog(fname, owner, group, or, ow, ox, gr, gw, gx, ar, aw, ax){
		$('.operation_option').hide();
		$('#chmod_file_name').attr('value', fname);
		$('#chmod_head').html("<?=tt('Permission Change Mode')?> : "+fname);
		$('#chmod_owner').html("<?=tt('Owner')?> : "+owner+" | <?=tt('Group')?> : "+group);
		if(or == '1') $('#or').attr('checked', 'checked');
		if(ow == '1') $('#ow').attr('checked', 'checked');
		if(ox == '1') $('#ox').attr('checked', 'checked');
		if(gr == '1') $('#gr').attr('checked', 'checked');
		if(gw == '1') $('#gw').attr('checked', 'checked');
		if(gx == '1') $('#gx').attr('checked', 'checked');
		if(ar == '1') $('#ar').attr('checked', 'checked');
		if(aw == '1') $('#aw').attr('checked', 'checked');
		if(ax == '1') $('#ax').attr('checked', 'checked');
		$('#chmod_dialog').dialog('open');
	}
	
	$(function(){
		$('#rename_dialog').dialog({
			bgiframe: true,
			autoOpen: false,
			height: 120,
			width:500,
			modal: true
		});
		$('#chmod_dialog').dialog({
			bgiframe: true,
			autoOpen: false,
			height: 400,
			width:500,
			modal: true
		});
		$('#image_dialog').dialog({
			bgiframe: true,
			autoOpen: false
		});
		$('#url_dialog').dialog({
			bgiframe: true,
			autoOpen: false
		});
	});
	
	function hideSub(sub_conf){
		$('#'+sub_conf).toggle('blind');
	}
	
	var fileList = new Array();
	
	function checkFile(checker, file_name){
		if(checker.checked){
			fileList[fileList.length] = file_name;
		}
		else{
			for(var i=0;i< fileList.length;i++){
				if(fileList[i] == file_name){
					fileList[i]="";
					break;
				}
			}
		}
	}
	
	function checkAll(){
		var m = $('.file_check').length;
		for(var i=0; i< m ; i++){	
			var f_check = $('.file_check:eq('+i+')');
			f_check.attr("checked", "checked");
			fileList[i] = f_check.attr('value');
		}
	}
	
	function checkNone(){
		var m = $('.file_check').length;
		for(var i=0; i< m ; i++){	
			var f_check = $('.file_check:eq('+i+')');
			f_check.attr("checked", "");
			fileList[i] = "";
		}
	}
	
	function dropfileList(){
		var fileListURL = "";
		for(var i=0;i< fileList.length;i++){
			fileListURL += "file_"+i+"="+fileList[i]+"&";
		}
		window.location.href="<?=SECTION_URI?>?mode=file_drop_multiple&next_mode=<?=$mode?>&dir=<?=$dir?>&"+fileListURL;
	}
	
	function copyFile(fileName){
		$.get("<?=SECTION_URI?>?mode=file_copy&file="+fileName, function(data){
			$("#copy_list").html(data);
		});
		alert('<?=tt('The selected file is appended to file list for copying. Select target directory and click pase icon.')?>');
		$('.operation_option').hide();
	}
	
	function copyfileList(){
		var fileListURL = "";
		for(var i=0;i< fileList.length;i++){
			fileListURL += "file_"+i+"="+fileList[i]+"&";
		}
		$.get("<?=SECTION_URI?>?mode=file_copy&"+fileListURL, function(data){
			$("#copy_list").html(data);
		});
		alert('<?=tt('The selected file is appended to file list for copying. Select target directory and click pase icon.')?>');
	}
	
	function clearCopy(){
		$.get("<?=SECTION_URI?>?mode=file_copy&clear=true", function(data){
			$("#copy_list").html(data);
		});
	}
	
	function cutFile(fileName){
		$.get("<?=SECTION_URI?>?mode=file_cut&file="+fileName, function(data){
			$("#cut_list").html(data);
		});
		alert('<?=tt('The selected file is appended to file list for cutting. Select target directory and click pase icon.')?>');
		$('.operation_option').hide();
	}
	
	function cutfileList(){
		var fileListURL = "";
		for(var i=0;i< fileList.length;i++){
			fileListURL += "file_"+i+"="+fileList[i]+"&";
		}
		$.get("<?=SECTION_URI?>?mode=file_cut&"+fileListURL, function(data){
			$("#cut_list").html(data);
		});
		alert('<?=tt('The selected file is appended to file list for cutting. Select target directory and click pase icon.')?>');
	}
	
	function clearCut(){
		$.get("<?=SECTION_URI?>?mode=file_cut&clear=true", function(data){
			$("#cut_list").html(data);
		});
	}
	
	function showOption(number){
		var pos = $('#option_icon_'+number).offset();
		$('.operation_option').each(function(){
			if($(this).attr('id') !=  'option_'+number) $(this).hide();
		});
		$($('#option_'+number)).css({
			 top: pos.top+16, left: pos.left
		});
		$('#option_'+number).toggle('blind');
	}
	
	function addFileRow(){
		$('#file_grid').append(' <p><input name="file[]" size="40" type="file"/></p>');
		return false;
	}
	</script>
