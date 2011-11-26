<?php
define('POLL_PERMISSION_ALL', 0);
define('POLL_PERMISSION_AUTHENTICATED', 1);
define('POLL_PERMISSION_VOTER_ONLY', 2);

function poll_permission_option(){
	return array(
		POLL_PERMISSION_ALL => tt('everyone can vote'),
		POLL_PERMISSION_AUTHENTICATED => tt('only authenticated user can vote'),
		POLL_PERMISSION_VOTER_ONLY => tt('only user with Poll-Voter roll can vote'),
	);
}

function poll_get_ip(){
	return vv('REMOTE_ADDR').':'.getenv('HTTP_X_FORWARDED_FOR');
}
?>
