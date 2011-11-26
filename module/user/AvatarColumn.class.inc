<?php
require_once(INCLUDE_PATH.'class/column/ImageColumn.class.inc');

class AvatarColumn extends ImageColumn{
	function AvatarColumn(){
		ImageColumn::ImageColumn(tt('avarta'), 'files/avatar/');
		$this->help = tt('Image must be only .png or .jpeg or .jpg.');
		$pattern = tt('Avatar will be automatically cropped to %dx%d and %dx%d.');
		$size = cc('avatar_size');
		$bigSize = cc('avatar_big_size');
		$this->help.= sprintf($pattern, $size, $size, $bigSize, $bigSize);
	}
	
	protected function checkImage(){
		$file = ff($this->name);
		if($file['size'] and is_file(ROOT_PATH.$this->data)){
			$avatarWidth = cc('avatar_size');
			if(empty($avatarWidth)) $avatarWidth = 64;
			$bigAvatarWidth = cc('avatar_big_size');
			if(empty($bigAvatarWidth)) $bigAvatarWidth = 128;
			
			$bigDBAvatarFileName = dirname(ROOT_PATH.$this->dataDB).'/big_'.basename($this->dataDB);
			if(is_file($bigDBAvatarFileName)) unlink($bigDBAvatarFileName);
			
			$bigAvatarFileName = dirname(ROOT_PATH.$this->data).'/big_'.basename($this->data);
			copy(ROOT_PATH.$this->data, $bigAvatarFileName);
			image_square_crop($file, $bigAvatarFileName, $bigAvatarWidth);
			image_square_crop($file, ROOT_PATH.$this->data, $avatarWidth);
		}
	}
}
?>
