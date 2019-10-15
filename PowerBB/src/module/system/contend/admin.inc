<?php 


	function admin_status(): string
	{
		$contend  = "";
		$contend .= window("__lang_admin_main_menue;", "Alles Fein!!!");
		
		return $contend;
	}
	
	
	function admin_module(): string
	{
		$contend = "";
		// öffnen des Verzeichnisses
		$handle = opendir("./module/");

		$contend .= "<table width=70%>\n";
		$contend .= "<tr><td width=30%>ID des Moduls</td><td width=50%>Name des Moduls</td><td>Version</td><td>Author</td></tr>\n";
		// einlesen der Verzeichnisses
		while (($file = readdir($handle)) !== false)
		{
			if(file_exists("./module/".$file."/settings/install.inc"))
			{
				$modul_name = "";
				$modul_version = "";
				$modul_author = "";
				
				include "./module/".$file."/settings/install.inc";
				
				$contend .= "<tr>";
				$contend .= "<td>".$file."</td>";
				$contend .= "<td>".$modul_name."</td>";
				$contend .= "<td>".$modul_version."</td>";
				$contend .= "<td>".$modul_author."</td>";
				$contend .= "</tr>";
			}
		}
		closedir($handle);
		$contend .= "</table>";
		
		return window("__lang_admin_modul_menue;", $contend);
	}
	
	function get_conted($param = array())
	{
		/**
		 * @global cUser_manager $user
		 * @global cSystem $system
		 * @global cForm_manager $form_manager
		 */
		global $user;
		
		if(!$user->is_admin())
		{
			$contend = "__lang_not_a_admin; \n";
		}//if
		else
		{
			if(isset($_GET["aid"])) $aid = $_GET["aid"];
			else $aid = 1;
			
			$contend = "";
			$contend .= "<table width=100%><tr><td width=200>";
			
			// Menue
			$contend_1  = "";
			if($aid == 1) $contend_1 .= "\n&raquo;&nbsp;";
			$contend_1 .= "  ".generate_link($user->get_cid(), "__lang_menue_status;", array("aid"=>1), ($aid == 1?"link_default_sel":"link_default"))."<br><br>";
			if($aid == 2) $contend_1 .= "\n&raquo;&nbsp;";
			$contend_1 .= "  ".generate_link($user->get_cid(), "__lang_menue_module;", array("aid"=>2), ($aid == 2?"link_default_sel":"link_default"))."<br><br>";
			
			$contend .= window("__lang_admin_main_menue;", $contend_1);
			$contend .= "</td><td>";
			
			// inhalt
			// Sollte die Variable kein gültiges Format haben, würd sie neu initialisiert.
			if(!is_numeric($aid)) $aid = 1;
			
			switch($aid)
			{
				case 2:
					$return = admin_module();
					break;
				case 1:
				case 0:
				default:
					$return = admin_status();
					break;
			}//switch
			
			$contend .= $return;
			
			
			$contend .= "</td></tr></table>";
		}//if..else
		
		return $contend;
	}



?>