<?php
/// @file
///	This file provides file-module, which allows users to manage files in the system.
/// The files are in ROOT_PATH/files/member/ID, with ID=ID of the user.
///
/// @author bow_der_kleine <bow_der_kleine@yahoo.de>
/// @state fully refactored, global variable check required.

if(authority_is_guest()) error_denied();

execute_add_function('file_manage_user');
execute_add_function('file_manage_admin');
execute_add_function('file_create_dir');
execute_add_function('file_drop');

execute_add_function('file_upload');
execute_add_function('file_chmod');
execute_add_function('file_copy');
execute_add_function('file_cut');
execute_add_function('file_rename');
execute_add_function('file_show');

execute_add_function('file_paste');
execute_add_function('file_drop_multiple');

include('file.tool.inc');

if(MODE == 'file_manage_user' or MODE == 'file_manage_admin'){
	include('file_manage.module.inc');
}else{
	include('file_handle.module.inc');
}

?>
