<?
/*************************************************************************************				Module:			mysite
*				Author:			Walter
*				Date:			9/24/2002
*				Last modified		1/4/2003
*				Functions:								
************************************************************************************/
if(!defined("__DAILYWORK_INC__"))
{
define("__DAILYWORK_INC__",TRUE);

function DisplayCalendar($getarr="")
{
	global $basepath;
	extract($getarr,EXTR_SKIP);
	if(isset($year)&&isset($month)&&isset($date)&&($date<=31)&&($date>=1))
	{	
		if($month>12)	 {
			$month=1;
			$year++;
		}
		else if($month<1)	 {
			$month=12;
			$year--;
		
		}

		$distance=ceil((time()-mktime (date(H),date(i),date(s),$month,$date,$year))/(60*60*24));
	}
	else
	{
		$year=intval(date("Y"));
		$month=intval(date("m"));
		$date=intval(date("d"));
	}
	//echodebug($year."-".$month."-".$date,__LINE__);
	if(!isset($distance)||$distance<0)
		$distance=0;
	?>
	<script language="JavaScript">
	//mNames=new Array("January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December");//month name
	mNames=new Array("1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12");//month name
	nNames=new Array("○","一","二","三","四","六","七","八","九","十");
	mDays=new Array(31,28,31,30,31,30,31,31,30,31,30,31);//month days
	//get date as php variable:$year,$month,$date
	//note : javascript month is 0 to 11,so use ($month-s1)
	now=new Date(<?echo $year.",".($month-1).",".$date?>);
	day=now.getDay();//week day
	date=now.getDate();//month day
	month=now.getMonth();
	year=now.getFullYear();

	//Februeay have 29 days in leap year
	if((year%4==0&&year%100!=0)||year%400==0)
		mDays[1]=29;
	//judge which week day is the first day of a month
	spaces=date;
	while(spaces>7)
		spaces-=7;

	spaces=day-spaces+1;
	if(spaces<0)
		spaces+=7;

	with(document)
	{
		write("<table id='calendarbox' cellspacing=1 cellpadding=0>");
		write("<tr bgcolor=#0099CC><td colspan=7 align=center nowrap>");
		write("<a href='<?=$_SERVER["PHP_SELF"]?>?year="+year+"&month="+<?=$month-1?>+"&date="+date+"&action=listtoday'>");
		write("<img src='<?=$basepath?>/images/triangle_blue_left.gif'  border=0 alt='last month'></a>");
		write("&nbsp;"+year+"年"+mNames[month]+"月"+date+"日&nbsp;");
		write("<a href='<?=$_SERVER["PHP_SELF"]?>?year="+year+"&month="+<?=$month+1?>+"&date="+date+"&action=listtoday'>");
		write("<img src='<?=$basepath?>/images/triangle_blue_right.gif'  border=0 alt='next month'></a>");
		write("</td></tr><tr bgcolor=#666699 align=center>\
			<td>日</td>\
			<td>一</td>\
			<td>二</td>\
			<td>三</td>\
			<td>四</td>\
			<td>五</td>\
			<td>六</td>\
			</tr>");
		col=0;
		write("<tr>");
		for(s=0;s<spaces;s++)
		{
			write("<td>&nbsp;</td>");
			col++;
		}
		count=1;
		while(count<=mDays[month])
		{
		if(count==date)
			write("<td ><a  class='bluetext'  href='<?=$PHP_SELF?>?year="+year+"&month="+(month+1)+"&date="+count+"&action=listtoday'><b><u>"+count+"</u></b></a></td>")
		else
			write("<td ><a  class='tdtext'  href='<?=$PHP_SELF?>?year="+year+"&month="+(month+1)+"&date="+count+"&action=listtoday'>"+count+"</a></td>")
		count++;
		col++
		if(col==7)
		{
			col=0
			write("</tr><tr>")
		}
		
		}
		write("</tr></table>");

	}


	</script>	

	<?


}


function dodailywork($action,$question,$answer,$knowledgeid,$classid=0)
{
	$atas=new clsmysql;
	$action=strtolower(trim($action));
	$classid=intval($classid);
	$chapter=intval($chapter);
	$section=intval($section);
	if(strtolower(trim($action))=="insert"&&$question!="")
	{
		$insertsql="insert into knowledge(question,answer,classid,lastmodifiedtime) values('".$question."','".$answer."',".$classid.",now())";
		$result=$atas->Execute($insertsql);
		//$insertsql2="insert into knowclass(knowledgeid,classid) values(knowledgeid+1,$classid)";
		//$result=$atas->Execute($insertsql2);
		 $action="listtoday";
	}
	else if(strtolower(trim($action))=="update"&&$knowledgeid!="")
	{
		$updatesql="update knowledge set question='".$question."',answer='".$answer."',classid=".$classid." where  knowledgeid=".$knowledgeid;
		$result=$atas->Execute($updatesql);
		//$updatesql2="update knowclass set classid=$classid where knowledgeid=$knowledgeid";
		//$result=$atas->Execute($updatesql2);
		//$action="listtoday";
		JSReplace($PHP_SELF."?knowledgeid=".$knowledgeid);
	}
	else if(strtolower(trim($action))=="delete"&&$knowledgeid!="")
	{
			echo "<script language='JavaScript'>
			var truthBeDel=confirm('Delete the record?');
			if (truthBeDel) {
			var act='todelete';	
			location.replace('dailywork.php?action='+act+'&knowledgeid=".$knowledgeid."');}	</script>";
	}
	else if(strtolower(trim($action))=="todelete")
	{
			
		$deletesql1="delete from knowledge where knowledgeid=".$knowledgeid;
		//$deletesql2="delete from knowclass where knowledgeid=".$knowledgeid;
		$result=$atas->Execute($deletesql1);
		//$result=$atas->Execute($deletesql2);
		$action="listtoday";
	}
	else
	{	
		return $action;
	}
	return $action;
}

function getdailywork($action,$distance,$knowledgeid,$strSortBy,$strOrder,$classid,$keyword="")
{
	global $atas;
	$strorg="select knowledgeid, question,answer,t1.lastmodifiedtime,t1.classid,classname ";
	$strorg.="from knowledge as t1 LEFT JOIN class as t2 ON t1.classid = t2.classid";
	
	if(strtolower(trim($action))=="listtoday"&&!isset($knowledgeid))
		$strsql=$strorg." where to_days(now()) - to_days(t1.lastmodifiedtime) =".$distance;
	else
		$strsql=$strorg;
	if (!isset($knowledgeid)||$knowledgeid=="") 
		$strsql=$strsql;
	else 
	{
	    if ($strsql==$strorg )
	    	$strsql.=" where knowledgeid=".$knowledgeid;
	    else
		$strsql.=" and knowledgeid=".$knowledgeid;
         }
	if ($classid<0) 
		$strsql=$strsql;
	else 
	{
	    if ($strsql==$strorg )
		$strsql.=" where t1.classid=".$classid;
	    else
	        $strsql.=" and t1.classid=".$classid;
        }

	if ($keyword=="") 
		$strsql=$strsql;
	else 
	{
	    if ($strsql==$strorg )
		$strsql.=" where question like '%".$keyword."%' or answer like '%".$keyword."%'";
	    else
	        $strsql.=" and question like '%".$keyword."%' or answer like '%".$keyword."%'";
        }

	if (!isset($strSortBy))  $strSortBy=" knowledgeid";
	if (!isset($strOrder))  $strOrder=" desc";
	if (isset($strSortBy)&&$strSortBy!="")
	{	
		$strsql.=" order by ".$strSortBy;
		if (isset($strOrder))
			$strsql.=" ".$strOrder;
	}

	$rs=array();
	//echo $strsql;
	$rs=$atas->GetMatrix($strsql);
	return $rs;
}
function getclassarr()
{
	global $classarr,$atas;
	if(!isset($classarr))
	{	
	$classsql="select classid,classname from class";
	$classarr=$atas->GetMatrix($classsql);
	session_register("classarr");
	}
	return $classarr;

}

}//__FUNCTION_INC__end	

?>