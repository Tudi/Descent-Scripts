<?php
$file=fopen("quiz.txt","r");
if( !$file )
	exit("Could not open input file. Exiting<br>");
while(!feof($file)) 
{
   	$line = fgets($file);
	$line = str_replace("\",\"","#",$line);
	$line = str_replace(array("\"","{","}",";","\n","\r"),"",$line);
	$parts = explode(" = ",$line);
	$question = $parts[1];

   	$line = fgets($file);
	$line = str_replace("\",\"","#",$line);
	$line = str_replace(array("\"","{","}",";"),"",$line);
	$parts = explode(" = ",$line);
	$answers = $parts[1];

   	$line = fgets($file);
	$line = str_replace("\",\"","#",$line);
	$line = str_replace(array("\"","{","}",";"),"",$line);
	$parts = explode(" = ",$line);
	$category = $parts[1];

   	$line = fgets($file);
	$line = str_replace("\",\"","#",$line);
	$line = str_replace(array("\"","{","}",";"),"",$line);
	$parts = explode(" = ",$line);
	$points = $parts[1];

   	$line = fgets($file);
	$line = str_replace("\",\"","#",$line);
	$line = str_replace(array("\"","{","}",";","\n","\r"),"",$line);
	$parts = explode(" = ",$line);
	$hints = $parts[1];

   	$empty = fgets($file);

	echo "$question<br>";
	if( $hints=="" )
		echo "#<br>";
	else
		echo "$hints<br>";
	echo "$answers<br>";
	echo "$points<br>";
	echo "<br>";
}
?>