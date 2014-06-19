<?php

$list_info="";
$list_values=0;
//read up the file and convert it
$inp_h = @fopen("Opcodes.h", "r");
$inp_h2 = @fopen("Opcodes2.h", "r");
$out = "";
while($inp_h && !feof($inp_h)) 
{
	//take 1 line
   	$line = fgets($inp_h);
	$line =	str_replace(array(" ","\t","\n","\r",","),"",$line);
	$pieces = explode("=",$line);
	if( strlen( $pieces[0] ) < 5 )
		continue;
	$out .= "".$pieces[0]." = ".$pieces[1].",<br>";
}
$out .= "<br><br><br><br><br>";
//now check file 2
while($inp_h2 && !feof($inp_h2)) 
{
	//take 1 line
   	$line = fgets($inp_h2);
	$line =	str_replace(array(" ","\t","\n","\r"),"",$line);
	$pieces = explode("=",$line);
	//check if we already have this opcode
	if( strpos( "#".$out,$pieces[0] ) )
		continue;
	$out .= "".$pieces[0]." = ".$pieces[1].",<br>";
}

//check for double opcodes
while($inp_h && !feof($inp_h)) 
{
	//take 1 line
   	$line = fgets($inp_h);
	$line =	str_replace(array(" ","\t","\n","\r",","),"",$line);
	$pieces = explode("=",$line);
	if( strlen( $pieces[0] ) < 5 )
		continue;
	$pieces2 = explode( $pieces[1],$line);
	//check if we already have this opcode
	if( count( $pieces2 ) <= 1 )
		continue;
	echo "found duplicate opcode ".$pieces[1]."<br>";
}
while($inp_h2 && !feof($inp_h2)) 
{
	//take 1 line
   	$line = fgets($inp_h2);
	$line =	str_replace(array(" ","\t","\n","\r"),"",$line);
	$pieces = explode("=",$line);
	$pieces2 = explode( $pieces[1],$line);
	//check if we already have this opcode
	if( count( $pieces2 ) <= 1 )
		continue;
	echo "found duplicate opcode ".$pieces[1]."<br>";
}
echo $out;
?>