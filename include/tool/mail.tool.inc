<?php
/// @file
/// This file provides functions for email tools by using
/// PHPMailer (http://phpmailer.codeworxtech.com/)
///
/// @author Kittipong Piyawanno <k.piyawanno@gmail.com>

include(INCLUDE_PATH.'external/phpmailer/class.phpmailer.php');

/// Send mail with SMTP server.
/// @param $receiver   : email-address of receiver.
/// @param $subject    : subject of the email.
/// @param $body       : body of the email.
/// @param $attachment : file location of attachment.
/// @param $is_html    : Does body of email contain HTML?
function mail_send($receiver, $subject, $body, $is_html=false, $attachment=null){
	if(!defined('ENABLE_MAIL') or !ENABLE_MAIL) return null;
	$mail = new PHPMailer();
	$mail->IsSMTP();
	
	$server_info = split(":", SMTP_SERVER);
	$mail->Host = $server_info[0];
	if(count($server_info) > 1) $mail->Port = $server_info[1];

	if(strlen(SMTP_LOGIN) == 0){
		$mail->SMTPAuth = false;
	}else{
		$mail->SMTPAuth = true;
		$mail->Username = SMTP_LOGIN;
		$mail->Password = SMTP_PASSWD;
	}

	$mail->From = SITE_MAIL;
	$mail->FromName = cc('section_name');
	$mail->CharSet = 'UTF-8';
	$mail->WordWrap = 150;
	$mail->IsHTML($is_html);
	$mail->Subject = $subject;
	$mail->Body    = str_replace("\\\"", '"', stripslashes($body));
	$mail->Body    = str_replace("\\'", "'", $mail->Body);
	
	if($attachment != null){
		$mail->AddAttachment(ROOT_PATH.$attachment, basename($attachment));
	}
	
	$receiver_split = explode(',', $receiver);
	if(!is_array($receiver_split)) $receiver_split = array();
	$feedback = true;
	foreach($receiver_split as $key => $value){
		$value = trim($value);
		$mail->ClearAddresses();
		$mail->AddAddress($value);
		if($mail->Send()){
			if($feedback === true) $feedbak = $mail->ErrorInfo;
			else $feedbak .= ', '.$mail->ErrorInfo;
		}
	}
	
	return $feedback;
}
?>
