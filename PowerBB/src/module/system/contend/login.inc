<?php 

function prepro_login($prepro_form)
{
	global $user;
	
	$result = $user->try_login($prepro_form["user"]->value, $prepro_form["pass"]->value);
	
	if($result)
	{
		$prepro_form["user"]->add_errror("__lang_user_pass_wrong;");
	}
}



function preprocessor($command, $parameter)
{
	switch($command)
	{
		case "login":
			prepro_login ($parameter);
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
		$contend = "__lang_login_windows; \n";
		
		$form = $form_manager->get_new_form("login");
	
		$feld = $form->get_new_feld("user");
		$feld->bezeichner("__lang_user_name;");
		$feld = $form->get_new_password("pass");
		$feld->bezeichner("__lang_user_pass;");
		$feld = $form->get_new_submit("log_sub");
		$feld->bezeichner("__lang_submit;");
	
		$contend .= $form->get_form_string();
	}//if
	else
	{
		$contend = "__lang_welcome_login;";	
	}
	
	return window("__lang_login;", $contend);
	
}



?>