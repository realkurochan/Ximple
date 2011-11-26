<?php
///	@file
/// This file save the access information of Ximple.


// Ximple supports mysql.
define('DB_TYPE','mysql');

// Database-Configurations (SQLite does not need these configurations)
define('DB_USER','docgen');
define('DB_NAME','docgen');
define('DB_HOST','localhost');
define('DB_PASS','&D0cm3ntG3n&');
define('DB_TABLE_PREFIX','');

// Ximple 0.4 supports  en, th, de.
define('DEFAULT_LANGUAGE','th');

// Please put slash ('/') at the end of this parameters.
// example: 'http://mydomain/' is a valid parameter but 'http://mydomain' is a invalid parameter
// example: '/my/root/path/' is a valid parameter but '/my/root/path' is a invalid parameter
define('ROOT_URI','http://www.ximplehub.com/docgen/Ximple/');
define('ROOT_PATH','/var/www/docgen/Ximple/');

// User upload file size
define('LIMIT_UPLOAD', false);
define('MAX_UPLOADED_FILE_SIZE', 500e3);
define('MAX_USER_SPACE_SIZE', 100e6);

// Email configuration for sending mail.
define('SITE_MAIL', 'info@mydomain.net');
define('SMTP_SERVER', 'smtp.mydomain.net:25');
define('SMTP_LOGIN', 'Your login name for SMTP Server');
define('SMTP_PASSWD', 'SMTP password');
?>
