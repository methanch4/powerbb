<?php 

/**
 * @var Array $setup Dieser String wird von cUser ausgewertet und an die Datenbank übertragen
 */
$setup = array();


$setup[] = "SET SQL_MODE = \"NO_AUTO_VALUE_ON_ZERO\"";
$setup[] = "SET AUTOCOMMIT = 0;";
$setup[] = "START TRANSACTION;";
$setup[] = "SET time_zone = \"+00:00\";";


$setup[] = "CREATE TABLE `[pre_]system_config` (
  `config_id` int(11) NOT NULL,
  `sys_module` varchar(20) NOT NULL,
  `sys_key` varchar(20) NOT NULL,
  `value` text NOT NULL,
  `comment` text,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "
CREATE TABLE `[pre_]system_log` (
  `log_id` int(11) NOT NULL,
  `modul` varchar(20) NOT NULL,
  `methode` varchar(20) NOT NULL,
  `event` varchar(20) NOT NULL,
  `value` text NOT NULL,
  `timestamp` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;";

$setup[] = "ALTER TABLE `[pre_]system_config`  
  ADD PRIMARY KEY (`config_id`);";

$setup[] = "ALTER TABLE `[pre_]system_log`
  ADD PRIMARY KEY (`log_id`);";

$setup[] = "ALTER TABLE `[pre_]system_config`
  MODIFY `config_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;";

$setup[] = "ALTER TABLE `[pre_]system_log`
  MODIFY `log_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;";

$setup[] = "COMMIT;";

?>
