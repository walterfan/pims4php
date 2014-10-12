<?php
//otherfunc.php
function htmlencode($str){
	$str = (string)$str;

	$ret = '';
	$len = strlen($str);
	$nl = false;
	for($i=0;$i<$len;$i++){
	$chr = $str[$i];
	switch($chr){
	case '<':
	$ret .= '<';
	$nl = false;
	break;
	case '>':
	$ret .= '>';
	$nl = false;
	break;
	case '"':
	$ret .= '"';
	$nl = false;
	break;
	case '&':
	$ret .= '&';
	$nl = false;
	break;
	/*
	case ' ':
	$ret .= ' ';
	$nl = false;
	break;
	*/
	case chr(9):
	$ret .= '    ';
	$nl = false;
	break;
	case chr(10):
	if($nl) $nl = false;
	else{
	$ret .= '<br>';
	$nl = true;
	}
	break;
	case chr(13):
	if($nl) $nl = false;
	else{
	$ret .= '<br>';
	$nl = true;
	}
	break;
	default:
	$ret .= $chr;
	$nl = false;
	break;
	}
	}

	return $ret;
}


function htmlencode4textarea($str){
	$str = (string)$str;

	$ret = '';
	$len = strlen($str);
	for($i=0;$i<$len;$i++){
		$chr = $str[$i];
		switch($chr){
		case '<':
		$ret .= '<';
		break;
		case '>':
		$ret .= '>';
		break;
		case '"':
		$ret .= '"';
		break;
		case '&':
		$ret .= '&';
		break;
		case ' ':
		$ret .= ' ';
		break;
		case chr(9):
		$ret .= '    ';
		break;
		default:
		$ret .= $chr;
		break;
		}
	}

	return $ret;
}

function emailcheck($email){
	$ret=false;
	if(strstr($email, '@') && strstr($email, '.')){
		if(eregi("^([_a-z0-9]+([\\._a-z0-9-]+)*)@([a-z0-9]{2,}(\\.[a-z0-9-]{2,})*\\.[a-z]{2,3})$", $email)){
			$ret=true;
		}
	}
	return $ret;
}

function str2url($path){
	return eregi_replace("%2f","/",urlencode($path));
}
?> 