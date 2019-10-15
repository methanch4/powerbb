<?php
	####################################################################################
	
	##   ##     #######      #####        ######          #######    #     #     ######
	# # # #        #        #     #      #      #            #       ##    #    #      #
	#  #  #        #        #            #                   #       # #   #    # 
	#     #        #         #####       #                   #       #  #  #    #
	#     #        #              #      #                   #       #   # #    #
	#     #        #        #     #      #      #    ##      #       #    ##    #      #
	#     #     #######      #####        ######     ##   #######    #     #     ######

	####################################################################################
	
	/* misc.inc
	 *
	 * This is a collection of usefull functions this can be used in several diffren 
	 * Projects. This collection first was  used in November 2004 and from this time
	 * on i looked  after it, day by day functions are reonvated  and tryed to write 
	 * to write it in a better way.
	 * I put  this file into severel Projects, but it  don't belonges to them. There- 
	 * for  it may be that in this libery  are functions that will  never be used in 
	 * the specific  project, even i try my best to clean this file  up from old  or 
	 * obsolet functions.
	 * If you are intreted  in new versions of this file,  so please have look on my
	 * homepage.
	 *
	 * license Type: GPL 2.0
	 */ 

	/********************************************************************************/
	/* Analyse Functions (D E B U G)                                                */
	/********************************************************************************/

	/**
	* @desc help debug_analyse function for recusriv analyse of SubObjects and SubArrays
	* @param variant $var
	* @internal
	*/
	function _analyse_array($var, $surpress)
	{
		echo "<br />{<TABLE style='border:0px'>";
		if (is_object($var))
		{
			$class = get_class($var);
			$array = get_class_methods($class);
			sort($array);
			foreach ($array as $value)
			{
				echo "<TR><TD colspan=4>&nbsp; &nbsp; ".$class."::<FONT color='#C00000'><B>$value</B></FONT>(...)</TD></TR>";
			}           
		}
		foreach($var as $key => $value)
		{
			echo "<TR><TD valign=top>&nbsp; &nbsp; ";
			switch(gettype($key))
			{
				case "integer":
					echo "<i>(integer)</i>";
					break;
				case "string":
					echo "<i>string (".strlen($key).")</i>";
					break;
			}
			echo "</TD><TD valign=top>";
			if (is_string($key)) echo "<FONT color='#0080F0'><I><B>\"</B></I></FONT>";
			echo "<FONT color='#000080'>$key</FONT>";
			if (is_string($key)) echo "<FONT color='#0080F0'><I><B>\"</B></I></FONT>";
			echo "</FONT></TD><TD valign=top><FONT color='#C00000'>=></FONT><TD valign=top>";
			switch(gettype($value))
			{
				case "boolean":
					echo "<i>(boolean)</i></TD><TD><B>";
					if($value) echo "<FONT color='#00C000'>true</FONT>";
					else  echo "<FONT color='#C00000'>false</FONT>";
					echo "</B>";
					break;
				case "integer":
					echo "<i>(integer)</i></TD><TD align=right width=30><B><FONT color='#008000'>".$value."</FONT></B>";
					break;
				case "double":
					echo "<i>(floating point)</i></TD><TD>".$value."";
					break;
				case "string":
					$lenght = strlen($value);
                    $value = str_replace(array("\0", "\r\n", "\t", "\$"), array("<FONT color='#FF0000'>&otimes;</FONT>", "<FONT color='#0000FF'>&crarr;</FONT>", "<FONT color='#0000FF'>&rarr;</FONT>", "<FONT color='#A00000'>\$</FONT>"), htmlentities($value));
					$value = str_replace(array("\n", "\r"), array("<FONT color='#008000'>&crarr;</FONT>", "<FONT color='#C00000'>\\r</FONT>"), $value);
					echo "<i>string (".$lenght.")</i></TD><TD valign=top><FONT color='#0080F0'><I>\"</I></FONT><FONT color='#000080'>".$value."</FONT><FONT color='#0080F0'><I>\"</I></FONT>";
					break;
				case "array":
					echo "<i><B>Array</B></i>";
					if(!isset($surpress[$key])) _analyse_array($value, $surpress);
					else echo $surpress[$key];
					break;
				case "object":
					echo "<i><B>Object of <FONT color=#A0A000>".get_class($value)."</FONT></i></B>";
					if(!isset($surpress[$key])) _analyse_array($value, $surpress);
					else echo $surpress[$key];
					break;
				case "NULL":
					echo "<I><B>NULL</B></I>";
					break;
				default:
					echo "<i>unkown type</i></TD><TD>".gettype($value);
			} // switch
			echo "</TD></TR>";
		} // foreach
		echo "</TABLE>}<br />";   
	}

	/**
	 *   writes a HTML compatible readable serialzation of an vaibale
	 *   This is mostly used in debuging of this projects /more then 10.000 times ;(
	 * 
	 * @param     variant          $var 
	 * @param     array         at Objects it may be usefull not to display some propertys (like a parent structure, in lists or trees)
	 * 
	 * @since 0.3
	 * 
	 * version 1    kernel 0.03        18.07.2003
     * version 2    kernel 0.10        14.12.2007   analyses objects
	 * version 3                       26.02.2008   completly overworked. nor the result is mush more readable
	 * version 3.1  kernel 0.11        03.03.2008   patching visibilety of not visbile signe ("\0") will be marked with a red &otimes;
	 *
	 * @todo #001 this function will not display privat propertys.  
	 * 
	 * @legend  red cressed zero        \0   Zero sign
	 *          blue down-left arrow    \r\n (windows) line break
	 *          green down-left arrow   \n   (linux, Solaris, MacOS X) line break
	 *          ! + red " "             \r   (old MacOS) line break
	 */
	function debug_analyse($var, $surpress=array())
	{
        echo "<br />called from: ".error_tracing_string(1)."<br />";
		switch(gettype($var))
		{
			case "boolean":
				echo "<i>(boolean)</i> => <B>";
				if($var) echo "<FONT color='#00C000'>true</FONT>";
				else  echo "<FONT color='#C00000'>false</FONT>";
				echo "</B>";
				break;
			case "integer":
				echo "<i>(integer)</i> => <B>".$var."</B>";
				break;
			case "double":
				echo "<i>(floating point)</i> => <B>".$var."</B>";
				break;
			case "string":
				$value = $var;
                $lenght = strlen($value);
                $value = str_replace(array("\0", "\r\n", "\t", "\$"), array("<FONT color='#FF0000'>&otimes;</FONT>", "<FONT color='#0000FF'>&crarr;</FONT>", "<FONT color='#0000FF'>&rarr;</FONT>", "<FONT color='#A00000'>\$</FONT>"), htmlentities($value));
				$value = str_replace(array("\n", "\r"), array("<FONT color='#008000'>&crarr;</FONT>", "<FONT color='#C00000'>!\\r;</FONT>"), $value);
                echo "<i>string (".$lenght.")</i> => <FONT color='#00C000'>\"</FONT>".$value."<FONT color='#00C000'>\"</FONT>";
				break;
			case "array":
				echo "<i><B>Array</B></i>";
				_analyse_array($var, $surpress);
				break;
			case "object":
				echo "<i><B>Object</B></i> of ".get_class($var);
				_analyse_array($var, $surpress);
				break;
			case "NULL":
				echo "<I><B>NULL</B></I>";
				break;
			default:
				echo "<i>unkown type</i> - ".gettype($value);
		}//switch
		echo "<br />\r\n";
	}// function debug_analyse

    
    /**
     * writes a HTML compatible readable serialzation of an vaibale
     * This is mostly used in debuging of this projects /more then 10.000 times ;(
     * 
     * @param     variant          $var 
     * @param     array         at Objects it may be usefull not to display some propertys (like a parent structure, in lists or trees)
     * 
     * @since 0.3
     * 
     * version 1    kernel 0.03        18.07.2003
     * version 2    kernel 0.10        14.12.2007   analyses objects
     * version 3                       26.02.2008   completly overworked. nor the result is mush more readable
     * version 3.1  kernel 0.11        03.03.2008   patching visibilety of not visbile signe ("\0") will be marked with a red &otimes;
     * version 4    kernel 0.12        09.09.2009   will show private propertys now
     *
     * 
     * @legend  red cressed zero        \0   Zero sign
     *          blue down-left arrow    \r\n (windows) line break
     *          green down-left arrow   \n   (linux, Solaris, MacOS X) line break
     *          ! + red " "             \r   (old MacOS) line break
     */
    function new_debug_analyse($var)
    {
        $string = serialize($var);
        echo $string;

        kill();
    }
     

	function error_tracing($lvl)
	{
		$trace = debug_backtrace();

		$return = array();
		$return["file"] = $trace[$lvl]["file"];
		$return["line"] = $trace[$lvl]["line"];
		$return["function"] = @$trace[$lvl-1]["function"];
		$return["args"] = @$trace[$lvl-1]["args"];
		
		return $return;        
	}
	
	/**
	 * @desc returns an string which showes from where this line is called.
	 * @param int $lvl     how many steps should the tracestring go back?
	 * @return string
	 */
	function error_tracing_string($lvl)
	{
		$trace = debug_backtrace();
		if(isset($trace[$lvl])) return "<B>".$trace[$lvl]["file"]."</B>, line: ".$trace[$lvl]["line"];
	}

	/**
	* @desc returns a HTML string of given variable type, and if possible it's value.
	* @param variant $bool 
	*/
	function bool($bool)
	{
		// v0.01 - 01.12.2005
		// v0.02 - 28.02.2006
		// v0.03 - 20.01.2008
		if (is_null($bool)) $return = "<FONT color='#000000'>NULL</FONT>";
		elseif (is_bool($bool)) $return = "<B>bool:</B> ".($bool?"<FONT color='#00C000'>true</FONT>":"<FONT color='#FF0000'>false</FONT>");
		elseif (is_int($bool)) $return = "<B>int:</B> <FONT color='#00C000'>$bool</FONT>";
		elseif (is_string($bool))
        {
            $bool = str_replace(array("\0", "\r\n"), array("<FONT color='#FF0000'>&otimes;</FONT>", "<FONT color='#0000FF'>&crarr;</FONT>"), htmlentities($bool));
            $bool = str_replace(array("\n", "\r"), array("<FONT color='#008000'>&crarr;</FONT>", "<FONT color='#C00000'>!\\r;</FONT>"), $bool);
            $return = "<B>str</B>(".strlen($bool)."): <TT>\"<FONT color='#00C000'>$bool</FONT>\"</TT>";
        }
		elseif (is_float($bool)) $return = "<B>float</B> <FONT color='#00C000'>$bool</FONT>";
		elseif (is_array($bool)) $return = "<B>array</B> <FONT coloe='#000000'><I>".count($bool)." entr.</I></FONT>";
		elseif (is_object($bool)) $return = "<B>obj</B> ".get_class($bool);
		else $return = "<B>other:</B> <FONT color='#00C000'>$bool</FONT>";
		return $return."\n";
	}
	
	/**
	* convert list of parameters into a string.
	* @internal
    * 18.06.2009
	*/
	function _listing($array, $trenner=", ")
	{
		$return = "";
		foreach($array as $value)
		{       
            $text = bool($value);
			$return .= ((strlen($return) > 0)?$trenner:"").$text;
		}
		return $return;
	}

   /**
    * returns a string where the function is called from
    * @param int    $depth      how deep should this function go?   -1 = go to he ground
    * 18.06.2009
    */
    function called_from($depth=-1)
    {

        $trace = debug_backtrace();
        $count = count($trace);
        if($depth > $count) $depth = $count;
        if($depth <= 0) $depth = $count;
        
        
        $return = "<table>";
        $return .= "<tr><td bgcolor='#C0C0FF'><b>File</b></td><td bgcolor='#E0E0FF'><b>Methode</b></td><td bgcolor='#C0C0FF' valign=right><b>Line</b></td></tr>";
        //for($i = ($count-1); $i >= ($count-$depth); $i--)
        for($i=0;$i<$depth;$i++)
        {
            $file = $trace[$i]["file"];
            if (isset($trace[$i]))
            {
                // inner Funcion. In most cases it's more importian to see, where the function is called.
                $function = $trace[$i]["function"]."("._listing($trace[$i]["args"], ", ").");";
                $line = $trace[$i]["line"];
            }
            else
            {
                // this function is called from the main function
                $function = "<I>main function</I>";
                $line = $trace[$i]["line"];
            }
            $return .= "<tr><td bgcolor='#C0C0FF'>".$file."</td><td bgcolor='#E0E0FF'>$function</td><td bgcolor='#C0C0FF' width=50 align=right>".$line."</td></tr>";
        }//for
        $return .= "</table>";
        
        return $return;
    }//function called_from($depth)
   
   
   /**
	* @desc send's an error massige, replces the replacments, send's backtrcaing informations and cancle the script.
	* @param string             $error      the sending error massige
	* @param array[string]->string    $replace    template replacments
	* @return string
	*/
	function error_handler($error, $replace=array(), $depht=2)
	{
		// v0.04 - 31.10.2005
		// v0.05 - 03.03.2006 - erweiter um die Funktion Backtrace :)
		// v0.06 - 04.01.2008 - template options, supportes str_replacing
		// v0.07 - 06.01.2008 - new design

		$string =  "<HTML><HEAD><STYLE type='text/css'><!-- td {font: 8pt verdana,arial,sans-serif; color:#707070;} --> </STYLE></HEAD>"
				  ."<BODY bgcolor=#FFFFFF class='cl'><TABLE width=400 style='border: solid #FF0000 2px;' cellspacing=0 cellpadding=3>"
				  ."<TR><TD bgcolor=#FFFFFF style=''>__error<br /></TD></TR><TR><TD bgcolor=#FFFFFF style='font: 8pt verdana,arial,sans-serif; color:#707070;'>"
				  ."<B>Backtrace:</B><br />File: <FONT color=#000000>__file</FONT><br />Line: <FONT color=#000000>__line</FONT>"
				  ."<br />called from: <FONT color=#000000>__function</FONT>__called_from</TD></TR></TABLE></HTML></BODY>\n";

		$search = array_keys($replace);
		$error = str_replace($search, $replace, $error);
		$trace = debug_backtrace();
		$file = $trace[$depht]["file"];
		if (isset($trace[$depht]))
		{
			// inner Funcion. In most cases it's more importian to see, where the function is called.
			$function = $trace[$depht]["function"]."("._listing($trace[$depht]["args"], ", ").");";
			$line = $trace[$depht]["line"];
		}
		else
		{
			// this function is called from the main function
			$function = "<I>main function</I>";
			$line = $trace[$depht-1]["line"];
		}
        
        $called_from = "<br /><br />executed in: ".$trace[0]["file"]."<br />Line: ".$trace[0]["line"]."<br />";
		
        $string = str_replace(array("__error", "__file", "__line", "__function", "__called_from"), array($error, $file, $line, $function, $called_from), $string);
		echo $string;
		die();
	}
	
