<?php
$spells="CA 00 00 00 9F 02 00 00 A2 02 00 00 59 18 00 00 66 18 00 00 67 18 00 00 4D 19 00 00 93 54 00 00 94 54 00 00 A5 23 00 00 25 0D 00 00 EA 0B 00 00 4E 09 00 00 4E 19 00 00 CB 19 00 00 62 1C 00 00 63 1C 00 00 BB 1C 00 00 C2 20 00 00 AF 09 00 00 0B 56 00 00 1A 59 00 00 67 B3 00 00 33 07 00 00 FD EF 00 00 B9 CD 00 00 21 22 00 00 75 23 00 00 5E 2A 00 00 CB 0C 00 00 C6 48 00 00 FE 1E 00 00 CC 0C 00 00 CD 0C 00 00 CE 0C 00 00 F8 1E 00 00 F9 1E 00 00 58 2A 00 00 59 2A 00 00 C5 48 00 00 67 50 00 00 66 50 00 00 69 50 00 00 68 E6 00 00 CB 00 00 00 C4 00 00 00 0A 02 00 00 CC 00 00 00 51 00 00 00 4F B3 00 00 37 0C 00 00 76 23 00 00 4E B3 00 00 8A BC 00 00 02 C1 00 00 A5 B1 00 00 11 EA 00 00 0F F0 00 00 A8 C1 00 00 96 B1 00 00 B5 B9 00 00 E7 E9 00 00 C8 00 00 00 9C 02 00 00 6F 82 00 00 EE 02 00 00 C9 00 00 00 C5 00 00 00 01 00 8A BC 00 00 2F 00 00 00 00 00 E7 03 00 00";

$parts=explode(" ",$spells);
$count=count($parts);
for($index=56;$index<65;$index++)
	for($i=0;$i<$count/4;$i+=4)
	{
		$number = convert_from_hex_to_dec($parts[$i]) + convert_from_hex_to_dec($parts[$i+1])*256;
		echo "replace into playercreateinfo_spells values ($index,$number); <br>";
	}


function convert_from_hex_to_dec($str)
{
	$ret=0;
	for($i=0;$i<strlen($str);$i++)
	{
		if($str[$i]=="0")	$num=0;
		else if($str[$i]=="1")	$num=1;
		else if($str[$i]=="2")	$num=2;
		else if($str[$i]=="3")	$num=3;
		else if($str[$i]=="4")	$num=4;
		else if($str[$i]=="5")	$num=5;
		else if($str[$i]=="6")	$num=6;
		else if($str[$i]=="7")	$num=7;
		else if($str[$i]=="8")	$num=8;
		else if($str[$i]=="9")	$num=9;
		else if($str[$i]=="A")	$num=10;
		else if($str[$i]=="B")	$num=11;
		else if($str[$i]=="C")	$num=12;
		else if($str[$i]=="D")	$num=13;
		else if($str[$i]=="E")	$num=14;
		else if($str[$i]=="F")	$num=15;
		$ret=$ret*16 + $num;
//echo $str[$i]."=$num -> $ret <br>";
	}
	return $ret;
}