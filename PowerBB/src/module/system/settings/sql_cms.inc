<?php

/**
 * @var Array $setup Dieser String wird von cUser ausgewertet und an die Datenbank übertragen
 */
$setup = array();

$setup[] = "SET SQL_MODE = 'NO_AUTO_VALUE_ON_ZERO';";
$setup[] = "SET AUTOCOMMIT = 0;";
$setup[] = "START TRANSACTION;";
$setup[] = "SET time_zone = '+00:00';";

$setup[] = "CREATE TABLE `[pre_]cms_access_list` (
  `cms_access_id` int(11) NOT NULL,
  `cid` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  `status` enum('allowed','denied') NOT NULL,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "CREATE TABLE `[pre_]cms_conted_list` (
  `cid` int(11) NOT NULL,
  `name` varchar(20) NOT NULL,
  `param` text,
  `file` varchar(20) NOT NULL,
  `modul` varchar(20) NOT NULL,
  `parent_key` int(11),
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "CREATE TABLE `[pre_]cms_show_list` (
  `cms_show_id` int(11) NOT NULL,
  `cid` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  `status` enum('show','hide') NOT NULL,
  `pos` int(11) NOT NULL,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "ALTER TABLE `[pre_]cms_access_list`
  ADD PRIMARY KEY (`cms_access_id`),
  ADD KEY `cid` (`cid`,`group_id`);";

$setup[] = "ALTER TABLE `[pre_]cms_conted_list`
  ADD PRIMARY KEY (`cid`);";

$setup[] = "ALTER TABLE `[pre_]cms_show_list`
  ADD PRIMARY KEY (`cms_show_id`),
  ADD KEY `cid` (`cid`,`group_id`);";

$setup[] = "ALTER TABLE `[pre_]cms_access_list`
  MODIFY `cms_access_id` int(11) NOT NULL AUTO_INCREMENT;";

$setup[] = "ALTER TABLE `[pre_]cms_conted_list`
  MODIFY `cid` int(11) NOT NULL AUTO_INCREMENT;";

$setup[] = "ALTER TABLE `[pre_]cms_show_list`
  MODIFY `cms_show_id` int(11) NOT NULL AUTO_INCREMENT;";

$setup[] = "INSERT INTO `[pre_]cms_conted_list` (`cid`, `name`, `param`, `file`, `modul`, `timestamp`)
  VALUES (NULL, 'welcome', NULL, 'start.inc', 'system', UNIX_TIMESTAMP()),
         (NULL, 'register', NULL, 'register.inc', 'system', UNIX_TIMESTAMP()),
		 (NULL, 'login', NULL, 'login.inc', 'system', UNIX_TIMESTAMP()),
		 (NULL, 'admin', NULL, 'admin.inc', 'system', UNIX_TIMESTAMP()),
		 (NULL, 'profile', NULL, 'profile.inc', 'system', UNIX_TIMESTAMP()),
		 (NULL, 'logout', NULL, 'logout.inc', 'system', UNIX_TIMESTAMP());";


$setup[] = "COMMIT;";


?>