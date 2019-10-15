<?php 
/***************************************************************************************************/
/**                                                                                               **/
/**            P R O T O T Y P                                                                    **/
/**                                                                                               **/
/***************************************************************************************************/
	
/////////////////////////////////////////////////////////////////////////////////
//  Einfache Engabefeld
// 
	class cForm_Feld
	{
		protected  	$name			= "";
		
		protected	$html_ident     = "";		
		
		protected	$def_value		= "";
		
		protected	$get_value		= false;
		
		protected	$txt_bezeichner	= "";
		
		protected	$txt_desc		= "";
		
		protected	$txt_error  	= array();
		
		protected	$txt_kommentar 	= "";
		
		protected 	$old_value		= "";
		
		public function __construct($html_ident, $name)
		{
			$this->name = $name; 
			$this->txt_bezeichner = $name;
			$this->html_ident = $html_ident;
		}
		
		public function bezeichner($text)
		{
			$this->txt_bezeichner = $text;
		}
		
		public function beschreibung($text)
		{
			$this->txt_desc = $text;
		}
		
		public function add_errror($text)
		{
			$this->txt_error[] = $text;
		}
		
		public function kommentar($text)
		{
			$this->txt_kommentar = $text;
		}
		
		public function	default_value($text)
		{
			$this->def_value = $text;	
		}
		
		public function set_prepro_value($value)
		{
			$this->get_value = $value;
		}
		
		public function clear_error_list()
		{
			$this->txt_error = array();
		}
		
		public function get_prepro_value()
		{
			return $this->get_value;
		}
		
		public function	default_value_override($text)
		{
			$this->def_value = $text;
			$this->get_value = false;
		}

		public function get_name()
		{
			return $this->name;
		}

		public function get_error()
		{
			return $this->txt_error;
		}
		
		public function load_old_form($old_form)
		{
			$this->def_value = $old_form->value;
			$this->txt_error = $old_form->get_error();
		}
		
		public function get_html()
		{
			// Vorbereiten der Daten
			$value = $this->get_value;
			if(!$value) $value = $this->def_value;
			
			// Zähle die zusätzlichen Zeilen, die nenötigt werden.
			$count = 1;
			
			if($this->txt_desc) $count++;
			if(count($this->txt_error) > 0) $count++;
			if($this->txt_kommentar) $count++;
			
			$string  = "\n    <tr id='form_table_top_tr'>";
			
			$string .= "<td id='form_table_left' rowspan=".$count.">".$this->txt_bezeichner."</td>";
			$string .= "<td id='form_table_right'><input type='text' id='form_table_input_default' name='".$this->html_ident."' ";
			if($value)
			{
				$string .= " value='".$value."'";
			}//if
					
			$string .= "/></td>";
			$string .= "</tr>";
			
			if($this->txt_desc)
			{
				$string .= "\n<tr><td id='form_table_right'>".$this->txt_desc."</td></tr>";
			}
			
			if(count($this->txt_error) > 0)
			{
				$string .= "\n<tr><td id='form_table_right_error'><ul id='form_table_error_ul'>";
				foreach($this->txt_error as $value)
				{
					$string .= "<li>".$value."</li>";
				}
				
				$string .= "</ul></td></tr>";
			}
			
			if($this->txt_kommentar)
			{
				$string .= "\n<tr><td id='form_table_right'>".$this->txt_kommentar."</td></tr>";
			}
			
			return $string;
		}//public function get_html()
	
		
		public function __get($key)
		{
			$return = "";
			
			switch ($key)
			{
				case "value":
					$return = $this->get_value;
					break;
				default:
					echo "<FONT color=red>FATALER FEHLER</font> in <TT>cForm_manager</TT>. Es wurde versucht auf die Eigenschaft <TT>".$key."</TT> zuzugreifen. Diese wurde jedoch nicht definiert.<br>";
					kill(1);
			}//switch
			
			return $return;
		}//public function __get($key)
		
	}//class cForm_Feld
	
	
	/////////////////////////////////////////////////////////////////////////////////
	//  Einfaches Passwortfeld
	//
	class cForm_Password extends cForm_Feld
	{
		
		public function get_html()
		{
			// Zähle die zusätzlichen Zeilen, die nenötigt werden.
			$count = 1;
			
			if($this->txt_desc) $count++;
			if(count($this->txt_error) > 0) $count++;
			if($this->txt_kommentar) $count++;
			
			$string  = "\n    <tr id='form_table_top_tr'>";
			
			$string .= "<td id='form_table_left' rowspan=".$count.">".$this->txt_bezeichner."</td>";
			$string .= "<td id='form_table_right'><input type='password' id='form_table_input_default' name='".$this->html_ident."' ";
			$string .= "/></td>";
			$string .= "</tr>";
			
			if($this->txt_desc)
			{
				$string .= "\n      <tr><td id='form_table_right'>".$this->txt_desc."</td></tr>";
			}
			
			if(count($this->txt_error) > 0)
			{
				$string .= "\n      <tr><td id='form_table_right_error'><ul id='form_table_error_ul'>";
				foreach($this->txt_error as $value)
				{
					$string .= "        <li>".$value."</li>";
				}
				
				$string .= "      </ul></td></tr>";
			}
			
			if($this->txt_kommentar)
			{
				$string .= "\n<tr><td id='form_table_right'>".$this->txt_kommentar."</td></tr>";
			}
			
			return $string;
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////
	//  Einfaches Passwortfeld
	//
	class cForm_Submit extends cForm_Feld
	{
		public function get_html()
		{
			// Zähle die zusätzlichen Zeilen, die nenötigt werden.
			$string  = "\n    <tr id='form_table_top_tr'>";
			
			$string .= "<td id='form_table_left_submit' colspan='2'><input type='submit' id='form_table_submit_default' name='".$this->html_ident."' value='".$this->txt_bezeichner."'/></td>";
			$string .= "</tr>";
			
			return $string;
		}//function get_html()
	}
	
	
?>