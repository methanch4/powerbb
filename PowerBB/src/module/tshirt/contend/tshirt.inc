<?php 

include "inc/cTShirt.inc";


/**
 * @var cTShirt_shop $shirtigo 
 */
$shirtigo = null;	
	
	function change_data()
	{
		/**
		 * @global cUser_manager $user
		 * @global cForm_manager $form_manager
		 */
		global $user, $form_manager;
		
		$contend = "";
		
		$inhalt  = "__lang_daten;<br><br>";
		
		$name = $user->get_setting("et_tshirt", "name");
		
		$form = $form_manager->get_new_form("change_shirt_data");
		$form->add_link_parameter("sid", "1");
		
		$feld = $form->get_new_feld("user");
		$feld->bezeichner("__lang_user_name;");
		$feld->beschreibung("__lang_user_name_desc;");
		$feld->default_value($name);
		
		$feld = $form->get_new_submit("log_sub");
		$feld->bezeichner("__lang_submit;");  
		
		$inhalt .= $form->get_form_string();
		
		$inhalt .= "<br><br>".generate_link($_GET["cid"], "__lang_back_to_main;", array("sid"=>"0"));
		
		$contend .= window("__lang_title_daten;", $inhalt);
		return $contend;
	} // */
	
	/******************************************************************************************************/
	/**                                                                                                  **/
	/**                                A D M I N                                                         **/
	/**                                                                                                  **/
	/******************************************************************************************************/
	
	function admin_product_details()
	{
		$product_id = $_GET["product_id"];
		
		$inhalt = "Es werden alle gespeicherten Daten zu diesem Produkt angezeigt.<br><br>";
		
		$client = login_to_shirtigo();
		$data1 = $client->GET("base-products/".$product_id);
		$inhalt .= "<br /><br />__lang_admin_pd_detail;<br />\n";
		$inhalt .= "<table id='templ_new_table'>\n";
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_name</td><td id='templ_table_right'>".$data1->name."</td>\n";
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_is_activ</td><td id='templ_table_right'>".($data1->is_active?"<font color='green'>verfügbar</font>":"<font color='red'><b>nicht verfügbar</b></font>")."</td>\n";
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_desc</td><td id='templ_table_right'>".$data1->checkout_description."</td>\n";
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_desc_detail</td><td id='templ_table_right'>".$data1->description."</td>\n";
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_manufacturer</td><td id='templ_table_right'>".$data1->manufacturer->name."</td>\n";
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_price</td><td id='templ_table_right'>";
		foreach($data1->price->data as $val2)
		{
			$inhalt .= "(".$val2->type.") -&gt; ".sprintf("%01.2f", $val2->price->amount/$val2->price->currency_factor)." ".$val2->price->currency_symbol;
			$inhalt .= " &nbsp; &nbsp; ";
		}
		$inhalt .= "</td>\n";
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_color</td><td id='templ_table_right'>\n";
		$inhalt .= "     <table>\n";
		foreach($data1->colors->data as $val)
		{
			$inhalt .= "     <tr>\n";
			$inhalt .= "       <td width=20 bgcolor='".$val->color_hex."'>&nbsp;</td>\n";
			$inhalt .= "       <td width=200>".$val->id." -&gt; ".$val->color."</td>\n";
			$array = [false, false, false, false, false, false, false, false, false, false, false, false, false];
			foreach($val->sizes as $val2)
			{
				if($val2->is_available) $array[$val2->id] = true;
			}
			if($array[0])  $inhalt .= "<td><font id='temp_et_size_aktiv'>XXS</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>XXS</font></td>";
			if($array[1])  $inhalt .= "<td><font id='temp_et_size_aktiv'>XS</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>XS</font></td>";
			if($array[2])  $inhalt .= "<td><font id='temp_et_size_aktiv'>S</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>S</font></td>";
			if($array[3])  $inhalt .= "<td><font id='temp_et_size_aktiv'>M</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>M</font></td>";
			if($array[4])  $inhalt .= "<td><font id='temp_et_size_aktiv'>L</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>L</font></td>";
			if($array[5])  $inhalt .= "<td><font id='temp_et_size_aktiv'>XL</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>XL</font></td>";
			if($array[6])  $inhalt .= "<td><font id='temp_et_size_aktiv'>XXL</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>XXL</font></td>";
			if($array[7])  $inhalt .= "<td><font id='temp_et_size_aktiv'>3XL</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>3XL</font></td>";
			if($array[8])  $inhalt .= "<td><font id='temp_et_size_aktiv'>4XL</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>4XL</font></td>";
			if($array[9])  $inhalt .= "<td><font id='temp_et_size_aktiv'>5XL</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>5XL</font></td>";
			if($array[10]) $inhalt .= "<td><font id='temp_et_size_aktiv'>6XL</font></td>";
			else  	 	   $inhalt .= "<td><font id='temp_et_size_inaktiv'>6XL</font></td>";
			$inhalt .= "</td>\n";
			$inhalt .= "     </tr>\n";
		}
		$inhalt .= "     </table>\n";
		$inhalt .= "</td>\n";
		foreach($data1->images->data as $val)
		{
			$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_pic ".$val->sort_position."</td><td id='templ_table_right'><img src='".$val->url."' /></td>\n";
		}
		$inhalt .= "  <tr><td id='templ_table_left'>__lang_admin_pd_size_chart</td><td id='templ_table_right'><img src='".$data1->sizechart_image."' /></td>\n";
		$inhalt .= "</table>\n";

/*		$inhalt .= "<br /><br />__lang_admin_pd_raw;<br />\n";
		$inhalt .= "<table>\n";
		$array = (array)$data1;
		ksort($array);
		foreach($array as $key => $value)
		{
			$inhalt .= "<tr><td valign=top width=200>".$key."</td>\n";
			$inhalt .= "<td>\n";
			if(is_object($value))
			{
				if(isset($value->data)) $value = $value->data;
				foreach($value as $key1 => $value1)
				{
					$inhalt .= $key1." => ";
					if(is_object($value1))
					{
						foreach($value1 as $key2 => $value2)
						{
							$inhalt .= $key2."->".bool($value2); 
						}
					}
					else
					{
						$inhalt .= bool($value1);
					}
					
					$inhalt .= "<br>\n";
				}
			}
			else $inhalt .= bool($value);
			$inhalt .= "</td></tr>\n";
		}// foreach */
		$inhalt .= "</table>\n";
		
		$inhalt .= "<br><br>".generate_link($_GET["cid"], "__lang_new_product;", array("sid"=>"13", "product_id"=>$product_id));
		$inhalt .= "<br><br>".generate_link($_GET["cid"], "__lang_back_to_main;", array("sid"=>"0"));
		
		$contend = window("Produkt Details zu $product_id", $inhalt);
		return $contend;
	}
	
	
	function admin_list_of_products()
	{
//		$inhalt = "angezeigt werdne nur Produkte die in Schwarz verfügbar sind.";
		
/*		$client = login_to_shirtigo();
		
		$data = $client->GET("base-categories")->data;
		$data1 = $client->GET("base-products")->data;
		
		// Lade eine Liste von ALLEN Produktkategorien von Shirtigo. 
		foreach($data as $category)
		{
			$inhalt .= "<br /><br /><b><tt>".$category->id."</TT> - ".$category->name."</b><br />";
			
			$inhalt .= "<table>";
			$inhalt .= "<tr>";
			$inhalt .= " <td width=75><b>PID</b></td>";
			$inhalt .= " <td width=50>Status</td>";
			$inhalt .= " <td width=150>Name</td>";
			$inhalt .= " <td width=100>Bild</td>";
			$inhalt .= " <td width=75>CID</td>";
			$inhalt .= " <td width=250>Gr&ouml;&szlig;enID => Gr&ouml;&szlig;e</td>";
			$inhalt .= " <td width=100>Basispreis</td>";
			$inhalt .= "</tr>";
			
			// Lade die Liste der Spezifichen Produkte der jeweiligen Kathegorie
			foreach($data1 as $product)
			{
				if($product->category_id != $category->id) continue;
				$colors = $product->colors->data;

				$color_ok = false;
				$color_v = false;
				foreach($colors as $color_value)
				{
					if(($color_value->color == "Black") || ($color_value->color == "Schwarz") || ($color_value->color == "Dark Heather Grey"))
					{
						$color_ok = true;
						$color_v = $color_value;
						break;
					}
				} 
				
				$inhalt .= "<tr>";
				$inhalt .= " <td><b>".generate_link($_GET["cid"], "$product->id", array("sid"=>"12", "product_id"=>$product->id))."</b></td>";
				$inhalt .= " <td>";
				if($product->is_active)
				{
					if($color_ok)
					{
						$inhalt .= "<b><font color='green'>OK</font></b>";
					}
					else
					{
						$inhalt .= "<i><font color='red'>nfc</font></i>";
					}
				}
				else
				{
					$inhalt .= "<i><font color='gray'>nn</font></b>";
				}
				$inhalt .= " </td>";
				$inhalt .= " <td>".$product->name."</td>";
				$inhalt .= " <td><a href='".$product->images->data["0"]->url."' target='_blank' onClick='if (popup(this)) { return true; }'>Bild</a></td>";
				
				if($color_ok)
				{
					$inhalt .= " <td>";
					$inhalt .= $color_v->id;
					$inhalt .= " </td>";
					$inhalt .= " <td>";
					foreach($color_v->sizes as $size_value)
					{
						$inhalt .= $size_value->id."->".$size_value->size." &nbsp; &nbsp; ";
					}
					$inhalt .= " </td>";
					$price = $product->price->data[0]->price;
					$inhalt .= " <td>".sprintf("%01.2f", $price->amount/$price->currency_factor).$price->currency_symbol." (".$product->price->data[0]->type.")</td>";
				}
				else
				{
					$inhalt .= "<td colspan=3 align=center><i>nfc</i></td>";
				}
				$inhalt .= "</tr>";
			}
			$inhalt .= "</table>";
		} // */
		
		$inhalt  = "__lang_admin_lop_list;";
		
		
		
		$inhalt .= "<br><br>".generate_link($_GET["cid"], "__lang_back_to_main;", array("sid"=>"0"));
		$contend = window("__lang_title_daten;", $inhalt);
		
		return $contend;
	}
	
	function prepro_change_shirt_data($prepro_form)
	{
		global $user;
		
		if(strlen($prepro_form["user"]->value) > 2)
		{
			$user->set_setting("et_tshirt", "name", $prepro_form["user"]->value);
		}//if
	}  
	
	function preprocessor($command, $parameter)
	{
		switch($command)
		{
			case "change_shirt_data":
				prepro_change_shirt_data ($parameter);
				break;
		}//switch
	}
	
	
	function get_contend_default()
	{
		/**
		 * Erzeugt ein neues Projekt bei Shirtigo
		 * @param string $local_project_id
		 * @return string
		 */
		function create_new_shirtigo_project($client, $user_name)
		{
			/**
			 * @global cUser_manager $user
			 */
			global $user;
			
			echo "no Porject ID";
			
			$data = [
					'name' => "ET_".$user_name."_".date("Y-m-d\_H:i:s"),
			];
			
			$project = $client->post('projects', $data);
			$local_project_id = $project->reference;
			
			$user->set_setting("et_tshirt", "shirtigo_project_id", $local_project_id);
			
			return $local_project_id;			
		}// function create_new_shirtigo_project($client, $local_project_id)
		
		
		
		/**
		 * @global cUser_manager $user
		 */
		global $user;
		
		$contend = "";
		
		$inhalt  = "__lang_daten;<br><br>";
		
		$name = $user->get_setting("et_tshirt", "name");
		
		if ($name == NULL)
		{
			$name = $user->user_name;
			$user->set_setting("et_tshirt", "name", $name);
		}

		$inhalt .= "__lang_data_stored;<br><br>";
		$inhalt .= "__lang_data_user;: <tt>".$name."</tt><br>";
		
		$local_project_id = $user->get_setting("et_tshirt", "projekt-id");
		
		if (!$local_project_id)
		{
			$local_project_id = time()."-".$name;
			$user->set_setting("et_tshirt", "projekt-id", $local_project_id);
		}
		
		$inhalt .= "__lang_data_ID;: <tt>".$local_project_id."</tt><br>";
		
		$contend .= window("__lang_title_daten;", $inhalt);
		
		/******************************************************************/

		$image_path = "./temp_pics/".$local_project_id.".png";
		if(!file_exists($image_path))
		{
			// Es wurden noch kein Bild für dieses Projekt angelegt. Erzeuge ein neues.
			$im = @imagecreatefrompng("./pics/master_front.png");
			
			// Schütze den Alpha-Kanal aus der Datei 
			imagesavealpha($im, true);
			
			$text_color  = imagecolorallocate($im, 54, 200, 54);
			$font_path = "./fonts/comicbd.ttf";
			imagettftext($im, 120, 0, 310, 1050, $text_color, $font_path, $name);
			
			// Speichere das erzeugte Bild auf der Platte
			imagepng($im, $image_path, -1);
			
			// Lösche das Bild aus dem Arbeitsspeicher. 
			imagedestroy($im);
		}

		$inhalt = "";
		
/*		$inhalt  = "<div style='font-size: large;'><b>__lang_vorschau;</b></div><br />";
		$inhalt .= "__lang_vorschau_desc;<br>";		
/*		$inhalt .= "<table><tr><td style='background-image:url(\"./pics/background_shirt.png\"); vertical-align: top;' width=440 height=580>\n"
				 ."<img src='".$image_path."' style='padding-left:220; padding-top: 160px; width:100px;'/>\n"
				 ."</td>"
				 ."<td style='background-image:url(\"./pics/background_shirt_back.png\"); vertical-align: top;' width=440 height=580></td>"
				 ."</tr></table>"; 
		$contend .= "<br><br>";// */
/*		$contend .= window("__lang_title_preview;", $inhalt); */
		
		$inhalt .= "<div style='font-size: large;'><b>__lang_shop_title;</b></div><br />";
		$inhalt .= "__lang_shop_desc;<br>";
		
		/**********************************************************************************
		 * Verbinde mich mit der Datenbank von Shirtigo
		 */
//		$client = login_to_shirtigo_FDghdfgh(sdfsfg);
		
		// Lade die Projekt_ID aus der Datenbank.
		$shirtigo_project_id = $user->get_setting("et_tshirt", "shirtigo_project_id");
		
		if(!$shirtigo_project_id)
		{
			// Es wurde noch kein Projekt erzeugt. Erzeuge ein neues, für den Kunden passendes Projekt
			$shirtigo_project_id = create_new_shirtigo_project($client, $name);
		}//if
			
		// Versuche das in userer Datenbank hinterlegte Projekt bei ShirtiGo zu finden.
		$string = "/projects/".$shirtigo_project_id;
		echo $string."<br>";
		try
		{
			//$data = $client->get($string);
		}
		catch (Exception $e)
		{
			// Auf das Projekt konnte nicht zugegriffen werden. Dies kann mehrere Gründe haben...
			// Da eine fehlerbehandlung an dieser Stelle schwer ist, wird ein neues Projekt angelegt.
			$shirtigo_project_id = create_new_shirtigo_project($client, $name);
		}
		
		//kill();
		
		$inhalt .= "";
		if($user->is_admin())
		{
			$inhalt .= "__lang_admin_options:<br>";
			$inhalt .= generate_link($_GET["cid"], "__lang_list_of_products;", array("sid"=>"11"));
		}
		
		$contend .= "<br><br>";
		$contend .= window("__lang_title_preview;", $inhalt);
		return $contend;
	}
	
	function get_conted($param = array())
	{
		/**
		 * @global cUser_manager $user
		 */
		global $user;
		
		/**
		 * @global cTShirt_shop $shirtigo 
		 */
		global $shirtigo;

		// Initialisiere die Verhindung von Datenbank, Programm und Geschefft.
		$shirtigo = new cTShirt_shop();
		
		// Initialisiere die Rüchgabevariable
		$return = "";
		
		// Lade die Übergabevariable aus dem Übergabestring.
		// !!! HINWEIS: Die Übergabevariable ist NICHT sicher !!!
		if(!isset($_GET["sid"])) $sid=0;
		else $sid = $_GET["sid"];
		
		// Sollte die Variable kein gültiges Format haben, würd sie neu initialisiert.
		if(!is_numeric($sid)) $sid = 0;
		
		switch($sid)
		{
			case 11:
				if($user->is_admin()) $return = admin_list_of_products();
				break;			
			case 12:
				if($user->is_admin()) $return = admin_product_details();
				break;
			case 13:
				if($user->is_admin()) $return = admin_product_setup_product();
				break;
			case 0:
			default:
				$return = get_contend_default();
				break;
		}//switch
		
		return $return;
	}//function get_conted

?>