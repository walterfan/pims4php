<?
if(!defined("__TREEMENU_INC__"))
{
	define("__TREEMENU_INC__",TRUE);

	function treemenu($parentmenu_title,$childmenu_title,$childmenu_link,$childmenu_target)
	{
	//some javascript function for tree menu
	$titlesize=18;
	?>
	<script language="JavaScript">
		scores = new Array(20);
		var numTotal=0;
		NS4 = (document.layers) ? 1 : 0;
		IE4 = (document.all) ? 1 : 0;
		ver4 = (NS4 || IE4) ? 1 : 0;
		if (ver4) {    
			with (document) 
			{
				write("<STYLE TYPE='text/css'>");
				if (NS4) {
					write(".parent {position:absolute; visibility:visible}");           
					write(".child {position:absolute; visibility:visible}");            
					write(".regular {position:absolute; visibility:visible}")
			}        
			else {            
				write(".child {display:none}")        
			} 
			write("</STYLE>");    
		}
		}
	function getIndex(el) 
	{
		ind = null;
		for (i=0; i<document.layers.length; i++)
		{
			whichEl = document.layers[i];
			if (whichEl.id == el) 
			{
				ind = i;
				break; 
		    }
		}
		return ind;
	}

	function arrange()
	{    
		nextY = document.layers[firstInd].pageY +document.layers[firstInd].document.height;    	for (i=firstInd+1; i<document.layers.length; i++)
		{
			whichEl = document.layers[i];
			if (whichEl.visibility != "hide") 
			{            
				whichEl.pageY = nextY;            
				nextY += whichEl.document.height;        
			}    
		}
	}

	function initIt()
	{    
		if (!ver4) 
			return;    
		if (NS4) 
		{        
			for (i=0; i<document.layers.length; i++) 
			{            
				whichEl = document.layers[i];            
				if (whichEl.id.indexOf("Child") != -1) 
					whichEl.visibility = "hide";       
			}        
			arrange();    
		}    
		else
		{        
			divColl = document.all.tags("DIV");        
			for (i=0; i<divColl.length; i++) 
			{            
				whichEl = divColl(i);            
				if (whichEl.className == "child") 
					whichEl.style.display = "none";        
			}    
		}
	}

	function expandIt(el) 
	{	
		if (!ver4) return;    
		if (IE4) 
		{        
			whichEl1 = eval(el + "Child");		
			for(i=1;i<=numTotal;i++)
			{			
				whichEl = eval(scores[i] + "Child");			
				if(whichEl!=whichEl1) 
				{				
					whichEl.style.display = "none";			
				}		
				}        
				whichEl1 = eval(el + "Child");        
				if (whichEl1.style.display == "none") 
				{            
					whichEl1.style.display = "block";        
				}        
				else 
				{            
					whichEl1.style.display = "none";        
				}    
		}    
		else 
		{        
			whichEl = eval("document." + el + "Child");		
			for(i=1;i<=numTotal;i++)
			{			
				whichEl = eval("document." + scores[i] + "Child");			
				if(whichEl!=whichEl1) 
				{				
					whichEl.visibility = "hide";			
					}		
				}        
				if (whichEl.visibility == "hide") 
				{            
					whichEl.visibility = "show";        
				}        
				else 
				{            
					whichEl.visibility = "hide";        
				}        
				arrange();    
		}
	}

	//onload = initIt;
	</script>
	<?
		//----------------------------------split array--------------------------------//
		$parentmenu=preg_split ("/;/", $parentmenu_title);

		$childmenustr=preg_split ("/;/", $childmenu_title);
		for($i=0;$i<count($childmenustr);$i++)
			${"childmenu_".$i}=preg_split ("/,/", $childmenustr[$i]);

		$targetstr=preg_split ("/;/", $childmenu_target);
		for($i=0;$i<count($targetstr);$i++)
			${"target_".$i}=preg_split ("/,/", $targetstr[$i]);

		$linkstr=preg_split ("/;/", $childmenu_link);

		for($i=0;$i<count($linkstr);$i++)
			${"link_".$i}=preg_split ("/,/", $linkstr[$i]);

		//----------------------------generate menu---------------------------------------//

		echo "<script>  	numtotal=".count($parentmenu).";\n";
		for($i=0;$i<count($parentmenu);$i++)
			echo " 	scores[".$i."]='KB".$i."';\n";
		echo "</script>\n";

		//-------------------------generate parent menu-----------------------------//
		for($i=0;$i<count($parentmenu);$i++)
		{
			echo "<DIV id='KB".$i."Parent' class='parent'>\n";
			echo "<a href='".$PHP_SELF."' onclick=\"expandIt('KB".$i."'); return false\" title='".trim($parentmenu[$i])."' class='menutext1'>\n";
			echo "<img src='/prog/images/plus.gif' border=0>\n";
			echo local_substr(trim($parentmenu[$i]),0,$titlesize)."</a></DIV>\n";
			//-------------------------generate child menu-----------------------------//
			echo "<DIV id='KB".$i."Child' class='child'>\n";
			for($j=0;$j<count(${"childmenu_".$i});$j++)
			{
				echo "<img src='/prog/images/blank.gif' border=0><a class='menutext2' href='".${"link_".$i}[$j]."' target='".${"target_".$i}[$j]."' title='".trim(${"childmenu_".$i}[$j])."'>".local_substr(trim(${"childmenu_".$i}[$j]),0,$titlesize)."</a><br>\n";
			}
			echo "	</DIV>\n";

		}
	}
}//--------------------------------end this file
?>
