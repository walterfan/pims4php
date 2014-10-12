<?
if (!defined("__INITDRV_PHP__")) 
{
	define(__INITDRV_PHP__,TRUE);
// Emulate register_globals on
	if (!ini_get('register_globals')) {
	   $superglobals = array($_SERVER, $_ENV,
		   $_FILES, $_COOKIE, $_POST, $_GET);
	   if (isset($_SESSION)) {
		   array_unshift($superglobals, $_SESSION);
	   }
	   foreach ($superglobals as $superglobal) {
		   extract($superglobal, EXTR_SKIP);
	   }
	}
	if(!isset($PHP_SELF))
		$PHP_SELF=$_SERVER["PHP_SELF"];

	session_start();

	/*-----------------for adodb-----------------------------------*/
	include_once (dirname(__FILE__)."/dbconn.inc.php");
	/*-----------------for phrame-----------------------------------*/
	//require_once(GetHomePath().'/phrame/include.php');

	/*-----------------for Smarty-----------------------------------*/

	include_once (GetHomePath().'/tpl/libs/Smarty.class.php'); 

	$smarty = new Smarty; 

	//指定目录结构
	$smarty->template_dir= GetHomePath()."/tpl/templates/";
	$smarty->compile_dir= GetHomePath()."/tpl/templates_c/";
	$smarty->config_dir= GetHomePath()."/tpl/configs/";
	$smarty->cache_dir= GetHomePath()."/tpl/cache/";

	//指定分隔符
	$smarty->left_delimiter = "<{";
	$smarty->right_delimiter = "}>";
	if(isdebug())
		$smarty->debugging=true;// only for IE window
	$smarty->caching = false; 
	//$smarty->caching = true 
	//$smarty->cache_lifetime = 30;


}
?>