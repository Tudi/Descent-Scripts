<?php
#$packet="57 6F 57 27 73 20 34 74 68 20 41 6E 6E 69 76 65 72 73 61 72 79 21";
$packet="7C 63 66 66 46 46 43 31 43 42 7C 54 3A 39 39 39 39 3A 33 30 30 3A 33 30 30 30 7C 74 7C 54 3A 33 30 30 7C 74 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 7C 63 66 66 30 30 63 63 66 66 5B 53 74 61 66 66 5D 7C 63 66 66 30 30 66 66 30 30 5B 42 61 74 74 6C 65 6D 5D 3A 20 7C 63 66 66 66 66 63 63 30 30 74 65 73 74 7C 72 00";
$parts=explode(" ",$packet);
foreach($parts as $key=> $value)
	echo chr( convert_from_hex_to_dec( $value) );

echo "<br>";
$packet="7C 63 66 66 66 66 66 66 30 30 7C 54 3A 39 39 39 39 3A 33 30 30 3A 33 30 30 30 7C 74 7C 54 3A 33 30 30 7C 74 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 42 61 74 74 6C 65 6D 20 72 6F 6C 6C 73 20 39 37 20 28 31 2D 31 30 30 29 20 7C 72 00";
$parts=explode(" ",$packet);
foreach($parts as $key=> $value)
	echo chr( convert_from_hex_to_dec( $value) );

echo "<br>";
$packet="20 7C 63 66 66 61 33 33 35 65 65 7C 48 69 74 65 6D 3A 32 32 34 39 32 3A 30 3A 30 3A 30 3A 30 3A 30 3A 30 3A 30 3A 38 30 7C 68 5B 44 72 65 61 6D 77 61 6C 6B 65 72 20 42 6F 6F 74 73 5D 7C 68 7C 72 00";
$parts=explode(" ",$packet);
foreach($parts as $key=> $value)
	echo chr( convert_from_hex_to_dec( $value) );

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
?>