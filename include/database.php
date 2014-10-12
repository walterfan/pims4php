<?
/*Function List:--------------------------------------------------------------------
* GetFieldValue($stmt,$FieldName)						   *
* GetMatrix($stmt)								   *
* GetValue($stmt)								   *
* Init()									   *
* OCIConnect()				   					   *
* OCIDisconnect($conn)								   *
* PrepareSQL($conn,$strSQL)							   *
* GetATable($strTableName)							   *
* DoSql($strSQL)								   *
----------------------------------------------------------------------------------*/
$MySQL_USER = "root";
$MySQL_PASSWORD = "aibb";
if(!isset($DB)) $DB="mydb";
$MySQL_DB = $DB;
$MySQL_SERVER="localhost";
class Database
{
	function OCIConnect($ORACLE_USER= "", $ORACLE_PASSWORD = "",$ORACLE_SID="")
	{
		global $MySQL_USER;
		global $MySQL_PASSWORD;
		global $MySQL_DB;
		global $MySQL_SERVER;
		$link = mysql_pconnect($MySQL_SERVER,$MySQL_USER,$MySQL_PASSWORD) or die(mysql_error());
		$i=0;
		while(!$link && $i++ < 3)
		{
			$link = @mysql_pconnect($servername,$username,$password) or die(mysql_error());
			sleep(3);
		}

		if(!mysql_select_db($MySQL_DB))
		{
			print "Can not connect to database, exit.";
			exit();
		}
		return $link;
	}

	function Execute($strSQL)
	{
		global $debugstatus;
		if($debugstatus=="on")
			print "<font color=blue>debug> $strSQL<br></font>";

		$conn = $this->OCIConnect();
		//$stmt=OCIParse($conn,$strSQL);
		//OCIExecute($stmt);
		//OCICommit($conn);
		//$this->OCIDisconnect($conn);
		$result=mysql_query($strSQL,$conn) or die(mysql_error());
		mysql_close($conn);
		return $result;
	}
	
	function GetFieldValue($rs,$fieldname)
	{
		return $rs[0][strtoupper($fieldname)];
	}

	function GetMatrix($ResultSet)
	{
        $arrResultSet = array();
       
		$ncols = mysql_num_fields($ResultSet);
		$column_name=array();
		for($i=0;$i< $ncols;$i++)
		{
			$column_name[$i]=mysql_field_name($ResultSet,$i); 
		}
			        
        $nCount = 0 ;
        while($arr = @mysql_fetch_assoc($ResultSet))
		{
			for($j=0;$j<$ncols;$j++)
			{
				$arr2[strtoupper($column_name[$j])]=$arr[$column_name[$j]];
			}
			$rs[$nCount]=$arr2;
			$nCount ++ ;
       	}
       	
        mysql_free_result($ResultSet);
        //$this->EnumPointer=0;	
        return $rs;
	}
	
/*
	function OCIDisconnect($conn)
	{
		OCILogoff($conn);
	}

	function PrepareSQL($conn,$strSQL)
	{
		global $debugstatus;
		if($debugstatus=="on")
			print "<font color=blue>debug> $strSQL<br></font>";
		$query = "ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY HH12:MI AM'";
		$stmt = OCIParse($conn, $query);
		OCIExecute($stmt);
		$stmt=OCIParse($conn,$strSQL);
		OCIExecute($stmt);
		return $stmt;
	}
	

	//GetATable and ExcuteSql is a test function
	function GetATable($strTableName)
	{
		$conn = $this->OCIConnect("test","oracle");
		$strSQL = "SELECT * FROM ".$strTableName;
		$stmt=OCIParse($conn,$strSQL);
		OCIExecute($stmt);

		$nrows = OCIFetchStatement($stmt,$results);
		if ( $nrows > 0 ) 
		{
   			print "<TABLE BORDER=\"1\">\n";
   			print "<TR>\n";
   		
   			while ( list( $key, $val ) = each( $results ) ) 
   			{
      				print "<TH>$key</TH>\n";
   			}
   		
   			print "</TR>\n";
   
   			for ( $i = 0; $i < $nrows; $i++ ) 
   			{
      				reset($results);
      				print "<TR>\n";
      				while ( $column = each($results) ) 
      				{   
         				$data = $column['value'];
         				print "<TD>$data[$i]</TD>\n";
      				}
      			print "</TR>\n";
   			}
   			print "</TABLE>\n";
		} 
		else 
		{
   			echo "No data found<BR>\n";
		}      
	
		$this->OCIDisconnect($conn);
	}
*/	
	function DoSql($strSQL)
	{
		$strSQL = trim($strSQL);
		$conn = $this->OCIConnect();
		$result=$this->Execute($strSQL);
		
		if(strtolower(substr($strSQL,0,6))!="select")
			{
				if(!mysql_Error($conn))
					print("<br>Excuted Success<br>");
				else
					print("<br>Excuted Not success<br>");
				return;
			}
			
		$nrows = mysql_num_rows($result);
		$ncols = mysql_num_fields($result);
		print("Col:$ncols, Row:$nrows");
		if ( $nrows > 0 ) 
		{
			$rs=$this->getmatrix($result);
   			print "<TABLE BORDER=\"1\">\n";
   			print "<TR>\n";
   		
   			while ( list( $key, $val ) = each( $rs[0] ) ) 
   			{
      				print "<TH>$key</TH>\n";
   			}
   		
   			print "</TR>\n";
   
   			for ( $i = 0; $i < $nrows; $i++ ) 
   			{
      				reset($rs[$i]);
      				print "<TR>\n";
      				while (  list( $key, $val ) = each( $rs[$i] ) ) 
      				{   
         				print "<TD>$val</TD>\n";
      				}
      			print "</TR>\n";
   			}
   			print "</TABLE>\n";
		} 
		else 
		{
   			echo "No data found<BR>\n";
		}      
	
		mysql_close($conn);
	}
}

