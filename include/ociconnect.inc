<?
if (!defined("__OCICONNECT_INC__")) 
{
	define(__OCICONNECT_INC__,TRUE);
	

	function EscapeDBString($str)
	{

		$str = str_replace("'", "''", $str);
		
		return $str;
	}
	
	function Execute($strSQL,$Color="blue")
	{
		$debugstatus=$_SESSION["debugstatus"];
		if($debugstatus=="on")
			print "<font color=$Color><b>debug:</b> ".htmlspecialchars($strSQL)."<br></font>";
	
		$conn=OCIConnect();
		$stmt=OCIParse($conn,$strSQL);
		$rtn = OCIExecute($stmt);
		if(!$rtn)
			print "This sql statement have errors:".$strSQL."<br>";
		OCIFreeStatement($stmt);
		OCICommit($conn);
		OCIDisconnect($conn);
	}
	function ExecuteOnline($cnct_obj,$strSQL,$Color="blue")
	{
		$debugstatus=$_SESSION["debugstatus"];
		if($debugstatus=="on")
			print "<font color=$Color><b>debug:</b> ".htmlspecialchars($strSQL)."<br></font>";
		$stmt=OCIParse($cnct_obj,$strSQL);
		$rtn = OCIExecute($stmt);
		if(!$rtn)
			print "This sql statement have errors:".$strSQL."<br>";
	}

	function OCIConnect($username="glbEnvORACLE_USER", $password="glbEnvORACLE_PASSWORD", $oracle_sid="glbEnvORACLE_SID")
	{
		global $glbEnvORACLE_USER;
		global $glbEnvORACLE_PASSWORD;
		global $glbEnvORACLE_SID;
		if ($username=="glbEnvORACLE_USER")
			$username=$glbEnvORACLE_USER;
		if ($password=="glbEnvORACLE_PASSWORD")
			$password=$glbEnvORACLE_PASSWORD;
		if ($oracle_sid=="glbEnvORACLE_SID")
			$oracle_sid=$glbEnvORACLE_SID;
		
		$conn = OCIPLogon($username, $password, $oracle_sid);
		//Modified by Grant.Change OCILogon into OCIPLogon.
		$i=0;
		while(OCIError() && $i++ < 3)
		{
			$conn = OCIPLogon($username, $password, $oracle_sid);
			//Modified by Grant.Change OCILogon into OCIPLogon.
			sleep(3);
		}
	
		//print("<br>OCILogon okay!");
		if(OCIError())
		{
			//header("Location: err.php");
			print "Can not connect to database, exit.";
			exit();
		}
		return $conn;
	}
	
	function OCIDisconnect($conn)
	{
		return OCILogoff($conn);
	}

	function GetFieldValue($rs,$fieldname)
	{
		return $rs[0][strtoupper($fieldname)];
	}
	
	//******* add one function by Rain
	//******* get CC information should from CC DB
	function GetCCMatrix($strSQL)
	{
		$debugstatus=$_SESSION["debugstatus"];
		if($debugstatus=="on")
			print("<font color=blue>debug> $strSQL<br></font>");

		$CCUserName		= getenv("ORACLE_CC_USER");
		$CCUserPassword = getenv("ORACLE_CC_PASSWORD");
		$CCSID			= getenv("ORACLE_CC_SID");

		$conn=OCIConnect($CCUserName,$CCUserPassword,$CCSID);
		
		//Begin a transaction
				
		//In anytime,set full time format
		$stmt=OCIParse($conn,"ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY HH24:MI:SS'");
		OCIExecute($stmt);
		
		//Parse and run SQL
		$stmt=OCIParse($conn,$strSQL);
		OCIExecute($stmt);
				
		//Get column name to array
		$ncols=OCINumCols($stmt);
		$column_name=array();
		for($i=1;$i<=$ncols;$i++)
			$column_name[$i-1]=OCIColumnName($stmt,$i);
				
		//Record set
		$i=0;
		$arr2=array();
		while(OCIFetchInto($stmt,&$arr,OCI_RETURN_LOBS))
		{
			for($j=0;$j<$ncols;$j++)
				$arr2[$column_name[$j]]=$arr[$j];

			$rs[$i]=$arr2;
			$i++;
		}
		OCIFreeStatement($stmt);
		
		//Commit a transaction
		OCICommit($conn);
				
		OCIDisconnect($conn);
			
		$this->EnumPointer=0;
			
		return $rs;
	}
		
	function GetMatrix($strSQL,$Color="blue")
	{
		$debugstatus=$_SESSION["debugstatus"];
		if($debugstatus=="on"){?>
			<font color=<?=$Color?>> <A href="/webex/database.php?strSQL=<?=urlencode($strSQL)?>" target="new"> <b>debug:</b></A><?=htmlspecialchars($strSQL)?><br></font>
		<?}
		$conn=OCIConnect();
		
		//Begin a transaction
				
		//In anytime,set full time format
		$stmt=OCIParse($conn,"ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY HH24:MI:SS'");
		$rtn0=OCIExecute($stmt);
		if(!$rtn0)
		{	
			    OCIDisconnect($conn);
			    GetMatrix($strSQL);
		}
		
		//Parse and run SQL
		$stmt=OCIParse($conn,$strSQL);
		$rtn = OCIExecute($stmt);
		if(!$rtn)
		{	
			print "This sql statement have errors:".$strSQL."<br>";
		}
			
		//Get column name to array
		$ncols=OCINumCols($stmt);
		$column_name=array();
		for($i=1;$i<=$ncols;$i++)
		{
			$column_name[$i-1]=OCIColumnName($stmt,$i);
		}
				
		//Record set
		$i=0;
		$arr2=array();
		while(OCIFetchInto($stmt,&$arr,OCI_RETURN_LOBS))
		{
			for($j=0;$j<$ncols;$j++)
			{
				$arr2[$column_name[$j]]=$arr[$j];
			}
			$rs[$i]=$arr2;
			$i++;
		}
		OCIFreeStatement($stmt);
		
		//Commit a transaction
		OCICommit($conn);
		OCIDisconnect($conn);
		return $rs;
	}
	
	function InsertAndGetValue($strInsertSQL,$ReturnItemName)  //Add by Daniel  9/6/2000
	{
			$strSQL=$strInsertSQL." returning ".strtoupper($ReturnItemName)." into :RET";

			wt("SQL=".$strSQL);	
			
			$conn = OCIConnect();						
			$stmt = OCIParse($conn, $strSQL);
		        OCIBindByName($stmt,"RET",&$ret,32);
			OCIExecute($stmt);
			OCIFreeStatement($stmt);
			OCIDisconnect($conn);
			return $ret;
	}

	function InsertCCAndGetValue($strInsertSQL,$ReturnItemName)  //Add by Rain  10/10/2000
	{
			$strSQL=$strInsertSQL." returning ".strtoupper($ReturnItemName)." into :RET";
			
			$CCUserName		= getenv("ORACLE_CC_USER");
			$CCUserPassword = getenv("ORACLE_CC_PASSWORD");
			$CCSID			= getenv("ORACLE_CC_SID");
			wt("SQL=".$strSQL);	

			$conn=OCIConnect($CCUserName,$CCUserPassword,$CCSID);
			
			$stmt = OCIParse($conn, $strSQL);
		        OCIBindByName($stmt,"RET",&$ret,32);
			OCIExecute($stmt);
			OCIFreeStatement($stmt);
			OCIDisconnect($conn);
			return $ret;
	}

	function GetOneFromOne($targetfield,$tablename,$examfield,$examvalue,$examtype)
	{
		$strSQL = "";
		if ($targetfield == "")     //judge if record exist who examfield=examvalue
		{
			if ($examtype == 0)     //number
				$strSQL = "select 1 from ".EscapeDBString($tablename)." where ".EscapeDBString($examfield)."=".$examvalue;
			else
				$strSQL = "select 1 from ".EscapeDBString($tablename)." where ".EscapeDBString($examfield)."='".EscapeDBString($examvalue)."'";
		}
		else
		{
			if ($examtype == 0)
				$strSQL = "select ".EscapeDBString($targetfield)." from ".EscapeDBString($tablename)." where ".EscapeDBString($examfield)."=".$examvalue;
			else
				$strSQL = "select ".EscapeDBString($targetfield)." from ".EscapeDBString($tablename)." where ".EscapeDBString($examfield)."='".EscapeDBString($examvalue)."'";
		}
	
		$rs = GetMatrix($strSQL);
	
		if(sizeof($rs)==0)
			return 0;
		else
			if ($targetfield == "")
				return $rs[0]["1"];
			else
				return $rs[0][strtoupper(EscapeDBString($targetfield))];
	}

	function UpdateOneFromOne($targetfield,$targetvalue,$targettype,$tablename,$examfield,$examvalue,$examtype) // add by Rain 12/9
	{
		$strSQL = "";
		
		if ($targettype == 9)	//update date field
		{
			if ($examtype==0)
				$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."=".
						 "TO_DATE('". $targetvalue ."','mm/dd/YYYY hh24:mi:ss') where ".EscapeDBString($examfield)."=".$examvalue;
			else
				$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."='".
						 "TO_DATE('". $targetvalue ."','mm/dd/YYYY hh24:mi:ss') where ".EscapeDBString($examfield)."=".$examvalue;
		}
		else
		{
			if ($targettype==0&&$examtype==0)
				$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."=".
						 $targetvalue." where ".EscapeDBString($examfield)."=".$examvalue;
			if ($targettype==0&&$examtype!=0)
				$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."=".
						 $targetvalue." where ".EscapeDBString($examfield)."='".EscapeDBString($examvalue)."'";
			if ($targettype!=0&&$examtype==0)
				$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."='".
						 EscapeDBString($targetvalue)." where ".EscapeDBString($examfield)."=".$examvalue;
			if ($targettype!=0&&$examtype!=0)
				$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."='".
						 EscapeDBString($targetvalue)." where ".EscapeDBString($examfield)."='".EscapeDBString($examvalue)."'";
	    }
	    
		Execute($strSQL);
	}
	
	function UpdateOneFromTwo($targetfield,$targetvalue,$targettype,$tablename,$examfield1,$examvalue1,$examtype1,$examfield2,$examvalue2,$examtype2)// add by Rain 12/9
	{
		$strSQL = "";
		if ($targettype==0 && $examtype1==0 && $examtype2==0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."=".
					 $targetvalue." where ".EscapeDBString($examfield1)."=".$examvalue1." and ".
					 EscapeDBString($examfield2)."=".$examvalue2;
		if ($targettype==0&&$examtype1!=0&&$examtype2==0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."=".
					 $targetvalue." where ".EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."'"." and ".
					 EscapeDBString($examfield2)."=".$examvalue2;
		if ($targettype!=0&&$examtype1==0&&$examtype2==0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."='".
					 EscapeDBString($targetvalue)." where ".EscapeDBString($examfield1)."=".$examvalue1." and ".
					 EscapeDBString($examfield2)."=".$examvalue2;
		if ($targettype!=0&&$examtype1!=0&&$examtype2==0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."='".
					 EscapeDBString($targetvalue)." where ".EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."'"." and ".
					 EscapeDBString($examfield2)."=".$examvalue2;
		if ($targettype==0&&$examtype1==0&&$examtype2!=0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."=".
					 $targetvalue." where ".EscapeDBString($examfield1)."=".$examvalue1." and ".
					 EscapeDBString($examfield2)."=".EscapeDBString($examvalue2)."'";
		if ($targettype==0&&$examtype1!=0&&$examtype2!=0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."=".
					 targetvalue." where ".EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."'"." and ".
					 EscapeDBString($examfield2)."=".EscapeDBString($examvalue2)."'";
		if ($targettype!=0&&$examtype1==0&&$examtype2!=0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."='".
					 EscapeDBString($targetvalue)." where ".EscapeDBString($examfield1)."=".$examvalue1." and ".
					 EscapeDBString($examfield2)."=".EscapeDBString($examvalue2)."'";
		if ($targettype!=0&&$examtype1!=0&&$examtype2!=0)
			$strSQL = "update ".EscapeDBString($tablename)." set ".EscapeDBString($targetfield)."='".
					 EscapeDBString($targetvalue)." where ".EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."'"." and ".
					 EscapeDBString($examfield2)."=".EscapeDBString($examvalue2)."'";
		Execute($strSQL);
	}

 	function GetOneFromTwo($targetfield,$tablename,$examfield1,$examvalue1,$examtype1,$examfield2,$examvalue2,$examtype2)
	{
		$strSQL = "";
		if ($targetfield == "") {     //judge if record exist who examfield=examvalue
			if (($examtype1 == 0) && ($examtype2 == 0))     //number
				$strSQL = "select 1 as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."=".$examvalue1." and ".
						EscapeDBString($examfield2)."=".$examvalue2;
			else if (($examtype1 == 0) && ($examtype2 == 1))
				$strSQL = "select 1 as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."=".$examvalue1." and ".
						EscapeDBString($examfield2)."='".EscapeDBString($examvalue2)."'";
			else if (($examtype1 == 1)&&($examtype2 == 0))
				$strSQL = "select 1 as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."' and ".
						EscapeDBString($examfield2)."=".$examvalue2;
			else if (($examtype1 == 1)&&($examtype2 == 1))
				$strSQL = "select 1 as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."' and ".
						EscapeDBString($examfield2)."='".EscapeDBString($examvalue2)."'";
		}
		else {
			if (($examtype1 == 0) && ($examtype2 == 0))     //number
				$strSQL = "select ".EscapeDBString($targetfield). 
						" as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."=".$examvalue1." and ".
						EscapeDBString($examfield2)."=".$examvalue2;
			else if (($examtype1 == 0) && ($examtype2 == 1))
				$strSQL = "select ".EscapeDBString($targetfield).
						" as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."=".$examvalue1." and ".
						EscapeDBString($examfield2)."='".EscapeDBString($examvalue2)."'";
			else if (($examtype1 == 1) && ($examtype2 == 0))
				$strSQL = "select ".EscapeDBString($targetfield).
						" as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."' and ".
						EscapeDBString($examfield2)."=".$examvalue2;
			else if (($examtype1 == 1) && ($examtype2 == 1))
				$strSQL = "select ".EscapeDBString($targetfield).
						" as ResultValue from ".EscapeDBString($tablename)." where ".
						EscapeDBString($examfield1)."='".EscapeDBString($examvalue1)."' and ".
						EscapeDBString($examfield2)."='".EscapeDBString($examvalue2)."'";
		}

		$matrix = GetMatrix($strSQL);
		if(count($matrix) > 0)
			$ret = GetFieldValue($matrix,"ResultValue");
		else
			$ret = 0;
		return $ret;
	}
}	// end of !define("__OCICONNECT_INC__")
?>