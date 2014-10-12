<?
include_once("config.php");
include_once(GetHomePath()."/include/basefunction.inc");
include_once(GetHomePath()."/include/logon.inc");
include_once(GetHomePath().'/include/dh.php'); 
include_once(GetHomePath().'/include/initdrv.php');
include_once(GetHomePath().'/include/header.inc');

$folder="/workspace/cpp/cwhat/minute";
$arrFiles=getFileList($folder,0);
krsort($arrFiles);
?>
<style type="text/css">
#leftDiv ul{
	font-family:arial;
	font-size:18px;
	text-align:left;
	color:blue;
	line-height: 28px;
}
.listtab td {
	border:1px solid #336699;
	font-size:14px;
}
.tabheader{
	border:2px groov #99ccff;
	background-color: #DDDDEE;
	line-height: 32px;
	font-family:arial;
	font-size: 24px;
	text-align: left;
	padding: 0.5em

}
</style>
<center>
<TABLE WIDTH="90%">
<TR>
	<TD class="tabheader"><strong>My Journal</strong></TD>
</TR>
</TABLE>
<table width='90%' cellpadding='2' border='0' class="listtab">
<tr bgcolor="#6699EE">
<td width="20%" align="center">&nbsp;</td>
<td width="5%" align="center">SN</td>
<td width="25%" align="center">Title</td>
<td width="20%" align="center">File Name</td>
<td width="20%" align="center">Abstract</td>
<td width="10%" align="center">Update Date</td>

</tr>
<?
$i=0;
foreach($arrFiles as $file)
{
	$dotpos = strrpos($file, ".");
	$ext = substr($file, $dotpos +1);
	if($ext != "md" && $ext != "rst")
        continue;
    $i++;
?>
<tr>
<?
if($i==1) 
{
	?>
	<td width="20%" rowspan="<?=count($arrFiles)?>" valign="top" 
	style="background-color:#cceeee;">
	<div id="leftDiv">
<ul>
<li>Self-reverence, self-knowledge, self-control, these three alone lead life to sovereign power -- Alfred Tennyson</li>

<li>Excellence in any department can be attained only by the labor of a lifetime; it is not to be purchased at a lesser price -- Samuel Johnson</li>

<li>Our deeds determine us, much as we determine our deeds</li>

<li>#Maxim# Our greatest glory consists not in never falling but in rising every time we fall.</li>

<li>It's not who you are underneath, it's what you do that defines you</li>
	</ul></div>
-- by Walter Fan on 2003-1-30
	</td>
	<?
}
$lastpos = strrpos($file, "-");
if($lastpos === false) {
	$lastpos = strrpos($file, "_");
}
if($lastpos === false) {
	$lastpos = strrpos($file, "/");
}
if($ext == "md")
{
	$baseFileName = basename($file);
	$f = fopen($file, 'r');
	$line = fgets($f);
	fclose($f);
	?>
	<td nowrap align="center"><?=$i?>. </td>
	<td nowrap align="left">
		<A HREF="md2html.php?filename=<?=$file?>">
		<?=$baseFileName?>
	</A>
	</td>
	<td  nowrap align="left">
	<A HREF="md2html.php?filename=<?=$file?>">
		<?=substr($file, $lastpos + 1, $dotpos - $lastpos -1)?>
	</A>
	</td>

<td><?=$line?></td>
<td nowrap align="center"><?=date ("Y-m-d H:i:s", filectime($file))?></td>

</tr>
<?
}//if end
else if($ext == "rst")
{
	$baseFileName = basename($file);
	?>
	<td nowrap align="center"><?=$i?>. </td>
	<td  nowrap align="left">
	<A HREF="rst2html.php?filename=<?=$file?>">
		<?=substr($file, $lastpos + 1, $dotpos - $lastpos -1)?>
</A>
</td>
<td nowrap align="left"><A HREF="rst2html.php?filename=<?=$file?>"><?=$baseFileName?></A></td>
<td><?=$line?></td>
<td nowrap align="center"><?=date ("Y-m-d H:i:s", filectime($file))?></td>

</tr>
<?
}//if end
}//foreach end
echo "</table></center>";
include_once(GetHomePath().'/include/footer.inc');
?>
