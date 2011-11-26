<?php
execute_add_function('dbbackup');
execute_add_function('dbbackup_config');
execute_add_function('dbbackup_config_save');

function dbbackup(){
	if(cron_check_process()){
		require_once(INCLUDE_PATH.'tool/dump.tool.inc');
		$file_name = dump();
		if(cc('dbbackup_send_mail')){
			$receiver = cc('dbbackup_mail_list');
			$subject = tt('Database Backup');
			$message = tt('Database has been automatically backed up. ');
			$message.= tt('In attachment you can find the back-up file.');
			require_once(INCLUDE_PATH.'tool/mail.tool.inc');
			mail_send($receiver, $subject, $message, true, $file_name);
		}
	}
}

function dbbackup_config($params, $result){
	$cron = cc('cron');
	$interval = Model::timeInterval(tt('backup interval'));
	$interval->isMinute = false;
	$interval->isSecond = false;
	$result[tt('Database Backup')] = array(
		'dbbackup_send_mail' => MODEL::enable(tt('send backup to related emails')),
		'dbbackup_mail_list' => MODEL::plain(tt('emails of related persons'), tt('use comma to separate email')),
		'dbbackup_interval' => $interval,
	);
	return $result;
}

function dbbackup_config_save(){
	$cron = cc('cron');
	$cron['dbbackup'] = pp('dbbackup_interval');
	config_set('cron', $cron);
	config_set('dbbackup_interval', pp('dbbackup_interval'));
	config_set('dbbackup_send_mail', pp('dbbackup_send_mail' ));
	config_set('dbbackup_mail_list', pp('dbbackup_mail_list' ));
}
?>
