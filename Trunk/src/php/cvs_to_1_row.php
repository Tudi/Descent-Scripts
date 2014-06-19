<?php
$file=fopen("t.txt","r");
if( !$file )
	exit("Could not open input file. Exiting<br>");
$output_line=0;
$line = fgets($file);
while(!feof($file)) 
{
   	$line = fgets($file);
	$line = str_replace("\"","",$line);
	$line = str_replace(";","",$line);
	echo "$line";
	if( $line && strpos($line,",") == 0 )
		echo ",";
}
?>