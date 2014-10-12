<?
include(dirname(__FILE__)."/urlcheck.php");
include(dirname(__FILE__)."/otherfunc.php"); //这两个文件的内容，附在最后。
  
//ubbcode类
class ubbcode{
	var $call_time=0;
	//可处理标签及处理函数对应表
	var $tags = array( //小写的标签 => 对应的处理函数
	'url' => '$this->url',
	'email' => '$this->email',
	'img' => '$this->img',
	'b' => '$this->simple',
	'i' => '$this->simple',
	'u' => '$this->simple',
	'tt' => '$this->simple',
	's' => '$this->simple',
	'strike' => '$this->simple',
	'h1' => '$this->simple',
	'h2' => '$this->simple',
	'h3' => '$this->simple',
	'h4' => '$this->simple',
	'h5' => '$this->simple',
	'h6' => '$this->simple',
	'sup' => '$this->simple',
	'sub' => '$this->simple',
	'em' => '$this->simple',
	'strong' => '$this->simple',
	'code' => '$this->simple',
	'samp' => '$this->simple',
	'kbd' => '$this->simple',
	'var' => '$this->simple',
	'dfn' => '$this->simple',
	'cite' => '$this->simple',
	'small' => '$this->simple',
	'big' => '$this->simple',
	'blink' => '$this->simple'
	);
	//url裢接属性
	var $attr_url;
	//url合法性检查对象
	var $urlcheck;

	function ubbcode($attr_url){
	$this->attr_url = ''.$attr_url;
	$this->urlcheck = new urlcheck();
	}

	//对$str进行UBB编码解析
	function parse($str){
	$this->call_time++;
	$parse = ''.htmlencode($str);

	$ret = '';
	while(true){
	$eregi_ret=eregi("\[[#]{0,1}[[:alnum:]]{1,7}\]",$parse,$eregi_arr); //查找[xx]
	if(!$eregi_ret){
	$ret .= $parse;
	break; //如果没有，返回
	}
	$pos = @strpos($parse,$eregi_arr[0]);
	$tag_len=strlen($eregi_arr[0])-2;//标记长度
	$tag_start=substr($eregi_arr[0],1,$tag_len);
	$tag=strtolower($tag_start);

	if((($tag=="url") or ($tag=="email") or ($tag=="img")) and ($this->call_time>1)){
	echo $this->call_time."</br>";
	return $parse;//如果不能是不能嵌套的标记，直接返回
	}

	$parse2 = substr($parse,0,$pos);//标记之前
	$parse = substr($parse,$pos+$tag_len+2);//标记之后
	if(!isset($this->tags[$tag])){
	echo "$tag_start</br>";
	$ret .= $parse2.'['.$tag_start.']';
	continue;//如果是不支持的标记
	}

	//查找对对应的结束标记
	$eregi_ret=eregi("\[\/".$tag."\]",$parse,$eregi_arr);
	if(!$eregi_ret){
	$ret .= $parse2.'['.$tag_start.']';
	continue;//如果没有对应该的结束标记
	}
	$pos=strpos($parse,$eregi_arr[0]);
	$value=substr($parse,0,$pos);//这是起止标记之间的内容
	$tag_end=substr($parse,$pos+2,$tag_len);
	$parse=substr($parse,$pos+$tag_len+3);//结束标记之后的内容

	if(($tag!="url") and ($tag!="email") and ($tag!="img")){
	$value=$this->parse($value);
	}

	$ret .= $parse2;
	eval('$ret .= '.$this->tags[$tag].'("'.$tag_start.'","'.$tag_end.'","'.$value.'");');
	}
	$this->call_time--;
	return $ret;
	}

	function simple($start,$end,$value){
	return '<'.$start.'>'.$value.'</'.$end.'>';
	}

	function url($start,$end,$value){
	$trim_value=trim($value);
	if (strtolower(substr($trim_value,0,7))!="http://")
	$trim_value="http://".$trim_value;
	if($this->urlcheck->check($trim_value)) return '<a href="'.$trim_value.'" '.$this->attr_url.'>'.$value.'</a>';
	else return '['.$start.']'.$value.'[/'.$end.']';
	}

	function email($start,$end,$value){
	if(emailcheck($value)) return '<a href="mailto:'.$value.'">'.$value.'</a>';
	else return '['.$start.']'.$value.'[/'.$end.']';
	}

	function img($start,$end,$value){
	$trim_value=trim($value);
	if ((strtolower(substr($trim_value,0,7))!="http://") or ($this->urlcheck->check($trim_value)))
	return '<img src="'.$trim_value.'"></img>';
	else return '['.$start.']'.$value.'[/'.$end.']';
	}
}
//--class end
/*  
  //测试
  echo '<html>';
  echo '<head><title>测试</title></head>';
  echo '<body>';
  echo '<form action="'.str2url($PATH_INFO).'" method="post">';
  echo '<textarea cols="100" rows="10" name="ubb">'.htmlencode4textarea($ubb).'</textarea><br>';
  echo '<input type="submit" value="转换">';
  echo '</form>';
  
  if(isset($ubb)){
  $ubbcode = new ubbcode('target="_blank"');
  echo '<hr>'.$ubbcode->parse($ubb);
  }
  
  echo '</body>';
  echo '</html>';
*/  
  ?>  