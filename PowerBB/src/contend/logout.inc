<?php 

/**
 * @global cUser_manager $user
 * @param array(string)->string $prepro_form
 */

function prepro_logout($prepro_form)
{
	global $user;
	
	$user->logout();	
}



function preprocessor($command, $parameter)
{
	switch($command)
	{
		case "logout":
			prepro_logout ($parameter);
			break;
	}//switch
}

function get_conted($param = array())
{
	global $form_manager;
	
	$contend  = "__lang_logout;";
	
	$form = $form_manager->get_new_form("logout");
	
	$feld = $form->get_new_submit("log_sub");
	$feld->bezeichner("__lang_logout;");
	
	$contend .= $form->get_form_string();
	
	return window("__lang_register_windows_title;", $contend);
	
}



?>