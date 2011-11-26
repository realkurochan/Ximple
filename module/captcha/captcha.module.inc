<?php
/// @file
/// This file provides captcha module for antispaming
/// by using securimage(http://www.phpcaptcha.org).
/// The securimage functionality is enhanced with md5-, sha1-hashmap,
/// salt and timing for antispamming improvement.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>
/// @state complete.

include(MODULE_PATH.'captcha/securimage/securimage.php');
execute_add_function('captcha');
execute_add_function('captcha_check');

/// Generate captcha image.
/// Output of this function is a PNG-Image, hence the header should not be modified.
/// Use doc.php to use this function.
function captcha(){
	// You have 6 hours to use securimage.
	if( (time() - gg('gentime')) > cc('antispam_time')) die();
	$img = new Securimage();
	$img->salt = cc('antispam_salt');
	$img->charset = 'ABCDEFGHKLMNPRSTUVWYZ23456789';
	$img->generate_time = gg('gentime');
	$img->image_width  = 280;
	$img->image_height = 64;
	$img->font_size    = 24;
	$img->ttf_file = ROOT_PATH."module/captcha/securimage/AHGBold.ttf";
	$img->perturbation = 0.75;
	$img->image_bg_color = new Securimage_Color(0x0, 0x0, 0x0);
	$img->text_color = new Securimage_Color(0xff, 0xff, 0xff);
	$img->text_transparency_percentage = 10;
	$img->use_transparent_text = true;
	$img->text_angle_minimum = -10;
	$img->text_angle_maximum = 10;
	$img->num_lines = 4;
	$img->code_length = 6;
	$img->use_wordlist = false;
	$img->wordlist_file = MODULE_PATH.'captcha/securimage/words.txt';
	$img->line_color = new Securimage_Color(0xff, 0xaff, 0xff);
	$img->gd_font_file  = MODULE_PATH.'captcha/securimage/automatic.gdf';
	$img->show(MODULE_PATH.'captcha/securimage/bg.png');
	execute_set_document();
}

/// Ajax code-checking.
function captcha_check(){
	if( (time() - gg('gentime')) > cc('antispam_time')) die();
	$img = new securimage();
	$img->salt = cc('antispam_salt');
	$img->generate_time = gg('gentime');
	execute_set_ajax();
	if($img->check(gg('value'))) return '<code>'.$img->cryptcode.'</code>';
}
?>
