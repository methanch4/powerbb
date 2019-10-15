<?php 

	/**
	 * @global cUser_manager $user
	 * @param array(string)->string $prepro_form
	 */
	
	function prepro_register($prepro_form)
	{
		global $user;
		
		$error = false;
		
		if(strlen($prepro_form["user"]->value) < 4)
		{
			$error = true;
			$prepro_form["user"]->add_errror("__lang_username_to_short;");
		}//if

		if($prepro_form["user"]->value[0] == "_")
		{
			$error = true;
			$prepro_form["user"]->add_errror("__lang_username_fist_must_not__;");
		}//if
		
		if(strlen($prepro_form["pass1"]->value) < 6)
		{
			$error = true;
			$prepro_form["pass1"]->add_errror("__lang_password_to_short;");
		}
		
		if($prepro_form["pass1"]->value != $prepro_form["pass2"]->value)
		{
			$error = true;
			$prepro_form["pass1"]->add_errror("__lang_password_does_not_match;");
		}
		
		if(strlen($prepro_form["eMail"]->value) < 6)
		{
			$error = true;
			$prepro_form["eMail"]->add_errror("__lang_email_to_short;");
		}
		
		if(!$error)
		{
			$user->register_user($prepro_form["user"]->value, $prepro_form["pass1"]->value, $prepro_form["eMail"]->value);
		}
		
	}



function preprocessor($command, $parameter)
{
	switch($command)
	{
		case "register":
			prepro_register ($parameter);
			break;
	}//switch
}


function get_conted($param = array())
{
	/**
	 * @global cUser_manager $user
	 * @global cSystem $system
	 * @global cForm_manager $form_manager
	 */
	global $user, $system, $form_manager;
	
	if($user->user_id == $system->get_config("user", "guest_user_id"))
	{
		$contend  = "__lang_greeting;";
		
		$form = $form_manager->get_new_form("register");
		
		$feld = $form->get_new_feld("user");
		$feld->bezeichner("__lang_user_name;");
		
		$feld = $form->get_new_password("pass1");
		$feld->bezeichner("__lang_password;");
		$feld->beschreibung("__lang_password_desc;");
		
		$feld = $form->get_new_password("pass2");
		$feld->bezeichner("__lang_password_repeat;");
		
		$feld = $form->get_new_feld("eMail");
		$feld->bezeichner("__lang_email;");
		
		$feld = $form->get_new_submit("log_sub");
		$feld->bezeichner("__lang_submit;");
		
		$contend .= $form->get_form_string();
	}
	else
	{
		$contend = "__lang_welcome_new_user.";
	}
	
	return window("__lang_register_windows_title;", $contend);
	
}







?>