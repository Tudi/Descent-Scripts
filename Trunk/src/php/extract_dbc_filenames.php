<?php
$f = fopen("Wow_433.exe","rb");
$content = fread( $f, 10*1024*1024 );
//$content = fread( $f, 1024 );
$content .= "";
$pos = 1;
//echo "finished reading $pos <br> '".$content[$pos]."'<br>";
$count = 0;
$names = "";
while( $pos != 0 )
{
	$pos = strpos( $content, "DBFilesClient", $pos );
	$pos2 = $pos;
	$name = "";
	for( $i=$pos+14;$i<$pos+120+14; $i++)
	{
		$name .= $content[$i];
		if( ord($content[$i]) == 0 )		
			break;
		$pos2++;
	}
	$pos = $pos2;
	if( strlen( $name ) > 3 )
	{
		$names[$count] = $name;
		$count++;
	}
//	if( $count >= 10 )
//		break;
}
fclose( $f );

//for( $i=$count;$i>=0;$i--)
//	echo $names[$i]."<br>";

$copy_count = 311;
$base_from = "d:\\Munka\Myserver\\wowbeez_core\\MPQ editors\\divide content\\DBFilesClient t\\";
//$base_from = "d:/Munka\Myserver/wowbeez_core/MPQ editors/divide content/DBFilesClient t/";
$base_to = "d:\\Munka\Myserver\\wowbeez_core\\MPQ editors\\divide content\\DBFilesClient\\";
//$base_to = "d:/Munka\Myserver/wowbeez_core/MPQ editors/divide content/DBFilesClient/";
if( $copy_count > $count )
	$copy_count = $count;
for( $i=$count-1;$i>=$count-$copy_count;$i--)
{
//	echo $base_from.$names[$i]."<br>";
//	echo $base_to.$names[$i]."<br>";
//	if( !copy( $base_from.$names[$i], $base_to ) )		echo "Issue = ".$base_from.$names[$i]." -> ".$base_to."<br>";
	echo "copy \"".$base_from.$names[$i]."\" \"".$base_to."\"<br>";
}
?>