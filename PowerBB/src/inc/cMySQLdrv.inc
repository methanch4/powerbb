<?php

    /*****************************************************
     *
     *  Database driver - MySQL Object (i)  für PHP 7.0
     *  version 1.4
     *
     *  by Siegurt Skoda
     *  ver 1.0 - 19.01.2005
     *  ver 1.1 - 01.12.2005
	 *                 	+ Ermöglicht multible Datenbanken.
	 *  ver 1.2 - 12.02.2006 		Ker.: 0.06
	 *                 	+ In der Fehlerbehandlung wurde ein Backtracing hinzugefügt. (PHP 5.0 wird vorrausgesetzt)
	 *  ver 1.3 - 28.10.2016
	 *                 	+ Update auf PHP 7.0
	 *                 	+ Fetcharray wurde mit einer Typenauswahl-Attribut erweitert.
	 *  ver 1.4 - 28.06.2019
	 *  				+ Überarbeitete Objektorientiertheit.
	 *  				+ Ergebnisse bekommen jetzt eine cDatabase_result klasse, um das semiprozedurale
	 *                    verhalten durch eine klare Objektorientiertheit zu ersetzen.
	 *                  - Viele, zum Teil völlig veraltete Methoden wurden entfernt.
	 *                  - Kompatibilität zum völlig veralteten PHP 4 wurde entfernt.   
     */
    
    /**
     * Eine Instanz dieser Klasse verwaltet eine Anfrage an die Datenbank.
     * Sie verwaltet die Ergebnisse.
     * 
     * Das Ergebniss ist Iterierbar (z.B. um es mit foreach zu durchmustern)
     * 
     * forach liefert im value ein stdClass 
     * 
     * @author Siegurt Skoda
     * @version 1.0		28.06.2019
     */
    class cDatabase_query implements Iterator
    {
    	/**
    	 * @var String 				Die Abfrage selber, für die dieses Objekt erzeugt wurde
    	 */
    	private $query_string;
    	
    	/**
    	 * @var Array 				Speichert das Ergebniss des Abfrage als Array of Objects
    	 */
    	private $stored_result = array();
    	
    	/**
    	 * @var boolean				Wurde die Anfrage erfolgreich bearbeitet?  
    	 */
    	private $success = false;
    	
    	/**
    	 * Sendet der datenbank ein Query und verarbeitet gegebenenfals das Ergebnis
    	 * @param cDatabase $parent
    	 * @param mysqli $db_link		Der Link zur Datenbank
    	 * @param string $string		Der Querystring
    	 */
    	function __construct($parent, $db_link, $string)
    	{
    		$this->query_string = $string;
    		
    		// Um "Out of Sync" probleme zu vermeiden wird das Query sofort vollständig durchgeführt.
    		$result = $db_link->query($string);
    		
			if($db_link->error)
			{
				$parent->error_handler("<B>SQL ERROR:</B> The given Query returned this error massage:<br /><br /><tt><b>".$db_link->error."</b></tt><br />", $string, 2);
				kill(1);
			}
    		
    		// Hat das Query einen Rückgabewert ergeben, dann muss dieses ausgelesen werden
    		if(is_object($result))
    		{
    			while($data = $result->fetch_object())
    			{
    				$this->stored_result[] = $data;
    			}//while
    		
    			// Die Datenverbindung muss geschlossen werden, damit eine neue Verbindung aufgebaut
    			// werden kann.
    			$result->close();
    			
    			$this->success = true;
    		}//if
    		else if($result) 
    		{
    			// Der Aufruf dieser Querry-Instanz liefert kein Ergebniss zurück (Wie INSERT INTO oder UPDATE)
    			// in diesem Fall erhalten wir bei Result nur einen Boolean zurückgeleifert, und die Verbindung
    			// wurde automatisch beendet (es kann ja nichts mehr nachkommen).
    			$this->success = true;
    		}
    	}//function __construct($db_link, $string)
    	
    	
    	/**
    	 * Gibt das Gesammte Ergebniss als Array zurück.
    	 * @return Array
    	 */
    	public function get_array()
    	{
    		return $this->stored_result;
    	}
    	
    	/**
    	 * Magische Funktion um Eigenschaften zu simulieren.
    	 * @param string $name
    	 * @return mixed
    	 */
    	public function __get($name)
    	{
    		$return = false;
    		
    		switch($name)
    		{
    			case "count":
    			case "length":
    				$return = count($this->stored_result);
    				break;
    			case "success":
    				return $this->success;
    				break;
    			default:
    				echo "<FONT color=red>FATALER FEHLER</font> in <TT>".__FILE__." - ".__CLASS__."</TT>. Es wurde versucht auf die Eigenschaft <TT>$name</TT> zuzugreifen. Diese wurde jedoch nicht definiert.<br>";
    				kill(1);
    		}//switch
    		
    		return $return;
    	}//public function __get($name)
    	
    	public function get_word()
    	{
    		if(isset($this->stored_result[0]))
    		{
    			foreach ($this->stored_result[0] as $value)
    			{
					$return = $value;    				
    				break;
    			}//foreach
    		}
    		else
    		{
    			$return = false;
    		}//if..else
    		
    		return $return;
    	}

    	
    	/* *************************************************************************** */
    	/*   Ab hier kommen Methoden, die die Iterator Klasse von seinen Kindern       */
    	/*   benötigt                                                                  */
    	/* *************************************************************************** */
    	
    	/**
    	 * Zeigt auf die Position des aktuellen Elementes des Iterators
    	 * @var integer
    	 */
    	private $iterator_pointer = 0;
    	
    	/**
    	 * {@inheritDoc}
    	 * @see Iterator::rewind()
    	 */
    	public function rewind()
    	{
    		$this->iterator_pointer = 0;
    	}
    	
    	/**
    	 * {@inheritDoc}
    	 * @see Iterator::current()
    	 * @return object
    	 */
    	public function current()
    	{
    		return $this->stored_result[$this->iterator_pointer];
    	}
    	
    	/**
    	 * {@inheritDoc}
    	 * @see Iterator::key()
    	 * @return integer
    	 */
    	public function key()
    	{
    		return $this->iterator_pointer;
    	}
    	
    	/**
    	 * {@inheritDoc}
    	 * @see Iterator::next()
    	 */
    	public function next()
    	{
    		++$this->iterator_pointer;
    	}
    	
    	/**
    	 * {@inheritDoc}
    	 * @see Iterator::valid()
    	 * @return boolean
    	 */
    	public function valid()
    	{
    		return isset($this->stored_result[$this->iterator_pointer]);
    	}

    }//class cDatabase_query implements Iterator
    
    
    class cDatabase
    {
        /**
         * Der globale Link zu MySQL Datenbank
         * @var mysqli
         */
    	private $db_link = false;

		/**
		 * Speichert benötigte Informationen um sich mit dem Datenbank-Server zu verbinden.
		 * @var String	Name/Pfad des Datenbankservers
		 */
    	private $db_host;
		
    	/**
		 * Speichert benötigte Informationen um sich mit dem Datenbank-Server zu verbinden.
    	 * @var String	Benutzername des Datenbank-Zugangs
    	 */
    	private $db_user;

    	/**
		 * Speichert benötigte Informationen um sich mit dem Datenbank-Server zu verbinden.
    	 * @var String	Passwort des Datenbank-zugangs
    	 */
    	private $db_pass;
		
    	/**
		 * Speichert benötigte Informationen um sich mit dem Datenbank-Server zu verbinden.
    	 * @var String	Name der Datenbank auf dem Server
    	 */
    	private $db_name;
		
    	/**
		 * Speichert benötigte Informationen um sich mit dem Datenbank-Server zu verbinden.
    	 * @var String	Tabellen-Prefix für mehrfrach genutzte Datenbanken.
    	 */
    	private $db_table_prefix;
		
		/**
		 * Während der Entwicklung ist es nützlich Datenbankfehlermeldungen in allen details zu sehen
		 * um Fehler auszumärzen. In Betriebsumgebungen sollte dies auf keinen Fall passieren, denn diese
		 * Fehlermedlungen geben viele Details über die Serverinfrastruktur preis.  
		 * @var boolean
		 */
    	private $show_error = true;
		
		/**
		 * Für Statistische auswertungen: wie viele anfragen an die Datenbank wurden in diesem Server
		 * aufruf durchgeführt?
		 * @var integer
		 */
    	private $query_count = 0;

		/**
		 * Es wird eine Verbindung zur Datenbank vorbreitete, aber noch nicht aufgebaut. Dies geschieht erst
		 * wenn die Verbindugn auch wirklich beöigt wird. 
		 * 
		 * @param string $db_host			Pfad zum Datenbank-Server
		 * @param string $db_user			Benutzername des Datenbank-Zugangs
		 * @param string $db_pass			Passwort des Datenbank-Zugangs (Klartext)
		 * @param string $db_name			Name der Datenbank in der die Tabellen dieses Projektes gespeichert sind
		 * @param string $db_table_prefix	Tabellenprefix für Mehrfachgenutzte Datenbanken
		 */
		function __construct($db_host, $db_user, $db_pass, $db_name, $db_table_prefix = "")
		{
			$this->db_host = $db_host;
			$this->db_user = $db_user;
			$this->db_pass = $db_pass;
			$this->db_name = $db_name;
			$this->db_table_prefix = $db_table_prefix;
			$this->db_link = false;
		}

		/**
		 * Wird das Programm beendet soll auch die Datenbankverbindung abgebaut werden,
		 * wenn sie denn existiert.
		 */
		function __destruct()
        {
			if ($this->db_link)
            {
                $this->db_link->close();
            }//if
        }

		/**
		 * Diese Methode überprüft ob die notwendigen Vorraussetzungen exisitieren um eine Datenbankverbindung
		 * aufzubauen, versucht die Verbindung herzustellen und überprüft ob das geklappt hat.
		 * im Fehlerfalle wird die Programausführung unterbrochen und eine geeigneten Fehlermeldung ausgegeben. 
		 */
        private function connect()
        {
        	// Ist das notwendige Modul geladen?
        	if (@function_exists("mysqli_connect"))
        	{
        		// Versuche die Verbindung herzustellen        		
        		$this->db_link = new mysqli($this->db_host, $this->db_user, $this->db_pass);
        		
        		if (!$this->db_link->connect_error)
        		{
        			$result = $this->db_link->query("USE ".$this->db_name);
        			if (!$result)
        			{
        				$this->error_handler("<B>FEHLER</B> in cDatabase::connect. Der MySQL-Treiber lieferte folgenden Fehler: <br /><B><TT>".$this->db_link->error."</TT></B><br />");
        			}
        		}
        		else
        		{
        			$this->db_link = false;
        			$this->error_handler("<B>FEHLER</B> in cDatabase::connect. Der MySQL-Treiber lieferte folgenden Fehler: <br /><B><TT>".$this->db_link->connect_error."</TT></B><br />");
        		}
        	}
        	else
        	{
        		$this->error_handler("<B>FATALER FEHLER</B> in cDatabase::connect.<br />driver_mysql_i.inc arbeitet mit dem PHP Modul <B>mysqli</B> zusammen. Dieses wurde jedoch nicht geladen. Bitte konfigureren Sie die php.ini so, dass dieses Modul geladen wird, laden Sie einen anderen Datenbanktreiber wie: driver_mysql.inc<br />");
        	}
        }//private function connect()
        
        
        /**
         * Erzeugt ein Query-Objekt, das die Anfrage an den Datenbankserver stellt.
         * Besteht noch keine Verbindung zum Datenbankserver, wird diese vorher aufgebaut.
         * @param String $string		Die Anfrage in SQL-Syntax. Tabellenprefix: "[pre_]"
         * @return cDatabase_query
         */
        public function query($string)
        {
			$string = trim($string);
        	
			// Wenn bisher keine Verbindung zum Datenbank-Server bestand, baue diese jetzt auf
        	if (!$this->db_link) $this->connect();
            
        	// Ersetze Tabellen-Templates durch den eigendlichen Prefix
        	$string = str_replace("[pre_]", $this->db_table_prefix, $string);
			
            // Erzeugt eine Querry-Klasse die diese Anfrage verwaltet.
        	$return = new cDatabase_query($this, $this->db_link, $string);
        	
            return $return;
        }

		public function __get($name)
		{
			$return = false;
			
			switch($name)
			{
				case "query_count":
					$return = $this->query_count;
					break;
				case "db_name":
					$return = $this->db_name;
					break;
				default:
					echo "<FONT color=red>FATALER FEHLER</font> in <TT>".__FILE__." - ".__CLASS__."</TT>. Es wurde versucht auf die Eigenschaft <TT>$name</TT> zuzugreifen. Diese wurde jedoch nicht definiert.<br>";
					kill(1);
			}//switch
			
			return $return;
		}
        
		public function error_handler($error, $query_string = "", $trace_level = 1)
        {
        	if ($this->show_error)
            {
            	$trace = debug_backtrace();
            	
            	// 12.02.06 - Backtrace eingefügt :) ENDLICH!!!!!!
            	echo "<HTML><BODY bgcolor=#FFFFFF style='font: 8pt verdana,arial,sans-serif; color:#000000'>\n";
		        echo "  <TABLE width=400 style='border: solid #FF0000 2px;' cellspacing=0 cellpadding=3>\n";
	            echo "    <TR height=20><TD bgcolor=#FF0000 style='font: 8pt verdana,arial,sans-serif; color:#000000'>Datenbankfehler: <B><FONT color=#FFFF00>".$this->db_name."</FONT></B> - Treiber: <FONT color=#FFFF00><B>dirver_mysql_i.inc</B></FONT></TD></TR>\n";
	            echo "    <TR><TD bgcolor=#FFFFFF style='font: 8pt verdana,arial,sans-serif; color:#707070;'>$error<br /></TD></TR>\n";
	            echo "    <TR><TD bgcolor=#FFFFFF style='font: 8pt verdana,arial,sans-serif; color:#707070;'><B>Query String:</B><br />\n";
	            $query = explode("\n", $query_string);
	            echo "<table cellspacing = 0;>\n";
	            $line_no = 1;
	            foreach($query as $line)
	            {
	            	echo "<tr>\n";
	            	echo "  <td align='right' style='border-right: 1px solid #888; backgroud-color: #CCC; color:888; font: 8pt verdana,arial,sans-serif; padding-right: 5px;'>".$line_no++."</td></td>\n";
	            	$line = str_replace(" ", "&nbsp;", $line);
	            	echo "  <td style='padding-left: 8px;'><tt>$line</tt></td>";
	            	echo "</tr>\n";
	            }
	            echo "</table>";
	            echo "</TD></TR>\n";
	            echo "    <TR><TD bgcolor=#FFFFFF style='font: 8pt verdana,arial,sans-serif; color:#707070;'><B>Backtrace:</B><br />File: <FONT color=#000000>".@$trace[$trace_level]["file"]."</FONT><br />Line: <FONT color=#000000>".@$trace[$trace_level]["line"]."</FONT><br />Function: <FONT color=#000000>".(isset($trace[$trace_level+1])?($trace[2]["function"]."("._listing($trace[$trace_level+1]["args"], ", ").");"):"aus der Hauptdatei oder einer abgeleiteten Instanz aufgerufen")."</FONT></TD></TR>\n";
	            echo "  </TABLE>\n";
	            echo "</HTML></BODY>\n";  //*/

				exit();
			}//if
		}//private function error_handler($error, $query_string)
		
    }//class cDatabase

?>