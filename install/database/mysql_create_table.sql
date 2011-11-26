CREATE TABLE `widget` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(250) NOT NULL,
  `config_form_mode` varchar(250) DEFAULT NULL,
  `config_save_mode` varchar(250) DEFAULT NULL,
  `call_mode` varchar(250) DEFAULT NULL,
  `position` varchar(250) DEFAULT NULL,
  `sort_number` int(11) DEFAULT NULL,
  `appear_module` text,
  `appear_uri` varchar(250) DEFAULT NULL,
  `appear_group` text,
  `body` text,
  `section` int(11) NOT NULL  DEFAULT 1,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `category` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `category` varchar(250) DEFAULT NULL,
  `mode` varchar(80) DEFAULT NULL,
  `pic` varchar(250) DEFAULT NULL,
  `section` int(11) NOT NULL DEFAULT 1,
  `description` text,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `comment` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `write_time` datetime DEFAULT NULL,
  `writer` varchar(80) NOT NULL,
  `writer_id` int(11) NOT NULL,
  `homepage` varchar(80) NOT NULL,
  `comment` text,
  `mode` varchar(80) NOT NULL,
  `parent` int(11) NOT NULL,
  `section` int(11) DEFAULT NULL,
  `parent_writer` int(11) DEFAULT NULL,
  `email` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `feed` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mode` varchar(250) DEFAULT NULL,
  `mode_id` int(11) DEFAULT NULL,
  `topic` varchar(250) DEFAULT NULL,
  `summary` text,
  `update_time` timestamp NOT NULL,
  `publish_time` timestamp NOT NULL,
  `writer` varchar(250) DEFAULT NULL,
  `comment_no` int(11) DEFAULT NULL,
  `uri` varchar(250) DEFAULT NULL,
  `freetag` varchar(250) DEFAULT NULL,
  `section` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `flashwidget` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `reference` varchar(250) DEFAULT NULL,
  `mode` varchar(250) DEFAULT NULL,
  `table_name` varchar(250) DEFAULT NULL,
  `column_name` varchar(250) DEFAULT NULL,
  `clause` varchar(250) DEFAULT NULL,
  `section` int(11) DEFAULT NULL,
  `item_number` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `freetag` (
  `freetag` varchar(255) DEFAULT NULL,
  `mode` varchar(80) DEFAULT NULL,
  `content_id` int(11) DEFAULT NULL
) CHARACTER SET utf8;

CREATE TABLE `groups` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(250) NOT NULL,
  `description` text NOT NULL,
  `section` int(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) CHARACTER SET utf8;

CREATE TABLE `map` (
  `users` int(11) DEFAULT NULL,
  `groups` int(11) DEFAULT NULL,
  `section` int(11) NOT NULL DEFAULT 1
) CHARACTER SET utf8;

CREATE TABLE `menu` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(250) NOT NULL,
  `uri` varchar(250) DEFAULT NULL,
  `widget` int(11) DEFAULT NULL,
  `sort_number` int(11) DEFAULT NULL,
  `section` int(11) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `message` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `message` text,
  `sender` int(11) DEFAULT NULL,
  `receiver` int(11) DEFAULT NULL,
  `write_time` datetime DEFAULT NULL,
  `status` varchar(5) DEFAULT 'new',
  `section` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `note` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `owner` int(11) DEFAULT NULL,
  `write_time` datetime DEFAULT NULL,
  `note` text,
  `section` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `profile` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `owner` int(11) DEFAULT NULL,
  `profiletype` int(11) DEFAULT NULL,
  `profile` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;


CREATE TABLE `profiletype` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(250) DEFAULT NULL,
  `description` varchar(250) DEFAULT NULL,
  `allowed_group` text,
  `section` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `search` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `keyword` varchar(250) DEFAULT NULL,
  `topic` varchar(250) DEFAULT NULL,
  `update_time` timestamp NOT NULL,
  `rendered` text,
  `content_id` int(11) DEFAULT NULL,
  `content_mode` varchar(250) DEFAULT NULL,
  `points` int(11) DEFAULT NULL,
  `clicks` int(11) DEFAULT NULL,
  `keywords_no` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `search_source` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(250) DEFAULT NULL,
  `keyword` varchar(250) DEFAULT NULL,
  `description` text,
  `content` text,
  `mode` varchar(250) DEFAULT NULL,
  `mode_id` int(11) DEFAULT NULL,
  `insert_time` timestamp NOT NULL,
  `language` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;


CREATE TABLE `translation` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `status` int(10) unsigned NOT NULL,
  `language` varchar(250) DEFAULT NULL,
  `module` varchar(250) DEFAULT NULL,
  `module_id` int(10) unsigned NOT NULL,
  `translation` mediumtext,
  PRIMARY KEY (`id`)
) CHARACTER SET utf8;

CREATE TABLE `section` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(250) DEFAULT NULL,
  `path` varchar(250) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `path` (`path`),
  UNIQUE KEY `section` (`name`)
) CHARACTER SET utf8;

CREATE TABLE `users` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `login_name` varchar(250) NOT NULL,
  `email` varchar(250) NOT NULL,
  `password` varchar(250) NOT NULL,
  `status` enum('active','deleted') DEFAULT 'active',
  `avatar` varchar(250) DEFAULT NULL,
  `section` int(11) NOT NULL DEFAULT 1,
  `space_used` int(11) NOT NULL DEFAULT 0,
  `login_name_hash` char(40) DEFAULT NULL,
  `email_hash` char(40) DEFAULT NULL,
  `contribution` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `login_name` (`login_name`),
  UNIQUE KEY `email` (`email`)
) CHARACTER SET utf8;

INSERT INTO groups VALUES (1, 'admin', 'An Admin can manage everthing on the Web.<br />This group ist not editable or deltetable.',1);
INSERT INTO groups VALUES (2, 'guest', 'Guest of Website, who did not log in.<br />This group ist not editable, deletetable or managable.',1);
INSERT INTO groups VALUES (3, 'users', 'Authenticated User of Website with some permission.<br />This group ist not editable, deletetable or managable.',1);
INSERT INTO map(users,groups) VALUES(1,1);
INSERT INTO profiletype(name, description, section) VALUES('about me', '', 1);

CREATE INDEX comment_mode ON comment(mode);
CREATE INDEX comment_parent ON comment(parent);
CREATE INDEX category_mode ON category(mode);
