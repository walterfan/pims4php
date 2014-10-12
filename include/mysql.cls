<?
/*******************************************************
 * 
 *	Programmed by Walter Fan 2003/04/14
 *
 ******************************************************/
if(!defined("__MYSQL_CLS__")) 
{
	define("__MYSQL_CLS__",TRUE);
		if(!isset($MySQL_USER)) $MySQL_USER="root";
		if(!isset($MySQL_PASSWORD)) $MySQL_PASSWORD="aibb";
		if(!isset($MySQL_DB)) $MySQL_DB="mydb";
		if(!isset($MySQL_SERVER)) $MySQL_SERVER="localhost";	
	
	class clsmysql
	{
		
	function mysql_connect()
	{
		global $MySQL_USER;
		global $MySQL_PASSWORD;
		global $MySQL_DB;
		global $MySQL_SERVER;
		$link = mysql_pconnect($MySQL_SERVER,$MySQL_USER,$MySQL_PASSWORD) or die(mysql_error());
		$i=0;
		while(!$link && $i++ < 3)
		{
			$link = mysql_pconnect($servername,$username,$password) or die(mysql_error());
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
			print "<font color=blue><a href='/prog/tool/mysql_database.php?SQL=".urlencode($strSQL)."' target='main'><u><i><b>debug</b></i></u></a>> $strSQL<br></font>";

		$conn = $this->mysql_connect();
		$result=mysql_query($strSQL,$conn) or die(mysql_error());
		//mysql_close($conn);
		return $result;
	}
	
	function GetFieldValue($rs,$fieldname)
	{
		return $rs[0][strtoupper($fieldname)];
	}

	function GetMatrix($strSQL)
	{
		$rs = array();
		$result=$this->Execute($strSQL);
		
		//$ncols = mysql_num_fields($result);
		$nrows=mysql_num_rows($result);
		
		for($j=0;$j<$nrows;$j++)
			$rs[$j]=mysql_fetch_assoc($result);
			//$rs[$j]=mysql_fetch_array($result);
        mysql_free_result($result);
        //$this->EnumPointer=0;	
        return $rs;
	}

	function GetRecordset($result)
	{
		$rs = array();
		$nrows=mysql_num_rows($result);
		for($j=0;$j<$nrows;$j++)
			$rs[$j]=mysql_fetch_assoc($result);
        mysql_free_result($result);
        return $rs;
	}


function ListForm($table)
{
		flush();
		$query="select * from $table";
		$result=$this->Execute($query);

		$nrows = mysql_num_rows($result);
		$ncols = mysql_num_fields($result);
		$column_name=array();
		for($i=0;$i< $ncols;$i++)
		{
			$column_name[$i]=mysql_field_name($result,$i); 
		}
		$rs=$this->GetRecordset($result);
		?>
		<table width="90%" border=1 cellspacing=1 cellpadding=1 style="border:2px groove #99CCFF">
		<tr  bgcolor="#99CCFF">
		<td colspan=<?=($ncols+1)?> nowrap align="center"  bgcolor="#99CCFF">ListRecord: &nbsp;&nbsp;<input type="text" name="">&nbsp;&nbsp;<input type="submit"  name="submit" value="Search">&nbsp;&nbsp;</td>
		</tr>

			<tr>
		<td><?=$table?></td>
		
		<?for($i=0;$i< $ncols;$i++)
					echo "<td>$column_name[$i]</td>";
		?>
		
		</tr>
		<?for($i=0;$i< $nrows;$i++)
		{?>
		<tr>
		<td><input type="radio" name="<?=$column_name[0]?>" value="<?=$rs[$i]["$column_name[0]"]?>"></td>
			
					<?for($j=0;$j< $ncols;$j++)
					{?>
					<td><?=htmlentities($rs[$i]["$column_name[$j]"])?></td>
					<?}?>
				
		</tr>
		<?}?>
		</table>
<?}

function AddForm($table,$file)
{
	flush();
	$query="select * from $table";
	$result=$this->Execute($query);

	$nrows = mysql_num_rows($result);
	$ncols = mysql_num_fields($result);
	$column_name=array();
	for($i=0;$i< $ncols;$i++)
	{
		$column_name[$i]=mysql_field_name($result,$i); 
	}
	?>
	<form name="<?=$table?>"  action="<?=$file?>" method="get">
	<table width="90%" border=1 cellspacing=1 cellpadding=1 style="border:2px groove #99CCFF">
	<tr bgcolor="#99CCFF">
		<td colspan=2  align="center"  bgcolor="#99CCFF">AddRecord</td>
	</tr>
	<?for($i=0;$i< $ncols;$i++)
	{?>
	<tr>
		<td align="right"><?=$column_name[$i]?>:&nbsp;&nbsp;</td><td><input type="text" name="<?=$column_name[$i]?>" size="80"></td>
	</tr>
	<?}?>
	<tr>
		<td colspan=2 align="center"><input type="submit" name="submit" value=" Add "></td>
	</tr>

	</table></form>

	<?
}

function EditForm($table,$condition="")
{
	flush();
	$query="select * from $table $condition";
	$result=$this->Execute($query);

	$nrows = mysql_num_rows($result);
	$ncols = mysql_num_fields($result);
	$column_name=array();
	
	for($i=0;$i< $ncols;$i++)
	{
		$column_name[$i]=mysql_field_name($result,$i); 
	}
	$rs=$this->GetRecordset($result);
	?>

	<table width="90%" border=1 cellspacing=1 cellpadding=1 style="border:2px groove #99CCFF">
	<tr bgcolor="#99CCFF">
		<td colspan=2 align="center"  bgcolor="#99CCFF">Edit/Delete Record</td>
	</tr>
	<?for($i=0;$i< $ncols;$i++)
	{?>
	<tr>
		<td align="right"><?=$column_name[$i]?>:&nbsp;&nbsp;</td><td><input type="text" name="<?=$column_name[$i]?>" value="<?=htmlspecialchars($rs[0]["$column_name[$i]"])?>"  size="80"></td>
	</tr>
	<?}?>
	<tr>
		<td colspan=2 align="center"><input type="submit"  name="submit" value="Update">&nbsp;&nbsp;<input type="submit"  name="submit" value="Delete"></td>
	</tr>

	</table>

	<?
}

function AddAction($table,$file)
{
	if(isset($submit)&&strtolower(trim($submit))=="add")
	{
		$strSQL="INSERT INTO buglist(CreateTime) VALUES(now())";
		flush();
		print($strSQL."<br>\n");
		$result=$this->Execute($strSQL);
		printf ("Inserted records: %d\n", mysql_affected_rows());
	}

}



}//class end
}//file end

