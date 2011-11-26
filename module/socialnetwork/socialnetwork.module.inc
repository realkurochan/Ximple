<?php
require_once('socialnetwork.tool.inc');

execute_add_function('socialnetwork_share');
execute_add_function('socialnetwork_share_config');

execute_add_function('socialnetwork_save_config');

execute_add_function('socialnetwork_like_box');
execute_add_function('socialnetwork_like_box_config');

execute_add_function('socialnetwork_activity_feed');
execute_add_function('socialnetwork_activity_feed_config');

execute_add_function('socialnetwork_profile');
execute_add_function('socialnetwork_profile_config');

execute_add_function('socialnetwork_faves');
execute_add_function('socialnetwork_faves_config');

function socialnetwork_share($data){
	$render_array = array();
	foreach($data as $key => $value){
		$render_array['data'] = $value;
		$data[$key]['content'] = render($render_array, 'socialnetwork_share.tpl', 'socialnetwork_share');
	}
	return $data;
}

function socialnetwork_share_config($config){
	$config[tt('Facebook Like Button')] = array(
		'socialnetwork_like_type' => socialnetwork_like_type_config(),
		'socialnetwork_like_color' => socialnetwork_color_config(),
		'socialnetwork_like_width' => socialnetwork_width_config(),
	);
	$config[tt('Tweet Button')] = array(
		'socialnetwork_tweet_type' => socialnetwork_tweet_type_config(),
	);
	return $config;
}

function socialnetwork_save_config(){
	$post = pp();
	foreach($post as $key => $value){
		if(substr($key, 0, 13) == 'socialnetwork'){
			config_set($key, $value);
		}
	}
	config_save();
}

function socialnetwork_like_box(){
	return render(array(), 'socialnetwork_like_box.tpl', 'socialnetwork_like_box');
}

function socialnetwork_like_box_config(){
	return array(
		'socialnetwork_like_box_uri' => socialnetwork_uri_config(tt('Facebook Page URI'), cc('socialnetwork_like_box_uri')),
		'socialnetwork_like_box_color' => socialnetwork_color_config(cc('socialnetwork_like_box_color')),
		'socialnetwork_like_box_width' => socialnetwork_width_config(cc('socialnetwork_like_box_width')),
		'socialnetwork_like_box_height' => socialnetwork_height_config(cc('socialnetwork_like_box_height')),
		'socialnetwork_like_box_appearance' => socialnetwork_like_box_appearance_config(),
	);
}

function socialnetwork_activity_feed(){
	return render(array(), 'socialnetwork_activity_feed.tpl', 'socialnetwork_activity_feed');
}

function socialnetwork_activity_feed_config(){
	return array(
		'socialnetwork_activity_feed_uri' => socialnetwork_uri_config(tt('Domain'), cc('socialnetwork_activity_feed_uri')),
		'socialnetwork_activity_feed_color' => socialnetwork_color_config(cc('socialnetwork_activity_feed_color')),
		'socialnetwork_activity_feed_width' => socialnetwork_width_config(cc('socialnetwork_activity_feed_width')),
		'socialnetwork_activity_feed_height' => socialnetwork_height_config(cc('socialnetwork_activity_feed_height')),
		'socialnetwork_activity_feed_appearance' => socialnetwork_activity_feed_appearance_config(),
	);
}

function socialnetwork_profile(){
	return render(array(), 'socialnetwork_profile.tpl', 'socialnetwork_profile');
}

function socialnetwork_profile_config(){
	$form = array(
		'socialnetwork_profile_user_name' => socialnetwork_uri_config(tt('user name'), cc('socialnetwork_profile_user_name')),
		'socialnetwork_profile_number' => socialnetwork_number_config(tt('tweets number'), cc('socialnetwork_profile_number')),
		'socialnetwork_profile_width' => socialnetwork_width_config(cc('socialnetwork_profile_width')),
		'socialnetwork_profile_height' => socialnetwork_height_config(cc('socialnetwork_profile_height')),
		'socialnetwork_profile_appearance' => socialnetwork_profile_appearance_config(),
	);
	$colors = array(
		'shell_background' => tt('shell background'),
		'shell_text' => tt('shell text'),
		'tweet_background' => tt('tweet background'),
		'tweet_text' => tt('tweet text'),
		'link' => tt('link')
	);
	foreach($colors as $key => $value){
		$key = 'socialnetwork_profile_color_'.$key;
		$form[$key] = Model::color($value);
		$form[$key]->data = cc($key);
	}
	return $form;
}

function socialnetwork_faves(){
	return render(array(), 'socialnetwork_faves.tpl', 'socialnetwork_faves');
}

function socialnetwork_faves_config(){
	$form = array(
		'socialnetwork_faves_user_name' => socialnetwork_uri_config(tt('user name'), cc('socialnetwork_faves_user_name')),
		'socialnetwork_faves_title' => socialnetwork_uri_config(tt('title'), cc('socialnetwork_faves_title')),
		'socialnetwork_faves_caption' => socialnetwork_uri_config(tt('caption'), cc('socialnetwork_faves_caption')),
		'socialnetwork_faves_number' => socialnetwork_number_config(tt('tweets number'), cc('socialnetwork_faves_number')),
		'socialnetwork_faves_width' => socialnetwork_width_config(cc('socialnetwork_faves_width')),
		'socialnetwork_faves_height' => socialnetwork_height_config(cc('socialnetwork_faves_height')),
		'socialnetwork_faves_appearance' => socialnetwork_faves_appearance_config(),
	);
	$colors = array(
		'shell_background' => tt('shell background'),
		'shell_text' => tt('shell text'),
		'tweet_background' => tt('tweet background'),
		'tweet_text' => tt('tweet text'),
		'link' => tt('link')
	);
	foreach($colors as $key => $value){
		$key = 'socialnetwork_faves_color_'.$key;
		$form[$key] = Model::color($value);
		$form[$key]->data = cc($key);
	}
	return $form;
}
?>
