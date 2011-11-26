<?php
define('XIMPLE_COMMENT_ALL', 0);
define('XIMPLE_COMMENT_AUTHENTICATED', 1);
define('XIMPLE_COMMENT_READ_ONLY', 2);
define('XIMPLE_COMMENT_DISABLE', 3);

function comment_permission_option(){
	return array(
		XIMPLE_COMMENT_ALL => tt('everybody can comment'),
		XIMPLE_COMMENT_AUTHENTICATED => tt('only authenticated user can comment'),
		XIMPLE_COMMENT_READ_ONLY => tt('read only'),
		XIMPLE_COMMENT_DISABLE => tt('disable'),
	);
}
?>
