<?
if (!defined("__CONFIG__")) 
{
	define(__CONFIG__,TRUE);

	function wbxsession($SessionName,$SessionValue="UNDEFINED")
	{
		if(isset($_SESSION[$SessionName])&&$_SESSION[$SessionName]!="")
		{
			global $$SessionName;
			if($SessionValue=="UNDEFINED")
				return $_SESSION[$SessionName];
			else
			{	
				$$SessionName=$SessionValue;
				$_SESSION[$SessionName]=$SessionValue;
			}
		}
		else
		{
			if($SessionValue!="UNDEFINED")
			{
				global $$SessionName;
				$$SessionName=$SessionValue;
				$_SESSION[$SessionName]=$SessionValue;
			}
		}
	}

	session_start();

	if(!isset($glbEnvSITE_PATH)||$glbEnvSITE_PATH=="")
		wbxsession("glbEnvSITE_PATH", "/prog");
	if(!isset($glbEnvHomeFolder)||$glbEnvHomeFolder=="")
		wbxsession("glbEnvHomeFolder", dirname(__FILE__));
	if(!isset($glbHttpsFlag)||$glbHttpsFlag=="")
		wbxsession("glbHttpsFlag", "-1");
	if(!isset($basepath)||$basepath=="")
		wbxsession("basepath", "/prog");
	if(!isset($glbLogPath)||$glbLogPath=="")
		wbxsession("glbLogPath", "/walter/weblog");

	function GetHomePath() //Only use for include(" ")  //----modified by walter 2003.2.25
	{
			global $glbEnvHomeFolder;
			$HomePath=$glbEnvHomeFolder;
			if (is_dir($HomePath))
				return $HomePath;
			else
				return "Error to find DOCUMENT_ROOT";
	}

	
}
?>
