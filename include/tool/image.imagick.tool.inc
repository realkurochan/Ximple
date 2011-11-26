<?php
///	@author Programmer Pairoj Poomkhajorn
///	Date		2011/08/09
///	Non debug version

/// Check size of image, which is not allowed to exceed the limit.
/// Error notification will occur, if image size exceed the limit.
/// @param $file : array of file information.	
/// @param $path : localtion of the given image.
/// @param $maxwidth : given maximum width, which should not be exceeded.
function image_check_max_width($file, $path, $maxwidth){
	$im = image_get_src($file, $path);
	if($im){
		$src_w = $im->getImageWidth();
		$src_h = $im->getImageHeight();
		if($src_w >= $maxwidth){
			$dst_w = $maxwidth;
			$dst_h = ceil($maxwidth/$src_w * $src_h);

			$im->thumbnailImage($dst_w, $dst_h, true);
			$im->writeImage($path);
			notify_add_info(tt('Image has been automatically resized.'));
		}
	}
}
/// Crop image to square format.
/// @param $file : array of file information.
/// @param $path : localtion of the given image.
/// @param size : given size to crop.
function image_square_crop($file, $path, $size){
	$im = image_get_src($file, $path);
	if($im){
		$src_size = min($im->getImageWidth(),$im->getImageHeight());
		$im->cropImage($src_size, $src_size, 0, 0);
		$im->thumbnailImage($size, $size, true);
		$im->writeImage($path);
	}
}

/// Generate thumbnail for given image.
/// @param $file : array of file information.
/// @param $path : localtion of the given image.
/// @param $thumbpath : location of image thumbnail.
function image_generate_thumb($file, $path, $thumbpath){
	$im = image_get_src($file, $path);
	if($im){
		$src_w = $im->getImageWidth();
		$src_h = $im->getImageHeight();
		if($src_w >= $src_h){
			$dst_h = 100;
			$dst_w = ($src_w/$src_h)*100;
		}else{
			$dst_w = 100;
			$dst_h = ($src_h/$src_w)*100;
		}
		$im->cropImage($src_w, $src_h,0,0);
		$im->thumbnailImage($dst_w, $dst_h);
		$im->writeImage($thumbpath);
	}
}

/// Check the given file : Is it a image file?
/// @param $file : array of file information.
/// @param $path : localtion of the given image.
function image_get_src($file, $path){
	if(is_file($path)){
		if(substr($file['type'], 0, 5)  == "image") {
			$im = new Imagick();
			$im->readImage($path);
			return $im;
		}else{
			notify_add_error(tt('Only .png, .jpeg and .jpg are allowed to be uploaded.'));
			return null;
		}
	}
}

function image_get_size($path){
	$im = new Imagick();
	$im->readImage($path);
	return array(
		$im->getImageWidth(),
		$im->getImageHeight(),
	);
}


/// Generate gradient image.
/// Source http://www.bradshawenterprises.com/blog/2010/dynamically-drawing-gradients-with-php/
/// @param $width : image width
/// @param $height : image height
/// @param $startColor : start color for gradient
/// @param $endColor : end color for gradient
function image_generate_gradient($width, $height, $startColor, $endColor) {
	$final=imagecreatetruecolor($width,$height);
 
	$r=($startColor >> 16) & 0xFF;
	$g=($startColor >> 8) & 0xFF;
	$b=($startColor) & 0xFF;
 
	if ($height==0) {
		imagefill($final,0,0,imagecolorallocate($final,$r,$g,$b));
	} else {
 
		$endR=($endColor >> 16) & 0xFF;
		$endG=($endColor >> 8) & 0xFF;
		$endB=($endColor) & 0xFF;
 
		$incR=($endR-$r);
		$incG=($endG-$g);
		$incB=($endB-$b);
 
		$absDisR=abs($incR);
		$absDisG=abs($incG);
		$absDisB=abs($incB);
 
		if ($absDisR>$absDisG) {
			if ($absDisR>$absDisB) {
				$distance=$absDisR;
			} else {
				$distance=$absDisB;
			}
		} else if ($absDisG>$absDisB) {
			$distance=$absDisG;
		} else {
			$distance=$absDisB;
		}
 
		if ($distance==0) {
			imagefill($final,0,0,imagecolorallocate($final,$r,$g,$b));
		} else {
			if ($distance>$height) $distance=$height;
 
			$sliver=imagecreatetruecolor(1,$distance);
			$sliverMax=$distance-1;
 
			$incR/=$sliverMax;
			$incG/=$sliverMax;
			$incB/=$sliverMax;
 
			$style=array();
 
			for ($t=0; $t<$distance; $t++) {
				$style[$t]=imagecolorallocate($sliver,$r,$g,$b);
				$r+=$incR;
				$g+=$incG;
				$b+=$incB;
			}
 
			imagesetstyle($sliver,$style);
			imageline($sliver,0,0,0,$sliverMax,IMG_COLOR_STYLED);
			imagecopyresized(
				$final,$sliver,
				0,0,0,0,
				$width,$height,1,$distance
			);
			imagedestroy($sliver);
		}
	}
	return $final;
}
?>

