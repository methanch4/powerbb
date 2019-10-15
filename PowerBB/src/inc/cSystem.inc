<?php

	/**
	 * Klasse zum verwalten der Systemkonfigurationen
	 * 
	 * @version 0.1		22.08.2019
	 * @author Siegurt Skoda
	 */
	
	class cSystem
	{
		private		$config = array();
		
		/**
		 * Enthällt eine Liste von Systemnachrichten, die während des Seitenaufbaus angefallen sind
		 * und dem Benutzer angezeigt werden sollten.
		 * @var array
		 */
		private 	$system_massage = array();
		
		
		public function __construct()
		{
			$boot = false;
			
			if(isset($_GET["session_restart"]) || (!isset($_SESSION["system_config"])))
			{
				echo "Start Session<br>";
				
				$config = array();
				
				include "./config.inc";
				
				$this->config = $config;
				
				$boot = true;
			}//if
			else
			{
				$this->config = $_SESSION["system_config"];
			}
			
			/* cDatabase ist ein generischer Datenbanktreiber. Er stellt die Verbindung zur Datenbank bei
			 * Bedarf her und Managt alle zugriffe auf die Datenbank. Dabei garantiert der Treiber das
			 * es zu keinen Abfragekollisionen bei multiblen Datenbankzugriffen kommt.
			 */
			$GLOBALS["database"] = new cDatabase($this->config["db"]["host"], $this->config["db"]["user"], $this->config["db"]["pass"], $this->config["db"]["db"], $this->config["db"]["prefix"]);
			
			if($boot)
			{
				$result = $GLOBALS["database"]->query("SHOW TABLES WHERE `Tables_in_".$this->config["db"]["db"]."` LIKE '[pre_]system_%' ");
				
				if($result->count == 0)
				{
					echo "Erzeuge die System-Datenbank.<br>";
					flush();
					
					// Lade die SQL-Informationen zum erstellen der Tabellen
					$setup = array();
					include "./inc/settings/system_sql.inc";
					
					// Installiere die Tabellen
					foreach($setup as $line) $GLOBALS["database"]->query($line);
					
					// erstelle einen Systemlog Eintrag das die Standart Benutzergruppen angelegt worden sind.
					$this->log("system", "__construct", "initilice_systemDB", "__langsys_initlice_systemdb_desc;");
				}//if
			}//if
		}//public function __construct()
		
		public function __destruct()
		{
			$_SESSION["system_config"] = $this->config;
		}
		
		/**
		 * Gibt eine Konfigurationseigenschaft zurück. Wenn diese noch nicht gecacht wurde
		 * wird sie in der Datenbank nachgeschlagen
		 * @param 	string $chapter		Kategorie in dem die Konfiguration abgelegt ist 
		 * @param 	string $key			Der Nachzufragende Schlüssel
		 * @return 	string|int|bool
		 * @global 	cDatabase $database		Link zur Datenbank
		 */
		public function get_config($chapter, $key)
		{
			global $database;
			
			if(!isset($this->config[$chapter][$key]))
			{
				$result = $database->query("SELECT `value` FROM `[pre_]system_config` WHERE `sys_module` = '".$chapter."' AND `sys_key` = '".$key."' ORDER BY `[pre_]system_config`.`timestamp` DESC LIMIT 1");
				
				if($result->count == 0)
				{
					$string = "system_config_get_".$key;
					$value = $GLOBALS[$chapter]->$string;
					
					$this->set_config($chapter, $key, $value);
					$this->config[$chapter][$key] = $value;
				}//if
				else 
				{
					$this->config[$chapter][$key] = $result->get_word();
				}
			}

			return $this->config[$chapter][$key];
		}//public function get_config($chapter, $key)
		
		/**
		 * Fügt eine neue Konfigurationseigenschaft zur Datenbank hinzu.
		 * Eine aktuallisierung einer alten Eigenschaft ist nicht notwendig und das löschen der 
		 * alten Eingeschaft zu dokumentationszwecken nicht gewünscht, da immer nur der zuletzt
		 * eingetragene Eintrag gültig ist. Alle älteren Einträge verlieren sofort ihre Gültigkeit
		 * @param 	string $chapter		Kategorie in dem die Konfiguration abgelegt ist
		 * @param 	string $key			Der Nachzufragende Schlüssel
		 * @param	mixed  $value		Der Wert des einzutragenden Schlüssel.
		 * @global 	cDatabase $database		Link zur Datenbank
		 */
		public function set_config($chapter, $key, $value, $comment = "NULL")
		{
			global $database;
			
			if($comment == "NULL") $sql_com = "NULL";
			else $sql_com = "'".$comment."'";
			
			$database->query("INSERT INTO `[pre_]system_config` (`sys_module`, `sys_key`, `value`, `comment`, `timestamp`) VALUES ('".$chapter."', '".$key."', '".$value."', ".$sql_com.", '".time()."') ");
		}//public function set_config(..)
		
		/**
		 * Fügt eine Logeintrag in das Systemlog hinzu
		 * @param 	string $chapter		Kategorie in dem die Konfiguration abgelegt ist
		 * @param 	string $key			Der Nachzufragende Schlüssel
		 * @param	mixed  $value		Der Wert des einzutragenden Schlüssel.
		 * @global 	cDatabase $database		Link zur Datenbank
		 */
		public function log($modul, $methode, $event, $value)
		{
			global $database;
			
			$database->query("INSERT INTO `[pre_]system_log` (`modul`, `methode`, `event`, `value`, `timestamp`) VALUES ('".$modul."', '".$methode."', '".$event."', '".$value."', '".time()."') ");
		}//public function set_config(..)
		
		public function add_system_massage($text)
		{
			$this->system_massage[] = $text;
		}
		
		public function get_system_massage()
		{
			$return = "";
			
			if(count($this->system_massage) > 0)
			{
				$liste = "<ol>\n";
				foreach($this->system_massage as $value)
				{
					$liste = "  <li>".$value."</li>\n";
				}//foreach
				$liste .= "</ol>\n";
				$return = window("__langsys_system_massages_title", $liste);
			}//if
			
			return $return;
		}
		
		
		public function __get($key)
		{
			$return = "";
			
			switch($key)
			{
				default:
					echo "<FONT color=red>FATALER FEHLER</font> in <TT>cSystem</TT>. Es wurde versucht auf die Eigenschaft <TT>".$key."</TT> zuzugreifen. Diese wurde jedoch nicht definiert.<br>";
					kill(1);
			}//switch
			
			return $return;
		}//public function __get($key)
		
	}


?>