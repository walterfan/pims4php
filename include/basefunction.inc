<?


//*****************************************************************************************
//				Module:			pmsite
//				Author:			Walter
//				Date:			9/24/2002
//				Last modified		4/4/2003
//				Functions:								
//
//
//*****************************************************************************************
//
if(!defined("__FUNCTION_INC__"))
{
		define("__FUNCTION_INC__",TRUE);
	//=====================================Collect from Diablo
	

//Everybody can't change the Encoder text.  -Daniel
	$encoderText="lsdfoglkwjemc-091324jlkmsda-0sd=1234;l;lsdkOPIER203-4LKJSLDJAS0D925JKNNC,MANSLDJQ32ELK1N4SAIp089er0234lkjo9df82l3kjlknf,nzxc,mn;lasdj9wquelq;d]qowe[;wq;qkwellsdkfj0-0POPOAR0W8RPOp-02@#"."$"."sdklj"."$"."#)0asdlksadLKJFA9820934)(&"."$"."3ij09sdj34-sdfj2po345-09dlkfjlkv,mxncv;laskdkl/a;au093hakjh2389~!@%&*%#&^539478(*&)^(&^_*8-*_+++|78w3ihsdnmnclksdj)(*#%*_@"."$"."(+#@"."$".")&@#^*&^#@"."$"."()(*#@"."$"."HDFIkdhfgkjh098k;ldsk.sdv.c,msd;flkp0w34;2lk-=sd0p121o39-werl2k3;4lj09sdflskjlekfj,mv,mcxvjlksjdflksjdl*(#@!&akhduyqweperilmmdxcasnd*(#@9879327kjhasudfewr kwehriwueyrhc ausdgiq7w8e71 cdsh93ol2q32879y8932qwhdkjanhdskjaoe*&w#jh"."$".")(*dsFshc na89wue32e981yewher12(*&#quds)(*i3o1928osaihdaklsdkalkduqowe3290874kljhklasdhlijhqweio4hwe89(*"."$"."#"."$"."eriho349oij(#*q"."$"."OIJHO)(&*#"."$"."_)(IUDSOIUoiOIUSAODFU034liusdrogiuet0lsdmc,.mg;lq-091lk3l;kjsdf--123098fe*(JOKJSFD983345oihjdp0(#*"."$"."&#@!HKJH!(@#*&ioysdk@#)uhOA7E98R7239845K(*&(#@*"."$"."&HKFDJHWERYIWoi)(*&#@&^%@!dsfoi;.;,p[osklejr230897*(&we2&^%@78*(&#@!(7~&*~^@*&^#(*&auroiqkjwrhoasdf89qlrlkjpour09werk23jh";


	// @func int | QueryAccessLevel | Check if current user has access to office/business unit
	// @rdesc <nl>
	// 0 : visitor
	// 1 : group access (readonly)
	// 2 : owner (full access)
	// @parm 0=user 1=office 2=business unit
	// @parm UserID/OfficeID/BizUnitID
	function QueryAccessLevel($ownerType, $ownerID)
	{
		$userID = GetUserInfo("UserID");

		if ($ownerID==$userID)
			return 2;
		else
			return 0;
	}
	
	function Random1($x)
	{
		return floor(random())%$x;
	}
	function random()
	{
		srand((double)microtime()*1000000);
		$strNum = rand();
		$strNum = str_replace(".","",$strNum);
		return $strNum;
	}

	function EncodeChar($c)   //Writed by Daniel
	{
	    $s = "";
	    $x = floor($c/16);
	    if ($x>9)
	        $s .= chr($x-10+0x61);
	    else
	        $s .= $x;

	    $x = $c%16;
	    if ($x>9)
			$s .= chr($x-10+0x61);
	    else
			$s .= $x;

		return $s;
	}
	
	function DecodeChar($str,$i)   //Writed by Daniel
	{
	    return intval(substr($str,$i,2),16);
	}
	
	function Encrypt($str)   //Writed by Daniel
	{
		global $encoderText;
		$seed = Random1(255);
		$pre = $seed & 3;
		$len = strlen($str);
		$elen = strlen($encoderText);
		$ret = "";
		$ret .= EncodeChar($seed);
		$ret .= EncodeChar((Random1(255)&0xfc)+$pre);
		for ($i=0;$i<$pre;$i++)
			$ret .= EncodeChar(Random1(255));

		for ($i=0,$j=$seed;$i<$len;$i++)
		{
			$ret .= EncodeChar( ord(substr($str,$i,1)) ^ ord(substr($encoderText,$j,1)) );
			if (++$j>=$elen)
				$j=0;
		}
		return $ret;
	}
	
	function Decrypt($str)  //Writed by Daniel
	{
		global $encoderText;
		$seed = DecodeChar($str,0);
		if (gettype($seed)!="integer")
			return "";
		$pre = DecodeChar($str,2);
		if (gettype($pre)!="integer")
			return "";
		$pre = $pre&3;
		$ret = "";
		$len = strlen($str);
		$elen = strlen($encoderText);
		for ($i=$pre+$pre+4,$j=$seed;$i<$len;$i+=2)
		{
			$x = DecodeChar($str,$i);
			if (gettype($x)!="integer")
				return "";
			$x ^= ord(substr($encoderText,$j,1));
			$ret .= chr($x);
			if (++$j>=$elen)
				$j=0;
		}
		return $ret;
	}

	function SetBit($intData,$bit,$value)
	{
		$value = ($value == 0) ? 0 : 1; /* limit to 0,1 */
		return ($value == 1) ? $intData | ($value << $bit) : ~(1 << $bit) & $intData;
	}
	
	function GetBit($intData,$bit)
	{
		return ($intData >> $bit) & 1;
	}


	function JSReplace($url, $errmsg="")
	{
		$c = strstr($url,"?")? "&" : "?";
		$str = "\"".$url.$c."Rnd\"+(Math.random()+\"\").substring(2,6)+\"=\"+Math.random()";
		if ( "".$errmsg!="" )
			print("<script language=\"javascript\">\nalert(\"".$errmsg."\");\n</script>\n");

		$debugstatus=$_SESSION["debugstatus"];
		if($debugstatus == "on")
		{
			print("JSReplace: <a href='javascript:location.replace(\"$url\")'>$url</a>\n");
		}
		else
		{
			print("<script language=\"javascript\">\nlocation.replace(".$str.");\n</script>");
		}

		flush();		
		exit();
	}		
	function goback($step=-1)
	{
		print("<script language=\"javascript\">\nhistory.go(".$step.");\n</script>");
		flush();		
		exit();
	}
	function MsgBox($k)
	{
		if("".$k!="")
		{
		$k=str_replace("\\","\\\\",$k);
		$k=str_replace("'","\'",$k);
		$k=str_replace("\n","\\n",$k);
		?>
			<script language='javascript'> alert('<?=$k?>') </script>
		<? 	return ;
		}
		return;
	}

	function GetCurCharSet()
	{	
		$CharSet = "iso-8859-1";
		
		$SysLanguage = getenv("HTTP_ACCEPT_LANGUAGE");
		if (strstr($SysLanguage , "zh-cn"))	$CharSet = "gb2312";
		if (strstr($SysLanguage , "zh-tw"))	$CharSet = "big5";
		if (strstr($SysLanguage , "ja"))	$CharSet = "shift_jis";
		if (strstr($SysLanguage , "ko"))	$CharSet = "euc-kr";
		return $CharSet;	
	}

        function CopyDir($source,$dest)
		{
			if(is_dir($source))
			{
				$dirName= basename($source);
				$dest.="/".$dirName;
				mkdir($dest,0777);
				$arrFiles=GetDirList($source);
				for($i=0;$i< sizeof($arrFiles);$i++)
					wbxCopyDir($source."/".$arrFiles[$i],$dest);
			}
			else
			{
				copy($source,$dest."/".basename($source));
				//echo "$source is copied to ".$dest."/".basename($source)." OK!<BR>";
			}
		
		}
		
/////////////////////////////////////////////////////// For personal site  /////////////////////////////////////////////////////// 
	
	function getUserid()
	{
		$sql="select user_id from users where user_name='".$_SESSION["ss_username"]."'";
		$atas=new mysql;
		$arr=$atas->getmatrix($sql);
		return $arr[0]["user_id"];
	
	}

	    function GetDirList($dir_name)
		{
			$tmp_arr = array();
			if (is_dir($dir_name) == false) return $tmp_arr;
			$tmp_dir= opendir($dir_name);
			$tmp_arr=array();
			while(1)
			{
					$tmp_filename= readdir($tmp_dir);
					if(!$tmp_filename) break;
					if($tmp_filename!="." && $tmp_filename!="..")
						$tmp_arr[count($tmp_arr)]=$tmp_filename;
			}
			closedir($tmp_dir);
			sort($tmp_arr);
			return($tmp_arr);
		}
function GetCookie($strName)
{
	global $$strName;
	if("".$$strName=="deleted")$$strName="";
	return "".$$strName;
}

function KillCookie($strName)
{
	setcookie($strName);
}


////////////////////////////////////////////////////////
		function RemoveDir($dir_name)
		{
			if(is_dir($dir_name))
			{
				$dirfile=wbxGetDirList($dir_name);
				for ($i=0;$i<count($dirfile);$i++)
				{
					if (is_dir($dir_name . "/" . $dirfile[$i]))
						wbxRemoveDir($dir_name . "/" . $dirfile[$i]);
					else
						unlink($dir_name . "/" . $dirfile[$i]);
				}
			
				$dirfile=wbxGetDirList($dir_name);
				if (count($dirfile)==0) 
					$rt= rmdir($dir_name);
				
				return($rt);
			}
		}
		
function getFileList ($path,$depth=1) 
{
    $retval=array();
	if ($dir = opendir($path))
	{
		 while (false !== ($file = readdir($dir)))
		 {
		
		   if ($file=="."||$file=="..") 
			   continue;
		   else if (is_dir($path."/".$file))
			{
			   if($depth>0)
					$retval = array_merge($retval,getFileList($path."/".$file,$depth-1));
		   }
		   else if (is_file($path."/".$file))
				array_push($retval,$path."/".$file);
		 }
		 closedir($dir);
   }
   return $retval;
}

function getFileExt($filename)
{
	$filename=basename($filename);
	$file=explode(".",$filename);
	return $file[count($file)-1];
}

function GetFile2HtmlString($filename)
{
	if(filesize($filename)>0&&(strstr($filename,".txt")||strstr($filename,".htm")))
	{
		$handle = fopen ($filename, "r");
	
		$contents = fread($handle, filesize($filename));

		fclose ($handle);
		return htmlspecialchars(trim($contents));

	}
	else
	{
		return $filename;
	}
}

function deletetree($file) 
{ 
	chmod($file,0777); 
	if (is_dir($file)) 
	{ 
		 $handle = opendir($file); 
		 while($filename = readdir($handle)) 
		{ 
			if ($filename != "." && $filename != "..") 
			{ 
			deletetree($file."/".$filename); 
			} 
		} 
		closedir($handle); 
		rmdir($file); 
	} 
	else
	{ 
		unlink($file); 
	} 
} 

	function writelog($message,$note="",$sep="\n",$logfile="messages.log")
	{
		
		$message=$note.$message.$sep;
		$fp=fopen($logfile,"a+");
		fwrite($fp,$message);
		fclose($fp);
		
	}

	function getlog($logfile="mylog.txt")
	{
		$fp=fopen($logfile,"r");
		$contents = fread ($fp, filesize ($logfile));	
		fclose($fp);
		return $contents;
	}

   	function get_timer()
      	{
	  $t=gettimeofday();
	  return ($t['sec'].'.'.$t['usec']);
	}

	function printduration($begin_time,$end_time)
	{
		$arr_begin=split("\.",$begin_time);
		$arr_end=split("\.",$end_time);
		echo "<br><b>Duration</b> ".$arr_end[0]." - ".$arr_begin[0];
		echo ", ".$arr_end[1]." - ".$arr_begin[1];
		$Duration_time=(intval($arr_end[0])*1000000+intval($arr_end[1]))-(intval($arr_begin[0])*1000000+intval($arr_begin[1]));
		echo " = ".intval($Duration_time/1000000)."s ".($Duration_time%1000000)."ms";
	}

	function setLocalefromLanguageID($LanguageID,$domain="messages",$location="")
	{
		if($LanguageID=="")
		{
			global $glbCurSiteID;
			$LanguageID = ${"CK_LanguageID"."_".$glbCurSiteID};
			if(intval($LanguageID)==0) $LanguageID = GetLanguageIDFromUrl(BrandPathName());
		}
		switch (intval($LanguageID))
		{
			case 1:
				$language = 'en';
				break;
			case 3:
				$language = 'zh_CN';
				break;
			case 4:
				$language = 'zh_TW';
				break;
		
			case 5:		
				$language = 'ja_JP';
				break;
		
			
			case 6:
				$language = 'ko_KR';
				break;
		
			case 7:
				$language = 'fr_FR';
				break;
		
			case 9:
				$language = 'de_DE';
				break;		
			case 12:
				$language = 'es_MX';
				break;
			case 13:
				$language = 'zh_CN';
				break;
			default:
				$language = 'sv_SE';
				break;
		
		}
		putenv("LANG=".$language);
		putenv("LANGUAGE=".$language); 
		$newLocale=setlocale(LC_ALL, $language);
		$debugstatus=$_SESSION["debugstatus"];
		if($debugstatus=="on")
			msgbox( "Current Locale: ".$newLocale." Text domain:".$domain);
		if($location=="")
			$location=GetHomePath();
		bindtextdomain($domain, $location."/locale"); 
		textdomain($domain);	
		
	}

function ListPage($pagenumber)
{		
/*
	$arr_page=array();
	$result=array();
	if(!isset($pagesize)) $pagesize = 10; 
	if(!isset($page)) 	$page = 1; 
	if($page<1)			$page = 1; 

	$recordcount=sizeof($arrCust);
	$pagecount = ceil($recordcount/$pagesize); 
	if($page>$pagecount) $page = $pagecount; 
	if(sizeof($arrCust)>0)
	{	
		$arr_page=array_chunk($arrCust, $pagesize);
		$result=$arr_page[$page-1];
	}

	
*/		
	global $pagecount,$page;
	$pagenumber=5;
	if($pagecount>1)
	{
		echo "Page:&nbsp;";
		$pagestart=$page-$page%$pagenumber+1;
		if(($pagestart-1)>0)
			echo "<a href='".$PHP_SELF."?page=".($page-$pagenumber)."'>Previous</a>&nbsp;";
		for($i=0;$i<$pagenumber;$i++)
		{
			$page_i=$pagestart+$i;
			if($page_i<1)	       $page_i = 1; 
			if($page_i>$pagecount) break;
			echo "<a href='".$PHP_SELF."?page=".$page_i."'>".$page_i."</a>&nbsp;";
		}
		if(($pagestart+$pagenumber-1)<$pagecount)
			echo "<a href='".$PHP_SELF."?page=".($page+$pagenumber)."'>Next</a>&nbsp;";

	}
}

function local_substr($str,$start=0,$len=24) 
{ 
    $tmpstr="";
    //$LanguageID = GetCurLanguageID();
    $LanguageID = 3;//Simple Chinese
    for($i=0;$i<strlen($str);$i++) 
    { 
    	if ($i>=$start+$len) 
    		break; 
 	if($i<$start)
 	        continue;
    	if(ord(substr($str,$i,1))>0x7F&&($LanguageID == 3 || $LanguageID == 4 || $LanguageID == 5 || $LanguageID == 6))
    	{ 
 		$tmpstr.=substr($str,$i,2); 
    		$i++;
    	} 
    	else 
    	{ 
     		$tmpstr.=substr($str,$i,1); 
    	} 
    }
	if(($len-$start)>=strlen($str)) 
		return ucfirst($str);
	else
		return ucfirst($tmpstr)."...";
}

	/*function EscapeDBString($str)
	{
		$str = EscapeSlashes($str);
		$str = str_replace("'", "''", $str);
		
		return $str;
	}*/
	function loadflash($flashname,$src,$width=966,$height=550,$bgcolor="#FFFFFF")
	{
	?>
	<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=7,0,0,0" id="<?=$flashname?>" width="<?=$width?>" height="<?=$height?>" align="middle">
		<param name="allowScriptAccess" value="sameDomain" />
		<param name="movie" value="<?=$src?>">
		<param name="quality" value="high">
		<PARAM name="bgcolor" value="<?=$bgcolor?>">
		<param name="wmode" value="<?=$bgcolor?>">
		<PARAM name="salign" VALUE="RL">
		
		
		<embed src="<?=$src?>"  quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer"
		quality="high" bgcolor="<?=$bgcolor?>" align="middle" 
		allowScriptAccess="sameDomain" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" width="<?=$width?>" height="<?=$height?>"></embed>
	</object>
	<?
	}
	
	function getfiletsuffix($filename)
	{
		$restr="";
		if(strstr($filename, "."))
		{
			$arrfile=split("\.",$filename);
			$restr.=$arrfile[1];	
		}
		return $restr;
	
	}

function BrandPathName()  //use for /site/....  and /brand/$SiteID/site_....     -Daniel
{
		$url=strtolower(getenv("SCRIPT_NAME"));
		$arr=explode("/",$url);
		return "$arr[1]";
}

/**
 * method: getdatastr
 * get data from an two dimension array
 * such as USERNAME=walter||adam||ayla&PASSWORD=pass||pass1||pa
 * @param:$arr
 * @return:$datastr
*/
//echo "sina";
function getdatastr($arr)
{
    $datastr="HEAD=0&";
    //$arr[6]['CITYNUM'] *= 10;
    if($arrlen=sizeof($arr)>0)
    {       	
        reset($arr[0]);
        while(list($key,$value) = each($arr[0]))
            $$key=""; 
        reset($arr[0]);
        while(list($key,$value) = each($arr))
        {
            while(list($key1,$value1) = each($value))
            {
				for($i = 0;$i<strlen($value1);$i++)
				{
					if(substr($value1,$i,1) == "&")
					$value1 = substr_replace($value1,"$",$i,1);
				}
            $$key1.=$value1."||";
       		}
        }
        reset($arr[0]);
        while(list($key2,$value2) = each($arr[0]))
        {
            $datastr.=$key2."=".trimlastchars($$key2,"||")."&";   
        }     
    }
    //return substr($datastr,0,strlen(rtrim($datastr))-1);
    return trimlastchars($datastr);
}

function trimlastchars($str,$trailstr="")
{
    if($trailstr=="")
        return substr($str,0,strlen(rtrim($str))-1);    
    else
        return substr($str,0,strlen(rtrim($str))-strlen(rtrim($trailstr)));    
}
/**
 * method: getCommaStr
 * get data string divided by comma from an array
 * such as "abc,cde,fgh"
 * @param:$arr
 * @return:$datastr
*/
function GetCommaStr($arr)
{
    $datastr="";
    if(is_array($arr)&&sizeof($arr)>0)
    {       	
			while(list($key,$value) = each($arr))
			{
		        if(is_array($value)&&sizeof($arr)>0)
				{
					while(list($key1,$value1) = each($value))
					{
						$datastr.=$value1.",";
					}
				}
				else
				{
						$datastr.=$value.",";
				}
			}
			return trimlastchars($datastr);
    }
	else
			return ""; 
}
function print_r_html($myvar)
{
       echo '<pre>'.str_replace(array("\n" , " "), array('<br>', '&nbsp;'), print_r($myvar, true)).'</pre>';
}

function orderArrayFromKey($strOrderBy,$strOrder,$orderarray)
{
	for($i = 0;$i < count($orderarray);$i++)
	{
		for($j = 0;$j < count($orderarray)-$i-1;$j++)
		{
			//judge if order by desc or Order by asc
			if($strOrder == "desc")
			{
				if($orderarray[$j]->$strOrderBy < $orderarray[$j+1]->$strOrderBy)
				{
					//swap the position of the two Obj.
					$arrTemp = $orderarray[$j];
					$orderarray[$j] = $orderarray[$j+1];
					$orderarray[$j+1] = $arrTemp;
				}
			}
			else
			{
				if($orderarray[$j]->$strOrderBy > $orderarray[$j+1]->$strOrderBy)
				{
					//swap the position of the two Obj.
					$arrTemp = $orderarray[$j];
					$orderarray[$j] = $orderarray[$j+1];
					$orderarray[$j+1] = $arrTemp;
				}
			}
		}
	}
	return $orderarray;
}

function issetvar($var)
{
	if(!isset($var)||$var=="")
		return false;
	else
		return true;

}

function isdebug()
{
	$debugstatus=$_SESSION["debugstatus"];
	if($debugstatus=="on")
		return true;
	else
		return false;
}

function code2html($codes,$line_number_input='off')
{
	$html_out=stripslashes(trim($codes));
	$html_out=str_replace('<','&lt;',$html_out);
	$html_out=str_replace('>','&gt;',$html_out);
	$html_out=str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",$html_out);
	$html_out=str_replace(' ','&nbsp;',$html_out);

	 if($line_number_input=='on')
	 {
		  $line=strtok($html_out,"\n");
		  $html_out='<ol>';
		  $line_num=0;
		  while($line)
		  {
		       $html_out.="<li> $line\n";
		       $line=strtok("\n");
		  }
		  $html_out.='</ol>';
          }
     $html_out=str_replace("\n",'<br>',$html_out);
     return $html_out;

}
function br2nl($str) { 
	$str = preg_replace("/(\r\n|\n|\r)/", "", $str); 
	return preg_replace("=<br */?>=i", "\n", $str); 
}

function sortRecordset($recordset,$sortedfield,$sorttype)
{//////////////////////////////////////////////////////
///Note : the argument 1 should be reference !
///       Used as sortRecordset(&$recordset,$sortedfield,$sorttype)
///       $sorttype : 'desc' / 'asc'

    //$resultset = array();
/*
    echo "sortedfield=$sortedfield<br>";
    echo "sorttype=$sorttype<br>";
//*/
    $nCount = count($recordset);

    for($i=0;$i<$nCount;$i++){
        for($j=$i;$j<$nCount;$j++){

            $len1 = strlen($recordset[$i]->$sortedfield);
            $len2 = strlen($recordset[$j]->$sortedfield);
            $len = $len1 > $len2 ? $len1 : $len2;

            if(strncasecmp($sorttype,'asc',3) == 0){
                if(strncmp($recordset[$i]->$sortedfield,$recordset[$j]->$sortedfield,$len) > 0){
                    $tmp =  $recordset[$i];
                    $recordset[$i] = $recordset[$j];
                    $recordset[$j] = $tmp;
                }
            }else if(strncasecmp($sorttype,'desc',4) == 0){
                if(strncmp($recordset[$i]->$sortedfield,$recordset[$j]->$sortedfield,$len) < 0){
                    $tmp =  $recordset[$i];
                    $recordset[$i] = $recordset[$j];
                    $recordset[$j] = $tmp;
                }
            }
        }
    }
    return $recordset;
}

	function ifneedlogin($username,$userpass,$savepwd="off")
	{
		if($_SESSION["ss_username"]!="")
			return false;
		if(!isset($username)||!isset($userpass)||$username==""||$userpass=="")
		{							
			return true;		
		}
		else
		{
			$conn = getConnection();
			$query="select * from users where user_name='".$username."'";
			$rs=$conn->GetAll($query);
			if($_SESSION["debugstatus"]=="on")
				echo "<hr>".__LINE__.": ".$rs[0]["user_pass"]."---".Decrypt($userpass)."--".$userpass;
			 if($rs[0]["user_pass"]==Decrypt($userpass))
			 {					  
			        
			       if($savepwd=="on")
				{
					setcookie("ck_username",$username, time()+3600*24*30,"/");
					setcookie("ck_userpass",Encrypt($userpass), time()+3600*24*30,"/");
				}
				$ss_username=$username;
				session_register("ss_username");
				$_SESSION["ss_username"]=$username;
				return false;
			}
		}
		return true;
	}
	function html2txt($document)
	{
	    $search = array('@<script[^>]*?>.*?</script>@si',  // Strip out javascript
			   '@<style[^>]*?>.*?</style>@siU',    // Strip style tags properly
			   '@<[\/\!]*?[^<>]*?>@si',            // Strip out HTML tags
			   '@<![\s\S]*?--[ \t\n\r]*>@'        // Strip multi-line comments including CDATA
	    );
	    $text = preg_replace($search, '', $document);
	    return $text;
	}


}//====================end this file


?>