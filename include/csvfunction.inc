<?
if(!defined("__CSVFUNCTION_INC__"))
{
		define("__CSVFUNCTION_INC__",TRUE);

	function getArrayFromCSV($csvfile)
	{
		$arrcsv=array();
		$arrcolname=array();
		$arrrowdata=array();
		$row = 0;
		$handle = fopen ($csvfile,"r");
		while ($data = fgetcsv ($handle, MAXLINELENGTH, ",")) 
		{
			$num = count ($data);
			if ($row==0)
				$arrcolname=$data;
			else
			{
				for ($i=0; $i < $num; $i++) 
				{
					 $arrrowdata[$arrcolname[$i]]=$data[$i]. "<br>\n";
				}
				$arrcsv[$row-1]=$arrrowdata;
			}
			$row++;
		}
		fclose ($handle);
		return $arrcsv;
	}
}
?>