/*	function shutdown()
	{
		if(!$GLOBALS["clear_shutdown"])
		{
			$string = "<HTML><HEAD><STYLE type='text/css'><!-- td {font: 8pt verdana,arial,sans-serif; color:#707070;} --> </STYLE></HEAD>"
					 ."<BODY bgcolor=#FFFFFF class='cl'><TABLE width=400 style='border: solid #FF0000 2px;' cellspacing=0 cellpadding=3>"
					 ."<TR><TD bgcolor=#FFFFFF style=''>__error<br /></TD></TR><TR><TD bgcolor=#FFFFFF style='font: 8pt verdana,arial,sans-serif; color:#707070;'>"
					 ."<b>Backtrace:</b><br /><i>impossible</i></td></tr></table></html></body>\n";
			$string = str_replace(array("__error"), array("<b>CRYTICAL SYSTEM ERROR</b><br />an Error occured which couldn't be fetched, therefor no debuging is possible. Please send this page to the developer."), $string);
		}
	} */
	
/*	function myErrorHandler($errno, $errstr, $errfile, $errline) 
	{
		$error  = "<b>Fatal PHP error</b><br />";
		$error .= "PHP-Report: ".$errorstr."<br />";
		$error .= "PHP-Debug-backtrace: (File)".$errfile." - (line) ".$errline."<br />";
		
		error_handler($error, false, false, $depht=2);   
	} */

	
	/**
	 * Killes the execution of the complete script flushing the point from where the kill was called
	 * @param integer $kill_lvl displayes the given element of the call-stack
	 * @param string $notice displayes a aditional notice.
	 */
	function kill($kill_lvl=0, $notice="")
	{
		$GLOBALS["clear_shutdown"] = true;
		if($kill_lvl>0) echo "<br />Called from: ".error_tracing_string($kill_lvl+1);
		if($notice) echo "<br />Notice: ".$notice;
		echo "<br /><b>PowerBB:</b> Process has been killed on ".error_tracing_string(1);
		die();
	}
	
	/********************************************************************************/
	/* String Functions                                                             */
	/********************************************************************************/
	
	/**
	* @desc converts a given string into a HTML W3C string
	* @param string     $text       given text
	* @param bool       $html       if this is set, "<" and ">" won't be translatet.
	* @param string     $charset    which charset should use the encoder to convert? (added 18.05.08)
	* @return string
	*/
	function html_entitets($text, $html=true, $charset="ISO-8859-15")
	{
		// v 0.01 - 26.09.2006
		// v 0.02 - 01.02.2008
		// v 0.03 - 18.05.2008

		if ($html)
		{
			$array_ascii = array("/&", ">", "<");
			$array_safe = array("/__s_and", "/__bigger", "/__smaller");
		}
		else
		{
			$array_ascii = array("/&");
			$array_safe = array("/__s_and");
		}
		
		// ASCII symbols which needes to be protected will be replaced by placeholders
		$text = str_replace($array_ascii, $array_safe, $text);

		// W3C Classifications
		$text = htmlentities($text, ENT_QUOTES, $charset, false);
		
		// Chars die gesichert waren und werden entschl�sselt und entsichert
		$text = str_replace($array_safe, $array_ascii, $text);

		return $text;
	}

	/**
	* @desc prevent Powerbb-Style elements from the convertation. After PowerBB-Style convertation they will be returned to normal
	* @param string $text the string with powerbb-style elements
	* @return string
	*/
	function save_powerbb($text)
	{
		// 11.05.2008
		return str_replace(array("[", "]"), array("\0((", "\0))"), $text);   
	}
	
	/**
	* @desc 
	*/
	
	/**
	* @desc converts the output html strings and unsave saved (e.g. from PowerBB converter) items.
	* @param string $text the saved string
	* @return string poor HTML string
	*/
	function unsave($text)
	{
		// 11.05.2008
		$return = str_replace(array("\0((", "\0))"), array("[", "]"), $text);
		$return = str_replace(array("{{{", "}}}"), array("[", "]"), $text);
		$return = str_replace("\0", " ", $return);
		return $return;
	}
	
    /**
     * 
     * @param string  $string    
     * @param int     $lenght
     */
    function right_aligned($string, $lenght)
    {
        $len = $lenght - strlen($string);
        if($len < 0) $len = 0;
        return str_repeat(" ", $len).$string;
    }//function right_aligned(..)
    
    
    
    /**
	* @desc crops a string at given sice and put a ending string behind it. It cares that the last word will not devide in the middle
	* @param string $string given string to devide
	* @param int $size max lenght of the string 
	* @param string $ending_string the string to put behinde the shortend string to show it has been shortend
	*/
	function string_crop($string, $size, $ending_string="...")
	{
		if (strlen($string)>$size)
		{
			$string = str_split($string, $size);
			$addon = split(' ', $string[1], 2);
			$string = $string[0].$addon[0];
			if (isset($addon[1])) $string .= $ending_string;
		}
		return $string;
	}

	/**
	 * @desc replace ALL replacements from the strings
	 * 
	 * Quelle: http://de2.php.net/manual/de/function.str-replace.php#78759
	 * 
	 * @param string $search 
	 * @param string $replace
	 * @param string $subject
	 * @return string
	 * 
	 * @since 0.10
	 * @author Howard Yeend
	 */
	function str_replace_all($search, $replace, $subject)
	{
		while(strpos($subject,$search)!==false)
		{
			$subject = str_replace($search,$replace,$subject);
		}
		return $subject;
	}

	/**
	* @desc generats a randomical number with $lenght digits
	* @param int $length number of digits
	* @return string
	*/
	function random_number($length = 10)
	{
		// Kernel 0.07 - Bug in der Zufallsabwicklung entfernt
		$string = false;
		for ($a=0;$a<$length;$a++)
		{
			$dump = random_int(0,9);
			$string .= chr($dump+48);
		}
		return $string;
	}
	
	/**
	* Returns a random String with small and big Characters a..z,A...Z
	* 
	* @param    int        $length        number of returning Characters
	* @return    string                String with random Characters
	* @package    misc.lib
	* @since    0.07
	*/
	function random_string($length = 30)
	{
		$string = false;
        $encode_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
		for ($a=0;$a<$length;$a++)
		{
			if ($a==0) $rnd = random_int(0,51); // first sign MUST be a character
			else $rnd = random_int(0,61);
            $string .= $encode_string{$rnd};
		}
		return $string;
	}

	function bin_to_hex($bin)
	{
		   $hex = bin2hex($bin);
		   return $hex;
	}

	function hex_to_bin($hex)
	{
		$len = strlen($hex);
		return pack("H" . $len, $hex);
	}
	
	
	/**
	* @desc reduces white spaces from a string
	* @link http://www.php.net/manual/en/function.preg-replace.php
	* @param string $string     input string
	* @return string            output string
	* v0.01 - k0.11 - 25.05.2008
	*/
	function reduce_white_spaces($string)
	{
		return preg_replace('/\s\s+/', ' ', $string);
	}
	
	
	/**
	 *       seperates the string and returns the Elements as an array. if a Limit is set, it counts _backwards_.
	 *       e.g.:  seperates_backwards(".", "pascal.sample.code.pas.back", 3) => array("pascal.sampe.code", "pas", "back")
	 * @param string $delimiter  The boundary string.
	 * @param string $string     The input string 
	 * @param integer   $limit      The returned array will contain a maximum of limit elements backwards with the first element containing the rest of string
	 * @return array[int] => string the array with the seperated 
	 * v0.01 - k0.11 - 21.09.2008
	 */
	function explode_backwards($delimiter, $string, $limit = 65536)
	{
		if($limit < 1) return false;
		
		$explode = explode($delimiter, $string);

		$count = count($explode);
		$return = array();
		
		$return[0] = $explode[0];
		for($i = 1; $i < $count; $i++)
		{
			if($i <= ($count-$limit))  $return[0] = $return[0].$delimiter.$explode[$i];
			else $return[$limit-$count+$i] = $explode[$i];
		}
		
		return $return;
	}//function explode_backwards
    
    /**
     * creats a md5 code of an string and converts it to a binary string
     * @param string $string
     * @return string
     */
