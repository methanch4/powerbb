<?php

	ini_set("display_errors", 1);
	ini_set("html_errors", 1);

	/**
	 * Loads a File into RAM and converts it in a strong
	 * @param 	string $filename	The name of the file to load
	 * @return 	string				a String, that contails the file
	 */
	
	function load_templ_file($filename, $template)
	{
		$file = file("./template/".$filename.".html");
		
		$return = "";

		// konvertiere die Datei in ein String
		foreach($file as $value)
		{
			$return .= $value."\n";
		}
		
		// ersetze alle vorkommenden template-platzhalter durch ihre templates
		foreach($template as $key => $value)
		{
			$return = str_replace($key, $value, $return);
		}
		
//		debug_analyse($return);
		return $return;
	}

?>
