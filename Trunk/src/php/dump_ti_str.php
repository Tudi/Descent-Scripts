<?php
$dump="4D 69 6C 65 6E 74 65";
$pieces=explode(" ",$dump);
foreach($pieces as $key => $value )
{
//	echo "number : $value <br>";
	$val = convert_from_hex_to_dec( $value );
	echo chr($val)."";
}

function convert_from_hex_to_dec($str)
{
	$ret=0;
	for($i=0;$i<strlen($str);$i++)
	{
		if($str[$i]<=9)	$num=$str[$i];
		else if($str[$i]=="A")	$num=10;
		else if($str[$i]=="B")	$num=11;
		else if($str[$i]=="C")	$num=12;
		else if($str[$i]=="D")	$num=13;
		else if($str[$i]=="E")	$num=14;
		else if($str[$i]=="F")	$num=15;
		$ret=$ret*16 + $num;
	}
	return $ret;
}

?>