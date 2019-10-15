<?php

	/**
	 * Klasse zum verwalten der Systemkonfigurationen
	 *
	 * @version 0.1		22.08.2019
	 * @author Siegurt Skoda
	 */
	
	class cLanguage
	{
		/**
		 * @var string  	Die eingestellte Sprache des Users
		 */
		private $laguage_settings = ""; 
		
		/**
		 * Konstruktor dieses Klasse.
		 * @global cUser_manager $user 
		 */
		public function __construct()
		{
			global $user;
			
			$this->laguage_settings = $user->get_setting("language", "lang_sel");
		}
		
		/**
		 * Analysiert den erzeugten String nach Sprachtemplates, extrahiert diese und
		 * ersetzt diese durch aus der Sprachdatei ausgelesene Templates.
		 * Fehlt das entsprechende Template in der Sprachdatei, wird diesese ergänzt
		 * und mit einem Marker versehen.
		 * @global cUser_manager $user 
		 * @param string $raw
		 * @return string
		 */		
		public function translate($raw)
		{
			// Sichere Steuerzeichen
			$raw = str_replace(array("?"), array("&#63;"), $raw);
			
			//////////////////////////////////////////////////////////////////////////////////////////
			// Zunächst werden nach Systemweiten Spachtemplates gesucht 
			
			// Trenne den Eingabestring anhand des Systemsprachtemplate Bezeichners __langsys_ auf
			$array = explode("__langsys_", $raw);
			
			// Trenne die Sprachtemplates vom rest des Textes und erzeuge dabei ein neues Array
			$c = count($array);
			$work = array();
			$work[] = $array[0];
			for($i=1;$i<$c;$i++)
			{
				$_array = explode(";", $array[$i], 2);
				// markiere die Sprachtemplates um sie später erkenne zu können
				$work[] = "?".$_array[0];
				if(isset($_array[1])) $work[] = $_array[1];
			}
			
			$lang = array();
			include "./language/".$this->laguage_settings."/__system.inc";
			
			// Durchmustere das Array nach Sprachtempaltes und ersetze sie durch die Sprachdatei
			$work2 = "";
			$change = false;
			
			foreach($work as $value)
			{
				if($value[0] == "?")
				{
					$value[0] = " ";
					$value = trim($value);
					
					if(!isset($lang["system"][$value]))
					{
						$lang["system"][$value] = "[lang]_".$value;
						$value = $lang["system"][$value];
						$change = true;
					}//if
					else
					{
						$value = $lang["system"][$value];
					}
				}//if
				
				$work2 .= $value;
			}//foreach
			
			if($change)
			{
				// Es wurden änderungen an der Sprachdatei vorgenommen. Offensichtlich fehlte
				// in der entsprechenden Sprachdatei ein Eintrag.
				$file_contend = "";
				foreach($lang["system"] as $key=>$value)
				{
					$file_contend .= "\t\$lang[\"system\"][\"";
					$file_contend .= $key;
					$file_contend .= "\"] = \"$value\";\n";
				}
				$file_contend = "<?php\n\r".$file_contend."?>";
				$handle = fopen ("./language/".$this->laguage_settings."/__system.inc", "w");
				fwrite ($handle, $file_contend);
				fclose ($handle);
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////
			// nun werden nach den Maskenspezifischen Sprachdateien geuscht
			
			// Trenne den Eingabestring anhand des Systemsprachtemplate Bezeichners __lang_ auf
			$array = explode("__lang_", $work2);
			
			// Trenne die Sprachtemplates vom rest des Textes und erzeuge dabei ein neues Array
			$c = count($array);
			$work = array();
			$work[] = $array[0];
			for($i=1;$i<$c;$i++)
			{
				$_array = explode(";", $array[$i], 2);
				// markiere die Sprachtemplates um sie später erkenne zu können
				$work[] = "?".$_array[0];
				if(isset($_array[1])) $work[] = $_array[1];
			}
			
			global $user;
			$mask_filename = $user->get_mask_file_name();
			
			$lang = array();
			include "./language/".$this->laguage_settings."/".$mask_filename;
			
			// Durchmustere das Array nach Sprachtempaltes und ersetze sie durch die Sprachdatei
			$work2 = "";
			$change = false;
			
			foreach($work as $value)
			{
				if($value[0] == "?")
				{
					$value[0] = " ";
					$value = trim($value);
					
					if(!isset($lang[$mask_filename][$value]))
					{
						$lang[$mask_filename][$value] = "[lang]_".$value;
						$value = $lang[$mask_filename][$value];
						$change = true;
					}//if
					else
					{
						$value = $lang[$mask_filename][$value];
					}
				}//if
				
				$work2 .= $value;
			}//foreach
			
			if($change)
			{
				// Es wurden änderungen an der Sprachdatei vorgenommen. Offensichtlich fehlte
				// in der entsprechenden Sprachdatei ein Eintrag.
				$file_contend = "";
				foreach($lang[$mask_filename] as $key=>$value)
				{
					$file_contend .= "\t\$lang[\"".$mask_filename."\"][\"";
					$file_contend .= $key;
					$file_contend .= "\"] = \"$value\";\n";
				}
				$file_contend = "<?php\n\r".$file_contend."?>";
				$handle = fopen ("./language/".$this->laguage_settings."/".$mask_filename, "w");
				fwrite ($handle, $file_contend);
				fclose ($handle);
			}
			
			$output = $work2;			
			
			$output = str_replace(array("&iquest;"), array(chr(150)), $output);
			return $output;
		}
	}


?>
