<?php /*

	function windows($title, $inhalt)
	{
		$return = "";
		
		$return = "<table><tr><td id='fenster_title'>".$title."</td></tr><tr><td id='fenster_inhalt'>".$inhalt."</td></tr></table>";
		
		return $return;
	} //*/
	
	
	/**
	 * @global cSystem $system
	 * @param integer $cid Die CID des Systems
	 * @param string $name Die Zeichenkette die als Link angezeigt werden soll
	 * @param array[string]=>string $param key=>value als Parameter des Links 
	 * @param string $class Die CSS-Klasse in der der String angezeigt werden soll
	 * @return string
	 */
	function generate_link($cid, $name, $param=false, $class="link_default")
	{
		global $system;
		
		$string  = "<a href='./".$system->get_config("main", "index")."?cid=".$cid;

		if($param != false)
		{
			foreach($param as $key => $value)
			{
				$string .= "&".$key."=".$value;
			}//foreach
		}//if

		$string .= "' class='".$class."'>".$name."</a>";
		
		return $string;
	}
	

	
	function menue()
	{
		global $user;
		
		$linklist = $user->get_link_list();
		
		$string = "";
		foreach($linklist as $value)
		{
			if($value->cid == $user->get_cid()) 
			{
				$string .= "\n&raquo;&nbsp;".generate_link($value->cid, "__langsys_".$value->name.";", $value->param, "link_default_sel")." &nbsp; ";
			}
			else
			{
				$string .= "\n".generate_link($value->cid, "__langsys_".$value->name.";", $value->param)." &nbsp; ";
			}
		}//foreach
		
		return $string."\n";
	}
	
	
	function debugging()
	{
		/**
		 * @global cUser_manager $user
		 * @global cSystem $system
		 */
		global $timer_start, $system, $user;
		
		$timer_ende = microtime(true);
		$duration = ($timer_ende - $timer_start)*1000*1000;
		
		if($duration < 1500) $zeit_einheit = "&mu;s";
		else
		{
			$duration /= 1000;
			if($duration < 1500) $zeit_einheit = "ms";
			else
			{
				$duration /= 1000;
				$zeit_einheit = "s";
			}//if..else
		}//if..else
			
		
		$array = array(
				$system->get_config("main", "projectname")." - ".$system->get_config("main", "version"),
				"__langsys_debug_runtime; = ".sprintf('%04.2f', $duration). " ".$zeit_einheit,
				"__langsys_debug_memory; = ".floor(memory_get_usage(false)/1024)." kByte",
				"__langsys_debug_user_id; = ".$user->user_id,
				"__langsys_debug_user_name; = ".$user->user_name,
		);
		
		$string = "";
		
		for($i = 0; $i < count($array); $i++)
		{
			$string .= $array[$i];
			if($i < (count($array)-1)) $string .=  " &nbsp; <font id='foot_bullet'>&bull;</font> &nbsp;";
		}//for
		
		return $string;
	}
	
	/**
	 * @global cSystem $system
	 * @global cUser $user
	 * @return string
	 */
	function body()
	{
		global $system, $user;
		
		$return = "";
		
		$array = array(
				"__temp_header" => $system->get_config("main", "projectname"),
				"__tmpl_title" => "Ganz Wichtiger Titel",
				"__templ_body" => $user->get_contend(),
				"__templ_menue" => menue(),
				"__templ_debugging" => debugging(),
				"__templ_system_massage" => $system->get_system_massage(),
		);
		
		$return .= load_templ_file("body", $array);
		
		return $return;
	}
	
	function window_error($titel, $contend)
	{
		$return  = "";
		
		$return .= "<div id='templ_window_title_error'>".$titel."</div>";
		$return .= "<div id='templ_window_box_error'>".$contend."</div>";
		
		return $return;
	}


	function window($titel, $contend)
	{
		$return  = "";
		
		$return .= "<div id='templ_window_title'>".$titel."</div>";
		$return .= "<div id='templ_window_box'>".$contend."</div>";
		
		return $return;
	}
	
	
?>