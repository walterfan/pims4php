<?
/*******************************************************
 * 
 *	Programmed by Walter Fan 2003/04/14
 *
 ******************************************************/
if(!defined("__DAO_CLS__")) 
{
define("__DAO_CLS__",TRUE);

define("VARCHAR",0);
define("NUMBER",1);
define("DATE",2);
define("BINARY",3);

class DAO
{
    var $name;
    var $type;
    var $len; 
    var $val;

    function DAO($name,$val="",$type=VARCHAR,$len=10)
    {
        $this->name = $name;
        $this->type = $type;
         $this->len = $len;
        $this->val = $val;
    }

    function SetType($type)
    {
        $this->type = $type;
    }

    function GetSQLVal()
    {
        if($this->type==VARCHAR)
            return "'".$this->val."'";
        else
            return $this->val;
    }
}

class DataModel
{
    var $table;
    var $conn;
    function DataModel($table,$conn="")
    {
        $this->table=$table;
        if($conn=="")
            $this->conn=getConnection();
        else
            $this->conn=$conn;
    }

	function Create($arrDao)
	{
        $names="";
        $vals="";
        for($i=0;$i<count($arrDao);$i++)
        {
            if($i>0)
            {    
                $names.=",";
                $vals.=",";
            }
  
            $names.=$arrDao[$i]->name;
            $vals.=$arrDao[$i]->GetSQLVal();

        }
	    $createsql="insert into ".$this->table." (".$names.") values (".$vals.")";
        echo $createsql;
        return $this->conn->Execute($createsql);
	}

	function Retrieve($arrDao="", $lines="", $fromline="")
	{
	    $retrievesql="select * from ".$this->table." where 1=1 ";
        if($arrDao!="")
        {    
            $retrievesql.=" and ";
            for($i=0;$i<count($arrDao);$i++)
            {
                if($i>0)
                {    
                    $retrievesql.=" and ";
                }      
                $retrievesql.=$arrDao[$i]->name."=".$arrDao[$i]->GetSQLVal();
            }
        }
        if($lines!="")
        {    
            if($fromline=="")
                $retrievesql.="limit $lines";
            else
                $retrievesql.="limit $fromline, $lines";
        }
        echo $retrievesql;
        return $this->conn->GetAll($retrievesql);
	}

	function Update($arrDao,$arrCondDao="")
	{
	    $updatesql="update ".$this->table." set ";
        for($i=0;$i<count($arrDao);$i++)
        {
            if($i>0)
            {    
                $updatesql.=", ";
            }      
            $updatesql.=$arrDao[$i]->name."=".$arrDao[$i]->GetSQLVal();
        }
        if($arrCondDao!="")
        {
            $updatesql.=" where ";
            for($i=0;$i<count($arrCondDao);$i++)
            {
                if($i>0)
                {    
                    $updatesql.=" and ";
                }      
                $updatesql.=$arrCondDao[$i]->name."=".$arrCondDao[$i]->GetSQLVal();
            }
        }
        echo $updatesql;
        return $this->conn->Execute($updatesql);
	}

	function Delete($arrDao)
	{
	        $deletesql.=" delete from ".$this->table;
            if($arrDao!="")
            {
                $deletesql.=" where ";
                for($i=0;$i<count($arrDao);$i++)
                {
                    if($i>0)
                    {    
                        $deletesql.=" and ";
                    }      
                    $deletesql.=$arrDao[$i]->name."=".$arrDao[$i]->GetSQLVal();
                }
            }
                echo $deletesql;
                return $this->conn->Execute($deletesql);
	}

}//end of class

class DataController
{
    var $urlmap;
    function DataController($urlmap)
    {
        $this->urlmap=$urlmap;
    }

    function SetUrl($module,$url)
    {
        $this->urlmap[$module]=$url;
    }

	function GetUrl($module)
	{	    
        if (array_key_exists($module, $this->urlmap)) 
            return $this->urlmap[$module];
        else
            return "";
	}
}

class DataView
{
    var $arrStyle;
    var $formname;
    var $formaction;
    var $formmethod;
    function DataView($formname="frmData",$formaction="")
    {
        $this->formname=$formname;
        $this->formaction=$formaction;
        $this->formmethod="POST";
    }
    function EchoHeader($title)
    {
    ?>
    <table><form method="<?=$this->formmethod?>" action="<?=$this->formaction?>"><tr><td><?=$title?></td></tr>
    <?
    }
    function EchoFooter()
    {
    ?>
    </form>
    </table>
    <?
    }
    function EchoTitle($record)
    {
        foreach($record as $key => $value)
        {
			if(is_int($key))
				continue;
        ?>
        <td><?=$key?></td>
        <?
        }
    }

    function EchoRecord($record)
    {
        foreach($record as $key => $value)
        {
			if(is_int($key))
				continue;
        ?>
        <td><?=$value?></td>
        <?
        }
    }


	function AddView($matrix)
	{

	}

	function EditView()
	{
	
	}

	function ListView($matrix)
	{
        //echo "<hr>line".__LINE__.": ";print_r($matrix,true);
		echo "<table class='daotab'>";
		for($i=0;$i<count($matrix);$i++)
        {
			echo "<tr>";
			if($i==0)
				$this->EchoTitle($matrix[$i]);
			$this->EchoRecord($matrix[$i]);
			echo "</tr>";
		}
		echo "</table>";
	}
}//end of class
}//--end of this file
?>