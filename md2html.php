<?php

# This file passes the content of the Readme.md file in the same directory
# through the Markdown filter. You can adapt this sample code in any way
# you like.

# Install PSR-0-compatible class autoloader
spl_autoload_register(function($class){
	require preg_replace('{\\\\|_(?!.*\\\\)}', DIRECTORY_SEPARATOR, ltrim($class, '\\')).'.php';
});

# Get Markdown class
use \Michelf\Markdown;

# Read file and pass content through the Markdown praser
$filename=$_GET['filename'];
$text = file_get_contents($filename);
$html = Markdown::defaultTransform($text);

?>
<!DOCTYPE html>
<html>
    <head>
        <title>Markdown to Html</title>
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
