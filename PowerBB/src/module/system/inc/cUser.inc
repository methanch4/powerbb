<?php


/**
 * Eine relativ simple Benutzerverwaltung
 *   + Basiert auf MySQL
 *   + Selbsteinrichtend
 *   + Gruppenverwaltung
 *   + Verwaltung des Contend mit zugriffsberechtigungsüberprüfung.
 *
 * @author Siegurt Skoda
 * @version 0.1		28.06.2019
 */




						
	/**
	 * Eine Klasse die eine einzelne Einstellung des Benutzers cacht 
	 * @author Skoda Skoda
	 * @version 0.1		22.08.2019
	 */
	class cUser_element_property
	{
		const PROPERTY_NONE = 0;
		const PROPERTY_USER = 1;
		const PROPERTY_GLOBAL = 2;
		
		/**
		 * @var string		Des Modul, zu dem diese Einstellung gehört
		 */
		private $module = "";
		
		/**
		 * @var string		Die Bibliothek, zu dem diese Einstellung gehört
		 */
		private $library = "";
		
		/**
		 * @var string		Der Schlüssel zu dieser Einstellung
		 */
		private $key = "";
		
		/**
		 * @var string		Der Wert dieser Einstellung
		 */
		private $value = "";
		
		/**
		 * @var int 	Aus welcher Quelle stammt dieses Einstellung?
		 */
		private $source = cUser_element_property::PROPERTY_NONE;
		
		/**
		 * @var int		Wie lange ist der gechachte Wert gültig?
		 */
		private $timeout = -1;
		
		
		private $user_id = -1;
		
		
		public function __construct($module, $library, $key, $user_id)
		{
			$this->module = $module;
			$this->library = $library;
			$this->key = $key;
			$this->user_id = $user_id;
		}
		
		/**
		 * Sucht nach der gefragten Information in den verschiedenen Datenbanken
		 * und speichert sie für spätere Nutung in der Session. 
		 * @global cDatabase $database
		 */
		public function get_value()
		{
			global $database;
			
			// überprüfe ob der Gespeicherte Inhalt dieser Information nicht
			// veraltet ist (oder noch nie ausgelesenwurde).
			if($this->timeout < time())
			{
				// Die gespeicherte Information ist veraltet.
				
				// zuerst überprüfe ob der Benutzer individuelle Einstellungen zu dieser angefragten Information hat.
				$result = $database->query("SELECT   `[pre_]user_config`.`value` \n"
						                . " FROM     `[pre_]user_config` \n"
						                . " WHERE    `[pre_]user_config`.`user_id` = '".$this->user_id."' \n"  
										     . " AND `[pre_]user_config`.`user_library` = '".$this->library."' \n"
                                             . " AND `[pre_]user_config`.`user_key` = '".$this->key."' \n"
										. " ORDER BY `[pre_]user_config`.`timestamp` DESC \n"
										. " LIMIT 1;"
						);
				
				if($result->count() == 0)
				{
					// der Benutzer hat keine individuelle Einstellung für dieses Setup hinterlegt. Wir suchen nun in der Globalen Datenbank
					$result1 = $database->query("SELECT   `[pre_]user_default_config`.`value` \n"
											  ." FROM     `[pre_]user_default_config` \n" 
											  ." WHERE    `[pre_]user_default_config`.`user_library` = '".$this->library."' \n"
												  . " AND `[pre_]user_default_config`.`user_key` = '".$this->key."' \n"
                                             . " ORDER BY `[pre_]user_default_config`.`timestamp` DESC \n"
											 . " LIMIT 1;"
						);
					if($result1->count() == 0)
					{
						$__user_setting = false;
						// Es gibt im Moment auch keine Globale einstellung für diese Eingschaft in der Datebank.
						// Lade die Default-Einstellungen für dieses Modul und überprüfe ob wir eine globale Einstellung setzen können.
						$filename = "./module/".$this->module."/settings/".$this->library.".inc";
						if(file_exists($filename))
						{
							include_once $filename;
						}
						else
						{
							// Eine Solche Datei muss es geben.
							echo "<br><b>Fataler Fehler:</b> Die Datei <tt><b>$filename</b></tt> existiert nicht.";
							echo "Gesucht wird der Parameter:<br>";
							echo "Modul: <tt>".$this->module."</tt> --- Library: <tt>".$this->library."</tt>--- Key: <tt>".$this->key."</tt><br>";
							kill(1);
						}
						
						// Sollten passende Einstellungen gefunden worden sein, übertrage die Einstellung in den Cache und speicher sie gleich
						// auch in der Datenbank für globale Einstellungen. 
						if(isset($__user_setting[$this->library][$this->key]))
						{
							$this->value = $__user_setting[$this->library][$this->key];
							$database->query("INSERT INTO `[pre_]user_default_config` (`user_default_config_id`, `user_library`, `user_key`, `value`, `timestamp`) 
                                                                         VALUES (NULL, '".$this->library."', '".$this->key."', '".$this->value."', '".time()."');");
						}
						else
						{
							// Es gibt auch im Setup dieses Moduls keine Default-Einstellung für diese Eigenschaft.
							// Das ist ein schwerer Fehler
							echo "Es wurden keine Benutzereigenschaften für hinterlegt für: <br />";
							echo "<b>Modul:</b> <tt>".$this->module."</tt> <br />";
							echo "<b>Library:</b> <tt>".$this->library."</tt> <br />";
							echo "<b>Schlüssel:</b> <tt>".$this->key."</tt> <br />";
							kill(-1);
						}
					}//if
					else
					{
						// Es wurden globale aber keine individuelle Einstellungen gefunden. Lade die globalen Einstellungen
						$this->value = $result1->get_word();
					}
				}//if
				else
				{
					// Es wurden individuelle Einstellungen gefunden. Lade diese.
					$this->value = $result->get_word();
				}
			}//if(timeout)
				
			return $this->value;
		}

		/**
		 * Speichert, wenn nötig, den Parameter unter den Benutzerinformationen
		 * @param string $value
		 * @global cDatabase $database
		 */
		public function set_value($value)
		{
			global $database;
			
			if($value != $this->value)
			{
				// Ändere nur was, wenn es auch was zu ändern gibt
				$this->value = $value;
				$database->query("INSERT INTO `powerbb_user_config` (`user_config_id`, `user_id`, `user_module`, `user_key`, `value`, `timestamp`) "
						        ."VALUES (NULL, '".$this->user_id."', '".$this->module."', '".$this->key."', '".$this->value."', UNIX_TIMESTAMP());");
			}//if
		}//function set_value($value)
		
		public function __get($value)
		{
			$return = "";
			switch($value)
			{
				case "value":
					$return = $this->get_value();
					break;
				default:
					echo "Schwerer systemfehler! cUser_element_property kennt die öffentliche Eigenschaft <tt><b>$value</b></tt> nicht.";
					kill (-1);
			}//switch
			
			return $return;
		}//public function __get($value)
	}//class cUser_element_property


	class cUser_element
	{
		private	$user_name	=		"";
		
		private	$user_id	=		0;
		
		/**
		 * @var cUser_element_property[][]		Liste der gechachten Einstellungen
		 */
		private $settings;
		
		/**
		 * @var array()				Ist der Benutzer Mitglied in dieser Gruppe?
		 */
		private $mog = array();
		
		/**
		 * @var array()				Liste der CIDs auf die dieser Benutzer zugriff hat.
		 */
		private $cms_list = array();
		
		/**
		 * Erzeugt einen neuen User
		 * @param string $first_param		either the user_id (the every other param should be free) or the user_name to register this user
		 * @param string $password
		 * @param string $email
		 * @global cDatabase $database
		 * @global cSystem $system
		 */
		public function __construct($first_param, $password="-1", $email="")
		{
			global $database, $system;
			
			if($password != "-1")
			{
				$name = $first_param;
				
				$salt = $system->get_config("user", "pass_salt");
				
				$pass = generate_password($name, $password, $salt);
								
				// Gibt es den Benutzername schon?
				$link = $database->query("SELECT `[pre_]user_list`.`user_id` FROM `[pre_]user_list` WHERE `[pre_]user_list`.`name` = '".$name."' LIMIT 1;");
				
				if($link->count > 0)
				{
					
					$link_1 = $database->query("SELECT `[pre_]user_list`.`user_id` FROM `[pre_]user_list` WHERE `[pre_]user_list`.`name` = '".$name."' AND `[pre_]user_list`.`passwd` = '".$pass."' LIMIT 1;");
					if($link_1->count > 0)
					{
						$this->user_id = $link_1->get_word();
						$this->load_base_user_data();
					}//if
					else
					{
						// ERROR: username exists
						$this->user_id = -2;
					}//if..else
				}//if
				else
				{
					$time = time();
					$database->query("INSERT INTO `[pre_]user_list` (`user_id`, `name`, `passwd`, `time_register`) VALUES (NULL, '".$name."', '".$pass."', '".$time."'); ");
										
					$link_1 = $database->query("SELECT `[pre_]user_list`.`user_id` FROM `[pre_]user_list` WHERE `[pre_]user_list`.`name` = '".$name."' AND `[pre_]user_list`.`passwd` = '".$pass."' LIMIT 1;");
					if($link_1->count > 0)
					{
						$this->user_id = $link_1->get_word();

						$group_id = $system->get_config("user", "group_registerd_user");
						$this->add_this_user_to_group($group_id);
						
						$this->load_base_user_data();
					}//if
				}//if..else
			}
			else
			{
				$this->user_id = $first_param;
				
				$db_link = $database->query("SELECT `name` FROM `[pre_]user_list` WHERE `user_id` = '".$this->user_id."' ORDER BY `[pre_]user_list`.`time_register` DESC LIMIT 1");
				$this->user_name = $db_link->get_word();
			}
			
		}//public function __construct($name, $password, $email)
		
		public function __get($name)
		{
			$return = "";
			
			switch($name)
			{
				case "name":
					$return = $this->user_name;
					break;
				case "user_id":
					$return = $this->user_id;
					break;
				case "cms_list":
					$return = $this->cms_list;
					break;
				default:
					echo "cUser_element::__get($name). Den Parameter $name gibt es nicht.";
					kill(1);
					break;
			}//switch($name)
			
			return $return; 
		}
		
		/**
		 * Es werden alle notwendigen Daten für diesen Benutzer geladen,
		 *   - Name
		 *   - Gruppenzugehörigkeit
		 * @global cDatabase $database
		 */
		private function load_base_user_data()
		{
			global $database;
			
			$link = $database->query("SELECT `[pre_]user_list`.`name` FROM `[pre_]user_list` WHERE `[pre_]user_list`.`user_id` = '".$this->user_id."' ORDER BY `[pre_]user_list`.`time_register` DESC LIMIT 1");
			$this->user_name = $link->get_word();
			
			///////////////////////////////////////////////////
			// Lade die Gruppendatenbank.
			
			// Um herauszufinden ob der Benutzer in einer bestimmten Gruppe ist wird der neuste "Member of group" Eintrag aus der
			// ist Liste geladen. Existiert dieser wird überprüft ob der Benutzer in der gruppe als aktiv markiert ist. Wenn ja, ist
			// er Mitglied dieser Gruppe.
			$link = $database->query("SELECT `[pre_]user_groups`.`group_id` FROM `[pre_]user_groups`");
			
			$group_list = $link->get_array();
			foreach($group_list as $value)
			{
				$link = $database->query("SELECT `valid` FROM `[pre_]user_member_of_group` \n"
										."WHERE    `[pre_]user_member_of_group`.`user_id` = '".$this->user_id."' \n"
                                        ."  AND    `[pre_]user_member_of_group`.`group_id` = ".$value->group_id." \n"
										."ORDER BY `[pre_]user_member_of_group`.`timestamp` DESC \n"
										."LIMIT 1; "
						);
				if(($link->count > 0) && ($link->get_word() == "yes")) $this->mog[$value->group_id] = true;
				else $this->mog[$value->group_id] = false;
			}//foreach
		}//load_base_user_data()
		
		/**
		 * Lädt die Rechteliste für den Benutzer
		 * @global cDatabase $database
		 * @global cSystem $system 
		 */
		public function get_user_permissions()
		{
			global $database, $system;
			
			///////////////////////////////////////////////////
			// Lade die CMS Datenstruktur passend zu der
			// Gruppeneinstellung
			
			// Da dies der erste Punkt ist, an dem das Skript mit der CMS-Datenbank in berührung kommt,
			// wird hier überprüft ob
			$result = $database->query("SHOW TABLES WHERE `Tables_in_".$database->db_name."` LIKE '[pre_]cms_%' ");
			
			if($result->count() == 0)
			{
				// Es wurden noch keine CMS-Tabellen angelegt. Hole dieses jetzt nach.
					
				echo "Erzeuge die CMS-Datenbank.<br>";
				flush();
								
				// Lade die SQL-Informationen zum erstellen der Tabellen
				$setup = array();
				include "./module/system/settings/sql_cms.inc";
				
				// Installiere die Tabellen
				foreach($setup as $line) $database->query($line);

				$database->query("SET autocommit = 1;");
				
				// Lade verschiedene Eigenschaften in Variablen				
				$result1 = $database->query("SELECT `cid` FROM `[pre_]cms_conted_list` WHERE `name` = 'welcome' ORDER BY `[pre_]cms_conted_list`.`timestamp` DESC LIMIT 1;");
				$cid_welcome = $result1->get_word();
				
				$result1 = $database->query("SELECT `cid` FROM `[pre_]cms_conted_list` WHERE `name` = 'register' ORDER BY `[pre_]cms_conted_list`.`timestamp` DESC LIMIT 1;");
				$cid_register = $result1->get_word();
				
				$result1 = $database->query("SELECT `cid` FROM `[pre_]cms_conted_list` WHERE `name` = 'login' ORDER BY `[pre_]cms_conted_list`.`timestamp` DESC LIMIT 1;");
				$cid_login = $result1->get_word();
				
				$result1 = $database->query("SELECT `cid` FROM `[pre_]cms_conted_list` WHERE `name` = 'admin' ORDER BY `[pre_]cms_conted_list`.`timestamp` DESC LIMIT 1;");
				$cid_admin = $result1->get_word();
				
				$result1 = $database->query("SELECT `cid` FROM `[pre_]cms_conted_list` WHERE `name` = 'profile' ORDER BY `[pre_]cms_conted_list`.`timestamp` DESC LIMIT 1;");
				$cid_profile = $result1->get_word();
				
				$result1 = $database->query("SELECT `cid` FROM `[pre_]cms_conted_list` WHERE `name` = 'logout' ORDER BY `[pre_]cms_conted_list`.`timestamp` DESC LIMIT 1;");
				$cid_logout = $result1->get_word(); // */
				
				$result1 = $database->query("SELECT `group_id` FROM `[pre_]user_groups` WHERE `name` = '__langsys_guestgroup;' ORDER BY `[pre_]user_groups`.`timestamp` DESC LIMIT 1;");
				$gid_guest = $result1->get_word();
				
				$result1 = $database->query("SELECT `group_id` FROM `[pre_]user_groups` WHERE `name` = '__langsys_usergroup;' ORDER BY `[pre_]user_groups`.`timestamp` DESC LIMIT 1;");
				$gid_user = $result1->get_word();
				
				$result1 = $database->query("SELECT `group_id` FROM `[pre_]user_groups` WHERE `name` = '__langsys_admingroup;' ORDER BY `[pre_]user_groups`.`timestamp` DESC LIMIT 1;");
				$gid_admin = $result1->get_word();
				
				// Füge einen Basisstamm an 
				$database->query("INSERT INTO `[pre_]cms_show_list` (`cms_show_id`, `cid`, `group_id`, `status`, `pos`, `timestamp`) VALUES (NULL, '".$cid_welcome."', '".$gid_guest."', 'show', '1', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_show_list` (`cms_show_id`, `cid`, `group_id`, `status`, `pos`, `timestamp`) VALUES (NULL, '".$cid_welcome."', '".$gid_user."', 'show', '1', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_show_list` (`cms_show_id`, `cid`, `group_id`, `status`, `pos`, `timestamp`) VALUES (NULL, '".$cid_register."', '".$gid_guest."', 'show', '2', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_show_list` (`cms_show_id`, `cid`, `group_id`, `status`, `pos`, `timestamp`) VALUES (NULL, '".$cid_login."', '".$gid_guest."', 'show', '3', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_show_list` (`cms_show_id`, `cid`, `group_id`, `status`, `pos`, `timestamp`) VALUES (NULL, '".$cid_logout."', '".$gid_user."', 'show', '100', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_show_list` (`cms_show_id`, `cid`, `group_id`, `status`, `pos`, `timestamp`) VALUES (NULL, '".$cid_profile."', '".$gid_user."', 'show', '4', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_show_list` (`cms_show_id`, `cid`, `group_id`, `status`, `pos`, `timestamp`) VALUES (NULL, '".$cid_admin."', '".$gid_admin."', 'show', '5', UNIX_TIMESTAMP());");

				$database->query("INSERT INTO `[pre_]cms_access_list` (`cms_access_id`, `cid`, `group_id`, `status`, `timestamp`) VALUES (NULL, '".$cid_welcome."', '".$gid_guest."', 'allowed', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_access_list` (`cms_access_id`, `cid`, `group_id`, `status`, `timestamp`) VALUES (NULL, '".$cid_welcome."', '".$gid_user."', 'allowed', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_access_list` (`cms_access_id`, `cid`, `group_id`, `status`, `timestamp`) VALUES (NULL, '".$cid_register."', '".$gid_guest."', 'allowed', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_access_list` (`cms_access_id`, `cid`, `group_id`, `status`, `timestamp`) VALUES (NULL, '".$cid_login."', '".$gid_guest."', 'allowed', UNIX_TIMESTAMP());");
				
				$database->query("INSERT INTO `[pre_]cms_access_list` (`cms_access_id`, `cid`, `group_id`, `status`, `timestamp`) VALUES (NULL, '".$cid_logout."', '".$gid_user."', 'allowed', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_access_list` (`cms_access_id`, `cid`, `group_id`, `status`, `timestamp`) VALUES (NULL, '".$cid_profile."', '".$gid_user."', 'allowed', UNIX_TIMESTAMP());");
				$database->query("INSERT INTO `[pre_]cms_access_list` (`cms_access_id`, `cid`, `group_id`, `status`, `timestamp`) VALUES (NULL, '".$cid_admin."', '".$gid_admin."', 'allowed', UNIX_TIMESTAMP());");

				// erstelle einen Systemlog Eintrag das die Standart Benutzergruppen angelegt worden sind.
				$system->log("user", "__construct", "initilice_cmsDB", "__langsys_initlice_cmsdb_desc;");
				
			}//if
			
			///////////////////////////////////
			// Load User-Group List
			
			// Load List of all Groups
			$query = $database->query("SELECT `name`, `group_id` FROM `[pre_]user_groups`");
			
			foreach($query as $group_id)
			{
				// Test for every group if this user a Member of this. 
				$query1 = $database->query("SELECT `valid` FROM `[pre_]user_member_of_group` "
						                     ."WHERE `user_id` = '".$this->user_id."'"
										       ."AND `group_id` = '".$group_id->group_id."' "
						                     ."ORDER BY `timestamp` DESC "
											 ."LIMIT 1;"
						);
				
				// only if there is an result and this result is "yes" this user is Member of this group.
				if($query1->count() > 0)
				{
					if($query1->get_word() == "yes")
					{
						$this->mog[(int)$group_id->group_id] = true;
					}//if
					else 
					{
						$this->mog[(int)$group_id->group_id] = false;
					}
				}//if
				else
				{
					$this->mog[(int)$group_id->group_id] = false;
				}
			}//foreach
			
			// load the contend Menue from Database, for all groupts this user is Member of.
			$query_string = "SELECT `cid` FROM `[pre_]cms_show_list` WHERE 0 ";
			foreach($this->mog as $key => $value)
			{
				if(!$value) continue;
				
				$query_string .= "OR `group_id` = '".$key."' ";
			}//foreach
			$query_string .= " ORDER BY `pos` ASC";
			$query = $database->query($query_string);
			
			// Load all Contend informations from the list of Contend-IDs this user is able to see.
			foreach($query as $value)
			{
				$cid = (int)$value->cid;
				$result2 = $database->query("SELECT * FROM `[pre_]cms_conted_list` WHERE `cid` = '".$cid."' LIMIT 1");
				$array = $result2->get_array()[0];
				
				$this->cms_list[$cid] = $array;
			}//foreach
		}//function get_user_permissions()
		
		/**
		 * Fügt diesen Benutzer eine Gruppe hinzu. Die Berechtigung dazu wird HIER nicht geprüft.
		 * @param integer $group_id
		 * @global cDatabase $database
		 */
		public function add_this_user_to_group($group_id)
		{
			global $database;
			
			// Füge den Gast nutzer der Gastgruppe hinzu
			$database->query("INSERT INTO `[pre_]user_member_of_group` (`user_id`, `group_id`, `comment`, `valid`, `timestamp`) VALUES ('".$this->user_id."', '".$group_id."', '', 'yes', '".time()."')");
		}
		
		/**
		 * Sucht nach einer bestimmten Eingenschaft des Benutzers und gibt diese zurück
		 * @param String $module	Das Modul zu der die Eigenschaft gehört
		 * @param String $key		Der Name der Eingeschaft
		 * @return mixed
		 */
		public function get_setting($module, $library, $key)
		{
			// wurde diese Eigneschaft schon einmal ausgelesen? Wenn nicht, muss der Eintrag im cache erst einmal
			// erzeugt und aus der DB ausgelesen werden.
			if(!isset($this->settings[$library][$key])) $this->settings[$library][$key] = new cUser_element_property($module, $library, $key, $this->user_id);

			// Gib die Eigenschaft zurück
			return $this->settings[$library][$key]->value;
		}
		
		/**
		 * Trägt einen neuen Parameter für den Benutzer ein. 
		 * @param string $module
		 * @param string $key
		 * @param string $value
		 */
		public function set_setting($module, $library, $key, $value)
		{
			if(!isset($this->settings[$library][$key]))
			{
				kill(1);
			}
			
			$this->settings[$library][$key]->set_value($value);
		}//function set_setting($module, $key, $value)
		
		/**
		 * Überprüft ob der Benutzer Mitgleid der Administratorengruppe ist. 
		 * @global cSystem $system
		 */
		public function is_admin(): bool
		{
			global $system;
			
			$gid_admin = $system->get_config("user", "group_admin_user");;
			
			return($this->mog[$gid_admin]);
		}

		/**
		 * Überprüft, ob der dieser Benutzer mitglied der Registrieten und Freigeschalteten Benutzer ist
		 * @global cSystem $system
		 */
		public function is_user(): bool
		{
			global $system;
			
			$gid_user = $system->get_config("user", "group_registerd_user");;
			
			return($this->mog[$gid_user]);
		}
		
	}//class cUser_element


class cUser_manager
{
	/**
	 * @var integer Aktuelle Contend ID
	 */
	private $cid = 0;
	
	/**
	 * @var integer Aktuelle ID des angemeldeten benutzers.  
	 */
	private $user_id = -1;
	
	/**
	 * @var cUser_element[] Die Datenbank mit den bisher geladenen Benutzereinstellungen. 
	 */
	private $user_database = array();
	
	/**
	 * Lädt die grundlegenden Konfiguartionen der Benutzerverwaltung aus
	 * der Session.
	 * Wurde die Session gerade erstellt, wird diese hier initialisiert und
	 * die Notwendigen Daten aus der Datenbank geladen. Sollten dort Werte fehlen
	 * werden diese hier erzeugt.  
	 * 
	 * @global $system cSystem
	 * @global $database cDatabase 
	 */
	function __construct()
	{
		global $system;
		global $database;
		
		if(isset($_SESSION["user_id"]) && !isset($_GET["session_restart"]))
		{
			$this->user_id = $_SESSION["user_id"];
			$this->user_database = $_SESSION["user_database"];
		}
		
		if(isset($_SESSION["user_database"]) && !isset($_GET["session_restart"]))
		{
			$this->user_database = $_SESSION["user_database"];
		}
		else
		{
			$this->user_database = array();
		}
		
		if($this->user_id == -1)
		{
			////////////////////////////////////////////////////
			// initialisiere die User_Session;

			$result = $database->query("SHOW TABLES WHERE `Tables_in_".$database->db_name."` LIKE '[pre_]user_%' ");
			
			if($result->count() == 0)
			{
				// Es wurde noch kein default user angelegt. Vermutlich wurde die Datenbank gerade erst
				// erstellt. Initialisiere die Datenbank mit den grundlegenen Benutzerdaten
				$time = time();
				
				$setup = array();
				include "./module/system/settings/sql_user.inc";
				
				echo "Erzeuge die Benutzerdatebank.<br>";
				flush();						
				
				// Installiere die Tabellen
				foreach($setup as $line) $database->query($line);
				
				$database->query("SET autocommit = 1;");
				
				// Erzeuge einen generischen Gast nutzer
				$database->query("INSERT INTO `[pre_]user_list` (`name`, `passwd`, `time_register`) VALUES ('__langsys_guestuser;', '-1', '".$time."')");
				
				// hole die Benutzere_id des gerade angelegten Benutzers
				$result = $database->query("SELECT `user_id` FROM `[pre_]user_list` WHERE `name` = '__langsys_guestuser;' AND `passwd` = '-1' AND `time_register` = '".$time."' LIMIT 1");
				$guest_id = $result->get_word();
				
				// Ereuge die drei Basis-Benutzergruppen
				$database->query("INSERT INTO `[pre_]user_groups` (`name`, `description`, `timestamp`) VALUES ('__langsys_guestgroup;', '__langsys_guestgroup_desc;', '".$time."')");
				$result = $database->query("SELECT `group_id` FROM `[pre_]user_groups` WHERE `name` = '__langsys_guestgroup;' AND `timestamp` = '".$time."' LIMIT 1");
				$group_guest_id = $result->get_word();
				
				$database->query("INSERT INTO `[pre_]user_groups` (`name`, `description`, `timestamp`) VALUES ('__langsys_usergroup;', '__langsys_usergroup_desc;', '".$time."')");
				$result = $database->query("SELECT `group_id` FROM `[pre_]user_groups` WHERE `name` = '__langsys_usergroup;' AND `timestamp` = '".$time."' LIMIT 1");
				$group_user_id = $result->get_word();
				
				$database->query("INSERT INTO `[pre_]user_groups` (`name`, `description`, `timestamp`) VALUES ('__langsys_admingroup;', '__langsys_admingroup_desc;', '".$time."')");
				$result = $database->query("SELECT `group_id` FROM `[pre_]user_groups` WHERE `name` = '__langsys_admingroup;' AND `timestamp` = '".$time."' LIMIT 1");
				$group_admin_id = $result->get_word();
								
				// Füge den Gast nutzer der Gastgruppe hinzu
				$database->query("INSERT INTO `[pre_]user_member_of_group` (`user_id`, `group_id`, `comment`, `valid`, `timestamp`) VALUES ('".$guest_id."', '".$group_guest_id."', '__langsys_admingroup;', 'yes', '".$time."')");
				
				// hinterlege den gerade angelegten Nutzer als Default Gast-Nutzer im System 
				$system->set_config("user", "guest_user_id", $guest_id, "__langsys_default_guest_user_desc;");

				// registriere die gerade angelegten Benutzergruppen mit den jeweiligen Systemrechten
				$system->set_config("user", "group_guest_user", $group_guest_id, "__langsys_default_guestgroup_desc;");
				$system->set_config("user", "group_registerd_user", $group_user_id, "__langsys_default_registeredgroup_desc;");
				$system->set_config("user", "group_admin_user", $group_admin_id, "__langsys_default_admingroup_desc;");
				
				// erstelle einen Systemlog Eintrag das die Standart Benutzergruppen angelegt worden sind.
				$system->log("user", "__construct", "initilice_userDB", "__langsys_initlice_userdb_desc;");
			}//
			
			$this->login_user($system->get_config("user", "guest_user_id"), "");
		}//if

		//Überprüfe ob der zu der User_ID gehörenden Benutzerdatenstamm bereits geladen wurde.
		if(!isset($this->user_database[$this->user_id]))
		{
			$this->login_user($this->user_id);
		}//if
	}//function __construct()
	
	function __destruct()
	{
		$_SESSION["user_id"] = $this->user_id;
		$_SESSION["user_database"] = $this->user_database;
	}
	
	/**************************************************************************************************/
	/** Benutzer - Verwaltung                                                                        **/
	/**************************************************************************************************/
	
	public function __get($name)
	{
		$return = "";
		
		switch($name)
		{
			case "user_id":
				$return = $this->user_id;
				break;
			case "user_name":
				$return = $this->user_database[$this->user_id]->name;
				break;
			case "system_config_get_pass_salt":
				// Erzeugt einen möglichst zufälligen Code, der als Salz für
				// die Passwortverschlüsselung verwendet werden soll, damit Rainbow-Tabellen
				// keine Chance haben. Wird nach der Installation genau einmal aufgerufen
				$return =  generate_password(time(), microtime(), $_SERVER["SERVER_NAME"]);
				break;
			case "system_config_get_add_admin_challange":
				// Dieser Parameter soll nur in der Config Datei stehen, und den ersten (berechtigten
				// benutzer in den Rang eines Admins erheben.
				$return = false;
				break;
			default:
				echo "cUser_manager::__get($name). Den Parameter $name gibt es nicht.";
				kill(-1);
				break;
		}//switch($name)

		return $return;
	}//public function __get(..)
	
	public function register_user($user_name, $user_pass, $email)
	{
		// Der erste Buchstabe darf auf keinen Fall ein _ sein.
		// Das _ ist für Systembezeichner (generische benutzer) reserviert.
		if($user_name[0] == "_") return -5;  
		
		// Überprüfe ob alles in Ordnung ist und handle entsprechend.
		$new_user = new cUser_element($user_name, $user_pass, $email);
		
		// Chached aktuelle informationen des Benutzers
		$this->user_database[$this->user_id] = $new_user;

		// Lädt alle Informationen die essentiell über den eingeloggten 
		// Benutzer sind. 
		$this->login_user($new_user->user_id);
		
		return $new_user;
	}//public function register_user($user_name, $user_pass, $email)
	
	
	/**
	 * versucht zum Paar "Username" und "Passwort" einen passenden Eintrag in der Datenbank zu finden.
	 * @global cDatabase $database
	 * @global cSystem $system
	 * @param string $user_name
	 * @param string $user_pass
	 */
	public function try_login($user_name, $user_pass)
	{
		global $database, $system;
		
		$salt = $system->get_config("user", "pass_salt");
		
		$pass = generate_password($user_name, $user_pass, $salt);
		
		$link_1 = $database->query("SELECT `[pre_]user_list`.`user_id` FROM `[pre_]user_list` WHERE `[pre_]user_list`.`name` = '".$user_name."' AND `[pre_]user_list`.`passwd` = '".$pass."' LIMIT 1;");
		if($link_1->count() > 0)
		{
			$this->login_user($link_1->get_word(), $pass);
			$return = 0; 
		}//if
		else
		{
			// ERROR: username exists
			$return = -1;
		}//if..else
		
		return $return;
	}//function try_login($user_name, $user_pass)
	
	
	/**
	 * Logt Benutzer ein und überprüft ob es schon einen Admin gibt.
	 * @global cSystem $system
	 * @global cDatabase $database	 
	 * @param integer $user_id
	 */
	public function login_user($user_id, $pass)
	{
		global $system, $database;
		
		$this->user_id = $user_id;
		
		// Lade Benutzerdaten, wenn sie noch nicht im System gespeichert sind
		if(!isset($this->user_database[$this->user_id]))
		{
			// Wurde noch nicht geladen. Hole dies an dieser Stelle nach.
			$this->user_database[$this->user_id] = new cUser_element($this->user_id);
		}//if
		
		// Lade die Benutzerberechtigungen für die Zugriffsüberprüfung
		$this->user_database[$this->user_id]->get_user_permissions();
		
		// Überprüfe ob Benutzer Mitglide der Benutzergruppe ist (also regulär angemeldet ist)
		if($this->is_user())
		{
			// Teste ob schon Admins in der Liste sind.
			$group_admin_id = $system->get_config("user", "group_admin_user");
			$result = $database->query("SELECT `member_id` FROM `[pre_]user_member_of_group` WHERE `group_id` = ".$group_admin_id." LIMIT 1");
			if($result->count() == 0)
			{
				$contend  = "";
				
				$_pass = $system->get_config("user", "add_admin_challange");
				
				if($_pass == $pass)
				{
					$contend .= "<b>__langsys_no_admin_user;</b><br><br>__langsys_no_admin_add_user;<br><br>\n";
					
					// Füge diesen Benutzer zur Admingruppe hinzu.
					$this->user_database[$this->user_id]->add_this_user_to_group($group_admin_id);
					
					// Lade die Benutzerberechtigungen für die Zugriffsüberprüfung neu, um die Adminberechtigung gleich zu erhalten.
					$this->user_database[$this->user_id]->get_user_permissions();
					
					// 
					$contend .= "__langsys_no_admin_delete_string;<br>\n";
					$contend .= "<tt>\$config[\"user\"][\"add_admin_challange\"]	= \"".$pass."\";</tt><br><br>";
				}
				else
				{
					$contend .= "<b>__langsys_no_admin_user;</b><br><br>__langsys_no_admin_user_desc;<br><br>\n";
					$contend .= "__langsys_no_admin_challange_desc;<br>";
					$contend .= "<tt>\$config[\"user\"][\"add_admin_challange\"]	= \"".$pass."\";</tt><br><br>";
					$contend .= "__langsys_no_admin_challange_todo;<br>";
					$contend .= generate_link(1, "__langsys_session_restart;", array("session_restart"=>"true"));
				}
				$system->add_system_massage($contend);
			}//if
			else
			{
				// Es gibt Admins.
				if($this->user_database[$this->user_id]->is_admin())
				{
					// Der Benutzer ist ein Admin
					$_pass = $system->get_config("user", "add_admin_challange");
					if($_pass)
					{
						// Die Challange ist noch in der Configdatei. Sie sollte aus sicherheitsgründen aber gelöscht werden.
						$contend  = "";
						$contend .= "<b>__langsys_no_admin_securety_admin_challange;</b><br>";
						$contend .= "__langsys_no_admin_delete_string;<br><br>\n";
						$contend .= "<tt>\$config[\"user\"][\"add_admin_challange\"]	= \"".$pass."\";</tt><br><br>";
						$system->add_system_massage($contend);
					}
				}//if
			}
		}//if
	}//public function login_user($user_id, $pass)
	
	/**
	 * @global cSystem $system
	 */
	public function logout()
	{
		global $system;
		
		$this->login_user($system->get_config("user", "guest_user_id"), "");
		
	}
	
	/**
	 * Sucht in den Einstellungen eines Benutzers
	 * @param string 	$module			Das Modul das 
	 * @param string 	$key
	 * @param int 		$user_id
	 */
	public function get_setting($module, $library, $key, $user_id = -1)
	{
		if($user_id == -1) $user_id = $this->user_id;
		
		return $this->user_database[$user_id]->get_setting($module, $library, $key);
	}
	
	public function set_setting($modul, $library, $key, $value, $user_id = -1)
	{
		if($user_id == -1) $user_id = $this->user_id;
		
		$this->user_database[$user_id]->set_setting($modul, $library, $key, $value);
	}
	
	public function is_admin($user_id = -1): bool
	{
		if($user_id == -1) $user_id = $this->user_id;
		
		return $this->user_database[$user_id]->is_admin();
	}

	public function is_user($user_id = -1): bool
	{
		if($user_id == -1) $user_id = $this->user_id;
		
		return $this->user_database[$user_id]->is_user();
	}
	
	/**************************************************************************************************/
	/** CMS - Verwaltung                                                                             **/
	/**************************************************************************************************/
	
	/**
	 * @global cSystem $system
	 * @return integer
	 */
	function load_cid()
	{
		global $system;
		
		if(!isset($_GET["cid"]))
		{
			$cid = $system->get_config("main", "start_cid");
		}
		else
		{
			$cid = $_GET["cid"];
		}
		
		$this->cid = $cid;
		
		return $cid;
	}
	
	function get_cid()
	{
		return $this->cid;
	}
	
	private function get_cid_db()
	{
		return $this->user_database[$this->user_id]->cms_list;
	}
	
	/**
	 * Gibt den Maskennamen der aktuell geladen CID zurück.
	 * Wird benötigt von cLanguage->translate() 
	 */
	public function get_mask_file_name()
	{
		$array = $this->get_cid_db();
		if(!isset($array[$this->cid])) return "fnf.inc";
		return $array[$this->cid]->file;
	}
	
	public function get_link_list()
	{
		return $this->get_cid_db();
	}
	
	public function get_contend()
	{
		global $form_manager;
		
		$array = $this->get_cid_db();
		
		if(!isset($array[$this->cid]))
		{
			$string = window_error("__lang_linkerror_cidnf_title;", "__lang_linkerror_cidnf_body;");
		}
		elseif(file_exists("./module/".$array[$this->cid]->modul."/contend/".$array[$this->cid]->file))
		{
			include "./module/".$array[$this->cid]->modul."/contend/".$array[$this->cid]->file;
			
			if(strlen($form_manager->prepro_action) > 0)
			{
				// Es sollen Formulardaten verarbeitet werden.
				$string = preprocessor($form_manager->prepro_action, $form_manager->prepro);
			}//if
			
			$string = get_conted();
		}
		else
		{
			$string = window_error("__lang_mainerror_fnf_title;", "__lang_mainerror_fnf_body;");
		}
			
		return $string;
	}//function get_contend()

}//class Users;


?>