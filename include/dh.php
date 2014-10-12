<?
if (!defined("__DH_PHP__")) 
{
	define(__DH_PHP__,TRUE);

include_once (dirname(__FILE__)."/basefunction.inc");
include_once (dirname(__FILE__)."/dbconn.inc.php");

function pagecode($beispiel)
	{
	   //  print("<p><br><br><h2>".$TEXT['srccode-header']."</h2>");
	    print("<p><br><br><h3><U>SOURCE CODE</h3>");
	    print("<textarea name=\"beispiel\" cols=\"80\" rows=\"18\" wrap=\"PHYSICAL\">");
		
	    if (file_exists($beispiel))
			{	
			$fp = fopen($beispiel, "r");
			while (!feof($fp))
				{
				$get= fgets($fp,4096);
				$get=ereg_replace("\\\\\"","\"",$get);
				 $get=ereg_replace("\\\\'","'",$get);
				 $get = ereg_replace( "\"","\"",$get); 
				//  $get = ereg_replace( "[\]","",$get);
				//  $get = ereg_replace( "\\\"","",$get); 
				print($get);
				// echo "$get";
				}
			fclose($fp);
			}
		print("</textarea><p>");
	} 

function viewsource($getarr)
{
	if (isset($getarr["source"])&&$getarr["source"]=="in")
	{ 
		$beispiel = $_SERVER["SCRIPT_FILENAME"]; 
		pagecode($beispiel);
	} 
	else
	{ 
		print("<p><br><br><h4><U><a href=\"".$_SERVER["PHP_SELF"]."?source=in\">View Source Codes</a></U></h4>");
	} 
}

function posttohost($url, $data) 
{
    $url = parse_url($url);
    if (!$url) return "couldn't parse url";
    if (!isset($url['port'])) { $url['port'] = ""; }
    if (!isset($url['query'])) { $url['query'] = ""; }

    $encoded = "";

    while (list($k,$v) = each($data)) {
    $encoded .= ($encoded ? "&" : "");
    $encoded .= rawurlencode($k)."=".rawurlencode($v);
    }

    $fp = fsockopen($url['host'], $url['port'] ? $url['port'] : 80);
    if (!$fp) return "Failed to open socket to $url[host]";

    fputs($fp, sprintf("POST %s%s%s HTTP/1.0\n", $url['path'], $url['query'] ? "?" : "", $url['query']));
    fputs($fp, "Host: $url[host]\n");
    fputs($fp, "Content-type: application/x-www-form-urlencoded\n");
    fputs($fp, "Content-length: " . strlen($encoded) . "\n");
    fputs($fp, "Connection: close\n\n");

    fputs($fp, "$encoded\n");

    $line = fgets($fp,1024);
    if (!eregi("^HTTP/1\.. 200", $line)) return;

      $results = ""; $inheader = 1;
      while(!feof($fp)) 
      {
            $line = fgets($fp,1024);
            if ($inheader && ($line == "\n" || $line == "\r\n")) 
            {
                $inheader = 0;
            }
            elseif (!$inheader) 
            {
                $results .= $line;
            }
        }
        fclose($fp);
        return $results;
    }	
}
?>