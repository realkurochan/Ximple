<?php
function tracker_insert($data){
	if(cc('tracker_enable') and !authority_is_guest()){
		$tracker_dir = FILES_PATH.'tracker';
		if(!is_dir($tracker_dir)) mkdir($tracker_dir);
		$tracker_dir = FILES_PATH.'tracker/'.USER_ID.'/';
		if(!is_dir($tracker_dir)) mkdir($tracker_dir);
		$file_name = io_get_loop_file('tracker/'.USER_ID.'/', ceil(cc('tracker_time')/86400));
		$track = $data['mode'].','.$data['id'].','.$data['topic'].',';
		$track.= $data['writer'].','.$data['writer_name'].','.$data['action']."\n";
		io_write_file($file_name, $track, 'a');
	}
}
?>
