<?
//*****************************************************************************************
//				Module:			WIN Weather Map
//				Author:			Walter
//				Date:			3/09/2004
//				Last modified	3/09/2004
//
//*****************************************************************************************

if (!defined("__LOGON_INC__")) 
{
	define(__LOGON_INC__,TRUE);
	header("Cache-Control: private"); // HTTP/1.1
	header("Pragma: Pragma");          // HTTP/1.0

	include_once(dirname(__FILE__)."/basefunction.inc");
	include_once(dirname(__FILE__)."/initdrv.php");

	if(!isset($_SESSION['ss_username']))
	{
		
		if($_COOKIE["ck_username"]!=""&&$_COOKIE["ck_userpass"]!="")
		{
			$needlogin=ifneedlogin($_COOKIE["ck_username"],$_COOKIE["ck_userpass"]);
		}
		else
		{
			$ReturnPage=getenv("REQUEST_URI");
			$_SESSION["ReturnPage"] = $ReturnPage;
			JSReplace($glbEnvSITE_PATH."/site/loginform.php");
		}
	}
	
	if(intval($glbHttpsFlag)==1&&!strstr(strtoupper($_SERVER["HTTP_REFERER"]),"https"))
	{
		?>
		<script language="Javascript">
		var strDocURL = document.URL;
		if (strDocURL.indexOf("http://") >= 0)
		{
			strDocURL = "https"+strDocURL.substring(4,strDocURL.length);
			location.href = strDocURL;
		}
		</script>
		<?	
	}
	else if(intval($glbHttpsFlag)==0&&strstr(strtoupper($_SERVER["HTTP_REFERER"]),"https"))
	{
		?>
		<script language="Javascript">
		var strDocURL = document.URL;
		if (strDocURL.indexOf("https://") >= 0)
		{
			strDocURL = "http"+strDocURL.substring(5,strDocURL.length);
			location.href = strDocURL;
		}
		</script>
		<?			
		
	}

	

	
}//====================end	

?>