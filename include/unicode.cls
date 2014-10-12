<?
class Unicode
{
    var $filepath;
    var $charset;
   
    function  str2utf8($str,$LanguageID)  
    {
   	if(!isset($LanguageID)||"".$LanguageID=="")
        	$LanguageID=1;
	if(!isset($str)||"".$str=="")
		return;
	else
		$string=$str;
	$filepath=GetHomePath()."/site/";
    	if ($LanguageID==3)	//------gb2312
		$filename=$filepath."gb2312.txt";  
   	else if ($LanguageID==4)//------big5
		$filename=$filepath."big5.txt";  
  	else if ($LanguageID==5)//------shiftjis
		$filename=$filepath."shiftjis.txt";  
	else if ($LanguageID==6)//------euc-kr
		$filename=$filepath."euckr.txt";  
   	else	     //return utf8_encode($string);
	    return $string;
	$tmp=file($filename);  
        $codetable=array();  
        while(list($key,$value)=each($tmp))  
        {
	        if(substr($value,0,1)!="#")
	               	$codetable[hexdec(substr($value,0,6))]=substr($value,7,6);  
	        
        }  
          
        $utf8="";  
        while($str)  
        {  
                    if(ord(substr($str,0,1))>127)  
                    {  
                           $this=substr($str,0,2);  
                           $str=substr($str,2,strlen($str)); 
                           if ($LanguageID==3)
                           {
                           	if(isset($codetable[hexdec(bin2hex($this))-0x8080]))
                           		$utf8.=dechex(hexdec($codetable[hexdec(bin2hex($this))-0x8080]));  
                           	else
                           		$utf8.=0;
                           }
                           else
                           {
                           	if(isset($codetable[hexdec(bin2hex($this))]))
                           		$utf8.=dechex(hexdec($codetable[hexdec(bin2hex($this))]));
                           	else
                           		$utf8.=0;
                           	
                           }
                           	
                                
   
                   }  
                    else  
                    {  
                           $utf8.="00".dechex(ord(substr($str,0,1)));  
                           $str=substr($str,1,strlen($str));  
                    }  
	
        }  
        $unicode=hex2bin(&$utf8); 
        return UnicodeToUTF(&$unicode);
    }

    function hex2bin(&$String)
    {
    	$ReturnString="";
    	$len=strlen($String);
    	for($i=0;$i<$len;$i+=2)
    	{
    		$ReturnString.= pack("C",hexdec(substr($String,$i,2)));
    	}
    	return $ReturnString;
    }	

    function UnicodeToUTF(&$String)
    {
    	$len=strlen($String);
    	$ReturnString="";
    	for ($x=0;$x<$len;$x++) {
    	    $Char=substr($String,$x++,1);
    		if ($x==$len) {
    		    $ReturnString.=bin2hex($Char);
    			continue;
    		}
    		$Char.=substr($String,$x,1);
    		$hex=bin2hex($Char);
    		$dec=hexdec($hex);
    		$bin=decbin($dec);
    		$temp="";
    		if($dec>0x7f){
    			$binlen=strlen($bin);
    			for ($i=0;$i<16-$binlen;$i++) {
    			    $bin="0".$bin;
    			}
    			$temp.="1110".substr($bin,0,4);
    			$temp.="10".substr($bin,4,6);
    			$temp.="10".substr($bin,10,6);
    			$temp=dechex(bindec($temp));
    		}
    		else {
    			$temp=substr($hex,2,2);
    		}
    		$ReturnString.=$temp;
    	}
    	return hex2bin($ReturnString);
    }
    