/*  function md5_bin($string)
    {
        $hexa = array("0"=>0, "1"=>1, "2"=>2, "3"=>3, "4"=>4, "5"=>5, "6"=>6, "7"=>7, "8"=>8, "9"=>9, "a"=>10, "b"=>11, "c"=>12, "d"=>13, "e"=>14, "f"=>15);
        $md5 = md5($string);
        
        $return = "";
        for($i=0;$i<16;$i++)
        {
            $char = $hexa[$md5{$i*2}] + $hexa[$md5{$i*2+1}]*16;
            $return .= chr($char);
        }
        
        return $return;
    } */
    
    /**
     * cut's a string at position, stors the left part in the return, the rest will be given back
     * BE CARFUL! this function changes the parameter values
     * 
     * @param string $string   !will be changed, given: the complete string, return the right part of the string
     * @param int $pos         the position where to cut the string
     * @return string          the left part of the string
     */
    function split_string(&$string, $pos)
    {
        $first = substr($string, 0, $pos);
        $string = substr($string, 0+$pos);
        
        return $first;
    }
	
	##################################################################################
	# A R R A Y   F U N C T I O N                                                    #
	##################################################################################
	
	/**
	 * returns the last element of an array without changing the array
	 * @param array $array the soruce array
	 * @return integer the last array element
	 * @since 05.12.2008
	 */
	function array_last_element($array)
	{
		$keys = array_keys($array);
		$last_element = array_pop($keys);
		return $array[$last_element];
	}

    /**
     * compairs twp array with the build in PHP array_analyse and counts the result
     * @param   array     the soruce array
     * @return  int       the number of simular entrys in this 
     * @since 04.01.2010 
     */
    function array_compair_num($array1, $array2)
    {
        $result = array_intersect($array1, $array2);
        return count($result);
    }
    
    
	##################################################################################
	# File handle                                                                    #
	##################################################################################
	
	function read_file($file)
	{
		// Liest eine Datei ein, und enfernt die f�r das Prasen ungeeignete Zeichen.
		// Kernel 0.07 - 28.09.2006 - (Setup)
		//
		// input:     $file            string        Pfad und name der Datei
		// output:    ~nothing~
		// return:                    array        Eingelesene Datei als Array

		$file = file($file);
		$c = count($file);
		for($i = 0; $i < $c; $i++)
		{
			$file[$i] = chop($file[$i]);
		}
		return $file;
	}

	/**
	* @desc Writes an array to a text/plain file
	* @param $file_path string Filename (including possibly needed path)
	* @param $file array contend of the file
	*/
	function write_file($file_path, $file)
	{
		// mit ver�nderungen von http://de.php.net/manual/de/function.file.php �bernommen
		// Kernel 0.07 - 04.10.2006 - (setup)
		//               26.03.2007 - fehlerkorrktur (eine leere Zeile hatte ein Warning erzeugt)
		// input:     $file_path        string        Pfad der Datei
		//            $file            array        Die datei als Array, wie sie aus file() gekommen ist.
		$handel = fopen($file_path, "w");
		foreach($file as $line)
		{
			$r = strlen($line);
			if ($r == 0)
			{
				$line = "\r\n";
			}
			elseif ($line[$r-1] != "\n")
			{
				   $line[$r] = "\r";
				   $line[$r+1] = "\n";
			}
			elseif($line[$r-2] != "\r")
			{
				   $line[$r-1] = "\r";
				   $line[$r] = "\n";
			}
			fputs($handel, $line); //place $line back in file
		}
		fclose($handel);
	}

	/**
	* @desc search for a string withing every line of an array.
	* @param array  $haystack   the given array
	* @param string $needle     string to search
	* @return   bool|array      false if nothing was found | array with keys in which values the string was found.
	* @since 01.10.2006
	*/
	
	function search_in_array($haystack, $needle)
	{
		$return = array();
		while (list($key, $line) = each($haystack))
		{
			if (strpos($line, $needle) !== false) $return[] = $key;
		}
		return $return;
	}

	function insert_line_in_array($line, $line_nr, $array)
	{
		// 05.10.2006
		// input:
		//       $line        string            Die einzuf�gende Zeile
		//        $line_nr    int                Zeilennummer f�r die einzuf�gende Zeile (erste Zeile ist 0)
		//        $array        array->string    Array in das die Zeile einge�fgt werden soll
		// output:
		//        ~nothing~
		// return:
		//        array->string                Das Array mit der neu eingef�gten Zeile

		$c = count($array);
		for ($i=($c-1);$i>=$line_nr;$i--)
		{
			$array[$i+1] = $array[$i];
		}
		$array[$line_nr] = $line;
		return $array;
	}
	
	/**
	* @desc converts php.ini size settings (eg. 8M) to calculatebal size
	* @param string $val the PHP.INI setting (eg. 8M)
	* @return int kbytesize
	*/
	function return_kbytes($val)
	{
		$val = trim($val);
		$last = strtolower($val{strlen($val)-1});
		switch($last)
		{
			// The 'G' modifier is available since PHP 5.1.0
			case 'g':
				$val *= 1024;
			case 'm':
				$val *= 1024;
			case 'k':
				$val *= 1;
				break;
			default:
				$val = floor($val / 1024);
		}
		return $val;
	}
	
	/**
	 * @desc searchs in a given path and returns all files either or the files with the given extension
	 */
	function get_dir($path, $ext=false)
	{
		$return = array();
		if ($handle = opendir($path))
		{
			while (false !== ($file = readdir($handle)))
			{
				if(!$ext || ($ext && strpos(".".$ext) !== 0))
				{
					$return[] = $file;
				}
			}
			closedir($handle);
		}           
		return $return;
	}

	/**
	 * @desc reads a table file
	 * @param string $filename the file with path relativ to the root file.
	 * @return bool|array[int][string]=>string (false if not compatible)|else table
	 * @version 0.01 - kernel 0.12 - 02.11.2008
	 */
	function read_table_file($filename) 
	{
		if(!file_exists($filename))
		{
			echo "<b>Fatal PowerBB Error</b> - file not found  - \"<tt>$filename</tt>\"";
			kill(1);
		}
		
		// read file into storage
		$file_handle = fopen($filename, "rb");
		$table = fread($file_handle, filesize($filename));
		fclose($file_handle);   
		
		// separate header
		list($table_header, $table) = explode("\4", $table, 2);
		
		// interpret header
		$array = explode("\1", $table_header);
		if(($array[0] != "pbb_table") || ($array[1] != "0.01"))
		{
			$return = false;
		}
		
		// read table head
		$head = explode("\0", $array[2]);
		
		// read table contend
		$array = explode("\4", $table);
		$return = array();
		foreach($array as $line_no=>$line)
		{
			$cells = explode("\0", $line);
			foreach($cells as $num=>$col)
			{
				$return[$line_no][$head[$num]] = $col;
			}
		}//foreach
		
		return $return;
	}//function read_table_file
    
    
    /**
     * Write a file to disk
     * 
     * @param string                     $path                     file path and name to store the file
     * @param array[int][int|string]     $table                    the contend of the file to store.
     */
    function creat_table_file_wh($path, $table_header, $array)
    {
        $contend = "pbb_table\1"."0.01\1";
        
        $first = true;
        foreach($table_header as $key)
        {
            if($first) $first = false;
            else $contend .= "\0";
            $contend .= $key;
        }
        
        $contend .= "\4";
        $first = true;
        foreach($array as $line)
        {
            if($first) $first = false;
            else $contend .= "\4";
            $first_2 = true;
            foreach($line as $col)
            {
                if($first_2) $first_2 = false;
                else $contend .= "\0";
                $contend .= $col;
            }
        }
        
        $file = fopen($path, "wb");
        fwrite($file, $contend);
        fclose($file); 
    }
        
        /*
    function creat_table_file($path, $table)
    {
        $contend = "pbb_table\1"."0.01\1";
        
        if(empty($table)) return;
        $table_header = current($table);
        
        $first = true;
        foreach($table_header as $key => $value)
        {
            if($first) $first = false;
            else $contend .= "\0";
            $contend .= $key;
        }//foreach
        
        $contend .= "\4";
        $first = true;
        foreach($array as $line)
        {
            if($first) $first = false;
            else $contend .= "\4";
            $first_2 = true;
            foreach($line as $col)
            {
                if($first_2) $first_2 = false;
                else $contend .= "\0";
                $contend .= $col;
            }//foreach
        }//foreach
        
        $file = fopen(_path.$path, "wb");
        fwrite($file, $contend);
        fclose($file); 
    }
    */
    
    /**
     * Erzeugt ein Sicheres Passwort-Hash mit Salz und Pfeffer
     * 
     * Das Salz wird vom System vorgegeben
     * als Pfeffer dient ein md5 Hash vom Benutzernamen  
     * 
     * @param string $name			Name des Benutzers
     * @param string $password		Passwort des Benutzers 
     * @param string $salt 		 	Salz vom System
     * @return string				Base64 endcodierter String des SHA256-Hash
     */
    function generate_password($name, $password, $salt)
    {
    	$base64_encode_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    	
    	$pepper = md5($name);
    	
    	$hash = hash("sha256", $password."-".$salt."-".$pepper);
    	
    	$return = "";
    	
    	$array = str_split($hash, 6);
    	foreach($array as $value)
    	{
    		$x = hexdec($value);
    		$a = (int) $x % 64;
    		$x = (int) floor($x / 64);
    		$b = (int) $x % 64;
    		$x = (int) floor($x / 64);
    		$c = (int) $x % 64;
    		$x = (int) floor($x / 64);
    		$d = (int) $x;
    		$return .= $base64_encode_table[$a].$base64_encode_table[$b].$base64_encode_table[$c].$base64_encode_table[$d];
    	}//foreach
    	
    	return $return;
    }//function generate_password($name, $password)
    
    
    
?>