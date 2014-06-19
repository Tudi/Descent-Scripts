<?php

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

foreach($_REQUEST as $key => $value)
	$$key=mysql_escape_string($value);

$i=0;
$where1[$i++]="description like '% high amount%'";
$where1[$i++]="description like '% large amount%'";
$where1[$i++]="description like '% moderate%'";
$where1[$i++]="description like '% additional%'";
$where1[$i++]="description like '% generat%'";
$where1[$j++]="description like '% increases%'";

$j=0;
$where2[$j++]="description like '%reduc%'";
$where2[$j++]="description like '% no t%'";
$where2[$j++]="description like '% low%'";

for($k=0;$k<$i;$k++)
	for($l=0;$l<$j;$l++)
	{
		$query="select id,name,description,EffectBasePoints_1,EffectBasePoints_2,EffectBasePoints_3 from dbc_spell where description like '% threat%' ";
		$query .= " and ".$where1[$k];
		$query .= " and ".$where2[$l];
		$get_res1=mysql_query($query,$dbi) or die(" Q20061123150142 ".mysql_error($dbi));
		if( $get_res1 )
		{
			echo "#############################################################################################################################<br>";
			echo "<br>$query <br>";
		}
		while ( list($id,$name,$description)=mysql_fetch_row($get_res1) )
			echo "$id -> $name -> $description <br>";
	}

if( $dbi)
	mysql_close($dbi);
?>