<?php
/// @file
/// This file provides form verification with captcha.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>

/// Check input code.
/// @param $gentime : captcha generate time.
/// @param $code : user given code.
/// @param $parity : generated parity check code.
/// @return TRUE, if captcha code is correct.
function captcha_check($gentime, $code, $parity){
	io_append_file(FILES_PATH.'captchae_log.txt', $gentime.', '.$code.', '.$parity."\n");
	if( (time() - $gentime) > cc('antispam_time')) return false;
	$salt  = strtolower(trim(cc('antispam_salt')));
	$crypt = sha1(md5(strtolower($code).$gentime).$salt);
	$success = $crypt == substr($parity, 6, -7);
	if(!$success) io_write_log(tt('Wrong Captcha code.'), 'captcha');
	return $success;
}

?>
