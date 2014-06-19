<?php

$list_info="";
$list_values=0;
//read up the file and convert it
$inp_h = @fopen("Opcodes.h", "r");
while($inp_h && !feof($inp_h)) 
{
	//take 1 line
   	$line = fgets($inp_h);
	$line =	str_replace(array(" ","\t","\n","\r"),"",$line);
	$pieces = explode("=",$line);
	if( strlen( $pieces[0] ) > 5 && strlen( $pieces[1] ) > 2 && strpos( "#".$pieces[0], "//" ) == 0 )
	{
		//echo "{".$pieces[0].",\"".$pieces[0]."\"},<br>";
		$new_string = sprintf("% -60s = 0x%04X,",$pieces[0],$pieces[1]);
		echo $new_string."\n";
	}
}
?>