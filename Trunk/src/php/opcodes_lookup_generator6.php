<?php

$list_info="";
$list_values=0;
//read up the file and convert it
$inp_h = @fopen("Opcodes.h", "r");
$out = "";

$our_file_content = "";
while($inp_h && !feof($inp_h)) 
{
	//take 1 line
   	$line = fgets($inp_h);
	$our_file_content .= $line;
	$line =	str_replace(array(" ","\t","\n","\r"),"",$line);
	$pieces = explode("=",$line);
	$name = $pieces[0];
	$pieces = explode(",",$pieces[1]);
	$opcode = $pieces[0];
	if( $name == "" || strlen( $name ) < 5 )
		continue;
	if( $opcode == "0x00000" )
		continue;
	if( strpos( $line,'=' ) == 0 )
		continue;
	if( $opcodes1[$opcode] )
		$multi[$opcode] = 1;
	$opcodes1[$opcode] .= $name." ";
//echo "'".$name."' = '".$opcode_dec."',<br>";	
}
echo "list of opcodes that share same value but have different name <br>";
foreach($opcodes1 as $opcode => $name)
	if( $multi[$opcode] )
		echo " $opcode = $name<br>";
		
?>