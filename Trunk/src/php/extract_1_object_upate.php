<?php
$file=fopen("a9_human_form.txt","r");
if( !$file )
	exit("Could not open input file. Exiting<br>");
$output_line=0;
$look_for_guid="D3 B7 16 00 00 00 00 00";
while(!feof($file)) 
{
   	$line = fgets($file);
	if( strpos("#".$line,$look_for_guid) )
		$output_line=1;
	else if ( strpos("#".$line,"started") )
		$output_line=0;
	if( $output_line==1 )
		echo $line."<br>";
}
?>