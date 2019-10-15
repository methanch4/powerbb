<?php 

	include "cForm_elements.inc";

	class cForm
	{
		
		/**
 		 * @var cForm_Feld
		 */
		private 	$form_liste = array();
		
		private		$manager_ident = "";
		
		private		$prepro_action = "";
		
		private 	$old_form = array();
		
		private		$link_parameter = array();
		
		
		public function __construct($prepro_action, $ident, $old_form = array())
		{
			$this->prepro_action = $prepro_action;
			$this->manager_ident = $ident;
			$this->old_form = $old_form;
		}
		
		public function get_ident()
		{
			return $this->manager_ident;
		}
		
		/**
		 * Erzeugt ein neues Namensfeld
		 * @param String $name
		 * @return cForm_Feld
		 */
		public function get_new_feld($name)
		{
			do
			{
				$string = random_string(8);
			}
			while(isset($this->form_liste[$string]));
			
			$this->form_liste[$string] = new cForm_Feld($string, $name);
			if(isset($this->old_form[$name]))
			{
				$this->form_liste[$string]->load_old_form($this->old_form[$name]);
			}
			
			return $this->form_liste[$string];
		}
		

		/**
		 * Erzeugt ein neues Namensfeld
		 * @param String $name
		 * @return cForm_Feld
		 */
		public function get_new_password($name)
		{
			do
			{
				$string = random_string(8);
			}
			while(isset($this->form_liste[$string]));
			
			$this->form_liste[$string] = new cForm_Password($string, $name);
			if(isset($this->old_form[$name]))
			{
				$this->form_liste[$string]->load_old_form($this->old_form[$name]);
			}
			
			return $this->form_liste[$string];
		}
		
		/**
		 * Erzeugt ein neues Namensfeld
		 * @param String $name
		 * @return cForm_Feld
		 */
		public function get_new_submit($name)
		{
			do
			{
				$string = random_string(8);
			}
			while(isset($this->form_liste[$string]));
			
			$this->form_liste[$string] = new cForm_Submit($string, $name);
			
			return $this->form_liste[$string];
		}
		
		/**
		 * Erzeugt den für die darstellung der Form notwendige HTML-Code
		 * @return string
		 * 
		 * @global cUser_manager	$user
		 * @global cSystem  		$system
		 */
		public function get_form_string()
		{
			global $user, $system;
			
			$string = "";
			
			$string .= "\n<form action='./".$system->get_config("main", "index")."?cid=".$user->get_cid()."&pid=".$this->manager_ident;
			
			foreach ($this->link_parameter as $key => $value)
			{
				$string .= "&".$key."=".$value;
			}
			
			$string .= "' method='post' accept-charset='UTF-8'>";
			$string .= "\n  <div id='form_table_div'><table id='form_table_default'>";

			foreach($this->form_liste as $value)
			{
				$string .= $value->get_html();
			}//foreach
			
			$string .= "\n  </table></div>";
			$string .= "\n</form>\n";
			
			return $string;
		}
	
		public function add_link_parameter($key, $value)
		{
			$this->link_parameter[$key] = $value;
		}
		
		public function __get($name)
		{
			$return = false;
			
			switch($name)
			{
				case "prepro_action":
					$return = $this->prepro_action;
					break;
				case "form_liste":
					$return = $this->form_liste;
					break;
				default:
					echo "<FONT color=red>FATALER FEHLER</font> in <TT>cForm</TT>. Es wurde versucht auf die Eigenschaft <TT>$name</TT> zuzugreifen. Diese wurde jedoch nicht definiert.<br>";
					kill(1);
			}//switch
			
			return $return;
		}
	}//class cForm

	
	
	class cForm_manager
	{
		/**
		 * Speichert die in diesem Seitenaufbau verwendeten Formen.
		 * @var array(String => cForm)
		 */
		private $list = array();
		
		private $prepro = array();
		
		private $prepro_action = "";
		
		public function __construct()
		{
			// Liegen Preorozessorinformationen vor?
			if(isset($_GET["pid"]))
			{
				// Lade die ID				
				$pid = $_GET["pid"];
				
				// Ist die ID gespeichert?
				if(isset($_SESSION["form"][$pid]))
				{
					// Lade die Forminformationen
					$form = $_SESSION["form"][$pid];
					
					$this->prepro_action = $form->prepro_action;
					
					// Ordne die gespeicherten Form-informatione den gesendeten zu
					foreach($form->form_liste as $key => $value)
					{
						if(isset($_POST[$key]))
						{
							$this->prepro[$value->get_name()] = $value;
							$this->prepro[$value->get_name()]->set_prepro_value(htmlentities($_POST[$key], ENT_QUOTES | ENT_HTML401, "UTF-8"));
							$this->prepro[$value->get_name()]->clear_error_list();
						}//if
					}//foreach

				}//if
				else
				{
					echo "Fehler: die PID ist nicht (mehr) bekannt.";
				}
			}//if
			
			// Lösche alle alten Formulare. Ihre Gültigkeit ist an diesem Punkt erloschen
			$_SESSION["form"] = array();
		}
		
		/**
		 * Erzeugt eine neue Form
		 * @param String $prepro
		 * @return cForm
		 */
		public function get_new_form($prepro)
		{
			$ident = random_string(7);
			
			if($prepro == $this->prepro_action)
			{
				$this->list[$ident] = new cForm($prepro, $ident, $this->prepro);
			}
			else
			{
				$this->list[$ident] = new cForm($prepro, $ident);
			}
			
			$_SESSION["form"] = $this->list;
			
			return $this->list[$ident];
		}
		
		
		public function __get($name)
		{
			$return = false;
			
			switch($name)
			{
				case "prepro_action":
					$return = $this->prepro_action;
					break; // */
				case "prepro":
					$return = $this->prepro;
					break;				
				default:
					echo "<FONT color=red>FATALER FEHLER</font> in <TT>cForm_manager</TT>. Es wurde versucht auf die Eigenschaft <TT>$name</TT> zuzugreifen. Diese wurde jedoch nicht definiert.<br>";
					kill(1);
			}//switch
			
			return $return;
		}
		
	}//class cForm_manager
	
	
?>