<?
if (!defined("__DBCONN_INC_PHP__")) 
{
	define(__DBCONN_INC_PHP__,TRUE);
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

/*-----------------for adodb-----------------------------------*/
include_once (GetHomePath()."/util/adodb_lite/adodb.inc.php");
/*
$glb_dbtype="mssql";
$glb_dbserver="WALTER-COMPUTER";
$glb_dbuser="sa";
$glb_dbpass="pass";
$glb_dbname="xjdb";
*/

$glb_dbtype="mysql";
$glb_dbserver="localhost";
$glb_dbuser="root";
$glb_dbpass="aibb";
$glb_dbname="mydb";

if($glb_dbtype=="mysql")
{
	$datesep="-";	
}
else if($glb_dbtype=="mssql")
{
	$datesep="/";	
}

	function getConnection($dbtype="",
		$dbserver="",$dbuser="",$dbpass="",$dbname="")
	{
		  global $glb_dbtype,$glb_dbserver, $glb_dbuser,$glb_dbpass,$glb_dbname;
		  if($dbtype=="")
				$dbtype=$glb_dbtype;
		  if($dbserver=="")
				$dbserver=$glb_dbserver;
		  if($dbuser=="")
				$dbuser=$glb_dbuser;
		  if($dbpass=="")
				$dbpass=$glb_dbpass;
		  if($dbname=="")
				$dbname=$glb_dbname;
		  if(!isset($db)||!$db) 
		  {
				$db = NewADOConnection($dbtype);
				if(isdebug())
					$db->debug=true;
				$db->PConnect($dbserver,$dbuser, $dbpass,$dbname);
		 }      
		 return $db;
	}

function freeConnection($db)
{
	 $db->close();
}
}
?>