function DisplaySQLResult($strSQL)
{
	if($strSQL != "")
	{
		print("<br>Your SQL Statement:<br>$strSQL<BR>");
		$atas = new Database;
		$atas->DoSql($strSQL);
	}
}
function DisplayWebexTable($strTableList)
{
	if($strTableList=="")
		DisplayAllTable();
	$arrTableList = explode(",",$strTableList);
	
	for($i=0;$i<sizeof($arrTableList);$i++)
		DisplayTable($arrTableList[$i]);
}

function DisplayAllTable()
{	
	$sql = "SELECT table_name FROM dba_table WHERE owner='TEST'";
	$atas = new Database;
	$atas->DoSql($sql);
}
function DisplayTable($strTableName)
{
?>
	<TABLE BORDER=1>
	<CAPTION ALIGN=LEFT>
	wbxCalendar--<INPUT TYPE="checkbox" NAME="DisplayAll">Always Display<BR>
	<A HREF="structure.php" onclick="window.open('structrue.php');exit(0);">
	Display structrue of wbxCalendar</A>
	</CAPTION>
	<TR>
		<TD>Table Name</TD>
		<TD>Display Structure</TD>
		<TD>Display Data</TD>
	</TR>
	</TABLE>
<?
}
if(!isset($SQL)) $SQL="show tables";
$SQL = $SQL."";
if(trim($SQL)!="")
	$SQL = str_replace("\'","'",$SQL);
?>
<HTML>
<HEAD>
<TITLE>Webex Database Manager Tools</TITLE>
</HEAD>

<BODY BGCOLOR="#FFFFFF">
<TABLE WIDTH=100%>
<TR>
	<TD ALIGN=CENTER>Webex Database Manager Tools Version 1.0.3</TD>
</TR>
<TR>
	<TD ALIGN=RIGHT><I>Writed by Summer Fang/Modified by Walter Fan for mysql:2003-1-30</I></TD>
</TR>
</TABLE>

<TABLE>
<TR><FORM METHOD=POST ACTION="database.php">
	<TD>Such as : select * from table limit 10<BR>
<TEXTAREA NAME="SQL" ROWS="10" COLS="60">
<?	print($SQL);
?>
</TEXTAREA></TD>
	<TD align=right>
	Server&nbsp;&nbsp;<input type="text" name="MySQL_SERVER" value="localhost" size=10><BR>
	User&nbsp;&nbsp;<input type="text" value="******" size=10><BR>
	Pass&nbsp;&nbsp;<input type="password"  value="******" size=10 READONLY><BR>
	DB&nbsp;&nbsp;<input type="text" name="DB"  value="mydb" size=10><BR><BR>
	<INPUT TYPE="submit" NAME="SubmitSQL" VALUE="Excute SQL">
</TD>
</TR>
</TABLE>

<?
if($SQL != "")
	DisplaySQLResult($SQL);

//DisplayWebexTable($strTableList)

?>
</FORM>
<CENTER>

</BODY>
</HTML>