    function  str2unicode($str,$charset)  
    {
        	if(!isset($charset)||"".$charset=="")
            	$charset="iso-8859-1";
            $filepath=GetHomePath()."/site/";
        	if (trim(strtolower($charset))=="gb2312")
    		$filename=$filepath."gb2312.txt"; 
        	else if(trim(strtolower($charset))=="big5")
    		$filename=$filepath."big5.txt"; 
        	else if(trim(strtolower($charset))=="euc-kr")
    		$filename=$filepath."euckr.txt"; 
        	else if(trim(strtolower($charset))=="shift-jis")
    		$filename=$filepath."shiftjis.txt"; 
    	else
    	     return $str;
        
    	$tmp=file($filename);  
    	$codetable=array();  
        	while(list($key,$value)=each($tmp))  
            	$codetable[hexdec(substr($value,0,6))]=substr($value,7,6);       
        	$utf8="";  
        	while($str)  
        	{  
           		if(ord(substr($str,0,1))>127)  
    		{  
                                       $this=substr($str,0,2);  
                                       $str=substr($str,2,strlen($str));  
                                       if(trim(strtolower($charset))=="gb2312")
                                       	$utf8.="&#x".dechex(hexdec($codetable[hexdec(bin2hex($this))-0x8080])).";";  
                                       else                                                                  
                                       	$utf8.="&#x".dechex(hexdec($codetable[hexdec(bin2hex($this))])).";";  
                           }  
                           else  
                           {  
                                       $utf8.="&#x".dechex(ord(substr($str,0,1))).";";  
                                       $str=substr($str,1,strlen($str));  
                           }  
               }  
                 
               return  $utf8;  
    }  
    
    
    function utf8ToUnicodeEntities ($source) {
    // array used to figure what number to decrement from character order value 
    // according to number of characters used to map unicode to ascii by utf-8
    $decrement[4] = 240;
    $decrement[3] = 224;
    $decrement[2] = 192;
    $decrement[1] = 0;
    
    // the number of bits to shift each charNum by
    $shift[1][0] = 0;
    $shift[2][0] = 6;
    $shift[2][1] = 0;
    $shift[3][0] = 12;
    $shift[3][1] = 6;
    $shift[3][2] = 0;
    $shift[4][0] = 18;
    $shift[4][1] = 12;
    $shift[4][2] = 6;
    $shift[4][3] = 0;
    
    $pos = 0;
    $len = strlen ($source);
    $encodedString = '';
    while ($pos < $len) {
    $asciiPos = ord (substr ($source, $pos, 1));
    if (($asciiPos >= 240) && ($asciiPos <= 255)) {
    // 4 chars representing one unicode character
    $thisLetter = substr ($source, $pos, 4);
    $pos += 4;
    }
    else if (($asciiPos >= 224) && ($asciiPos <= 239)) {
    // 3 chars representing one unicode character
    $thisLetter = substr ($source, $pos, 3);
    $pos += 3;
    }
    else if (($asciiPos >= 192) && ($asciiPos <= 223)) {
    // 2 chars representing one unicode character
    $thisLetter = substr ($source, $pos, 2);
    $pos += 2;
    }
    else {
    // 1 char (lower ascii)
    $thisLetter = substr ($source, $pos, 1);
    $pos += 1;
    }
    
    // process the string representing the letter to a unicode entity
    $thisLen = strlen ($thisLetter);
    $thisPos = 0;
    $decimalCode = 0;
    while ($thisPos < $thisLen) {
    $thisCharOrd = ord (substr ($thisLetter, $thisPos, 1));
    if ($thisPos == 0) {
    $charNum = intval ($thisCharOrd - $decrement[$thisLen]);
    $decimalCode += ($charNum << $shift[$thisLen][$thisPos]);
    }
    else {
    $charNum = intval ($thisCharOrd - 128);
    $decimalCode += ($charNum << $shift[$thisLen][$thisPos]);
    }
    
    $thisPos++;
    }
    
    if ($thisLen == 1)
    $encodedLetter = "&#". str_pad($decimalCode, 3, "0", STR_PAD_LEFT) . ';';
    else
    $encodedLetter = "&#". str_pad($decimalCode, 5, "0", STR_PAD_LEFT) . ';';
    
    $encodedString .= $encodedLetter;
    }
    
    return $encodedString;
    }
    
          
    }
?>