<?php

/**
 * @var Array $setup Dieser String wird von cUser ausgewertet und an die Datenbank übertragen
 */
$setup = array();

$setup[] = "SET SQL_MODE = \"NO_AUTO_VALUE_ON_ZERO\";";
$setup[] = "SET AUTOCOMMIT = 0;";
$setup[] = "START TRANSACTION;";
$setup[] = "SET time_zone = \"+00:00\";";

$setup[] = "CREATE TABLE `[pre_]user_config` (
  `user_config_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `user_module` varchar(20) NOT NULL,
  `user_key` varchar(15) NOT NULL,
  `value` text,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "CREATE TABLE `[pre_]user_default_config` (
  `user_default_config_id` int(11) NOT NULL,
  `user_module` varchar(20) NOT NULL,
  `user_key` varchar(20) NOT NULL,
  `value` text,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "CREATE TABLE `[pre_]user_event` (
  `evet_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `event_id` int(11) NOT NULL,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "CREATE TABLE `[pre_]user_groups` (
  `group_id` int(11) NOT NULL,
  `name` varchar(30) NOT NULL,
  `description` text,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "CREATE TABLE `[pre_]user_list` (
  `user_id` int(11) NOT NULL,
  `name` varchar(25) NOT NULL,
  `passwd` varchar(45) NOT NULL,
  `time_register` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "CREATE TABLE `[pre_]user_member_of_group` (
  `member_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  `comment` text NOT NULL,
  `valid` enum('no','yes') NOT NULL,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "ALTER TABLE `[pre_]user_config`
  ADD PRIMARY KEY (`user_config_id`),
  ADD KEY `user_id` (`user_id`);";

$setup[] = "ALTER TABLE `[pre_]user_default_config`
  ADD PRIMARY KEY (`user_default_config_id`);";

$setup[] = "ALTER TABLE `[pre_]user_event`
  ADD PRIMARY KEY (`evet_id`);";

$setup[] = "ALTER TABLE `[pre_]user_groups`
  ADD PRIMARY KEY (`group_id`);";

$setup[] = "ALTER TABLE `[pre_]user_list`
  ADD PRIMARY KEY (`user_id`);";

$setup[] = "ALTER TABLE `[pre_]user_member_of_group`
  ADD PRIMARY KEY (`member_id`),
  ADD KEY `user_id` (`user_id`,`group_id`);";

$setup[] = "ALTER TABLE `[pre_]user_config`
  MODIFY `user_config_id` int(11) NOT NULL AUTO_INCREMENT;";

$setup[] = "ALTER TABLE `[pre_]user_default_config`
  MODIFY `user_default_config_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;";

$setup[] = "ALTER TABLE `[pre_]user_event`
  MODIFY `evet_id` int(11) NOT NULL AUTO_INCREMENT;";

$setup[] = "ALTER TABLE `[pre_]user_groups`
  MODIFY `group_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;";

$setup[] = "ALTER TABLE `[pre_]user_list`
  MODIFY `user_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;";

$setup[] = "ALTER TABLE `[pre_]user_member_of_group`
  MODIFY `member_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;";

$setup[] = "COMMIT;";

?>