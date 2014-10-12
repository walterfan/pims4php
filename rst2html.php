<?php

ini_set('display_errors', 'on');
error_reporting(E_ERROR);
require "util/rst.php";

# Read file and pass content through the Markdown praser
$filename=$_GET['filename'];
$text = file_get_contents($filename);
$html = RST($text);

?>
<!DOCTYPE html>
<html>
    <head>
        <title>Restructure to Html</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    </head>
	<style type="text/css">
#journalDiv {
	border:1px solid #336699;
	background-color:#cceeee;
	padding: 0.5em
}
#journalSub{
	background-color: #DDDDEE;
	border-top:1px solid #336699;
	font-size: 24px;
	font-style: bold;
	text-align: center;
	line-height: 36px;
}
</style>

    <body>
	<div id="journalSub"><strong><?=basename($filename)?></strong></div>
	<div id="journalDiv">
		<?php
			# Put HTML content in the document
			echo $html;
		?>
	</div>

    </body>
</html>
