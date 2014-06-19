<?php

$list_info="";
$list_values=0;
//read up the file and convert it
$inp_h = @fopen("Opcodes.h", "r");
$inp_h2 = @fopen("Opcodes_cactus.h", "r");
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
//	if( $opcode >= 0x10000 || $opcode <= 0 )
//		continue;
	if( $opcode == 0x00000 )
		continue;
	$opcodes1[$opcode] = $name;
}
//now check file 2
while($inp_h2 && !feof($inp_h2)) 
{
	//take 1 line
   	$line = fgets($inp_h2);
	$line =	str_replace(array(" ","\t","\n","\r"),"",$line);
	$pieces = explode("=",$line);
	$name = $pieces[0];
	$pieces = explode(",",$pieces[1]);
	$opcode = $pieces[0];
//echo "'".$name."' = '".$opcode."',<br>";
	if( $name == "" || strlen( $name ) < 5 )
		continue;
	if( $opcode[2] == '1' )
		continue;
	if( strlen( $opcode ) < 7 )
		$opcode = "0x".substr($opcode,3);
	if( $opcode == 0x00000 )
		continue;
	$opcodes2[$opcode] = $name;
//echo "'".$name."' = '".$opcode."',<br>";
}
echo "<br><br><br><br><br>";
//check for name mismatches
echo "list of opcodes that only differ by name, Col 1 is our list, col2 is their list <br>";
foreach($opcodes1 as $opcode => $name)
	if( $opcodes1[ $opcode ] != $opcodes2[ $opcode ] && $opcodes2[ $opcode ] != "" )
		echo "name mismatch ${opcodes2[ $opcode ]} != ${opcodes1[ $opcode ]} with opcode $opcode<br>";
		
echo "<br><br><br><br><br>";		
//check for name mismatches
echo "list of opcodes that match by name but does not match as value, col1 is ours, col2 is their <br>";
foreach($opcodes1 as $opcode1 => $name1)
	foreach($opcodes2 as $opcode2 => $name2)
		if( $name1 == $name2 && $opcode1 != $opcode2 )
			echo "opcode mismatch : name $name1 => $opcode1 != $opcode2 <br>";
			
echo "<br><br><br><br><br>";
//check for missing opcodes
echo "list of opcodes that we do not have and they have <br>";
foreach($opcodes2 as $opcode => $name)
	if( $opcodes1[ $opcode ] == "" )
	{
		$name_fixed_len = $name;
		for($i=strlen($name_fixed_len);$i<47;$i++)
			$name_fixed_len .="&nbsp";
		echo "$name_fixed_len = $opcode,<br>";
	}
	
echo "<br><br><br><br><br>";
//check for missing opcodes
echo "Our file updated with new opcodes :<br>\n";
$our_file_content = str_replace( "0x000,","0x00000,",$our_file_content);
//$our_file_content = str_replace( "0x00,","0x00000,",$our_file_content);
//$our_file_content = str_replace( "0x0,","0x00000,",$our_file_content);
foreach($opcodes2 as $opcode => $name)
	if( $opcodes1[ $opcode ] == "" )
	{
		//try to find the name
		$opcode_pos = strpos( $our_file_content, $opcodes2[ $opcode ] );
		if( $opcode_pos == 0 )
		{
			$full_line_insertion = sprintf( "%-60s = %s,\n", $opcodes2[ $opcode ], $opcode );
			echo $full_line_insertion."<br>";
		}
		else
		{
			$hexformated_opcode = $opcode;
			$equ_pos = strpos( $our_file_content, '=', $opcode_pos );
			$coma_pos = strpos( $our_file_content, ',', $opcode_pos );
			$part1 = substr( $our_file_content, 0, $equ_pos );
			$part2 = substr( $our_file_content, $equ_pos, 30 );
			$part3 = substr( $our_file_content, $equ_pos+ 30, -1 );
			$part2 = str_replace( "0x00000,",$hexformated_opcode, $part2 );
			$our_file_content = $part1.$part2.$part3;
//echo "check ${opcodes2[ $opcode ]} !!<br>";
		}
	}
echo "\n";	
echo $our_file_content;	
?>