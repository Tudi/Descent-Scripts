<?php
//$line="27 67 4E 8E 9D 09 3A 32 76 24 E8 69 D0 93 A1 27 47 4E C6 9D 0D 3A 1A 74 34 EC C9 D9 93 B3 27 47 4E CC 9D 0D 3A 30";
//$line="27 66 4E CC 95 89 3B 12 76 24 EC E9 D8 D3 B1 A7 62 4E CE 9D 9D 3B 12 76 74 EC C9 D8 80";
//$line="27 46 4A 84 9D 09 3A 12 74 74 E8 69 D9 D3 B1 A7 46 4E 84 9D 09 3A 12 74 74 E8 E9 D9 93 A1 27 47 4E 84 9D 1D 3A 30";
$line="07 6A 0E D4 1D B8 3B 70";
$required_bits = Array( 5,6,6,5,5,5,6,5,5,6,6,6,6,6,5,5,5,6,5,6 );
$pieces = convert_line_to_elements($line);
$bin_str = convert_vect_to_bin( $pieces, 999999, 8 );
$bin_str2 = convert_vect_to_bin( $required_bits, 999999, 8 );
echo "Total number of ones in bytes : ".count_onces($bin_str[0])."<br>";
echo "Total number of ones required : ".count_onces($bin_str2[0])."<br>";
//print_r( $pieces );
//for( $i = 15; $i< 35; $i++)
//	print_test( $pieces, $i );

/*
$pieces2 = merge_byte_touint16( $pieces );
print_r( $pieces );
echo "<br>";
print_r( $pieces2 );
/**/

//$pieces = merge_byte_touint16( $pieces );

print_test( $pieces, 15 ,$required_bits );	
//print_test( $pieces, 16 ,$required_bits );	
//print_test( $pieces, 17 ,$required_bits);	
//print_test( $pieces, 30 ,$required_bits);	
//print_test( $pieces, 120 ,$required_bits);	

function print_test( $pieces, $i, $required_bits = "", $highlight_bits = 5 )
{
	echo "Testing len $i <br>";
	$bin_str = convert_vect_to_bin( $pieces, $i, 8 );
	foreach( $bin_str as $key => $val )
	{
		$ones = count_onces($val);
		if( $required_bits[$key] == $ones )
			echo "<B>$val = number of ones : $ones , line $key</B><br>";
		else
			echo "$val = number of ones : $ones , line $key <br>";
	}
}

function count_onces( $str )
{
	$ret = 0;
	$len = strlen( $str );
	for( $i=0;$i<$len;$i++ )
		if( $str[$i] == '1' )
			$ret++;
	return $ret;
}

function merge_byte_touint16( $vect )
{
	$total = count( $vect );
	for( $i=0;$i<$total;$i+=2)
//		$ret[ $i / 2 ] = $vect[$i]*256+$vect[$i+1];
		$ret[ $i / 2 ] = $vect[$i]+$vect[$i+1]*256;
	return $ret;
}

function convert_vect_to_bin($vect,$break, $bits)
{
	if( count($vect) == 0 )
		return "";
	$cur_break=0;
	foreach( $vect as $key => $val )
	{
		$tval = $val;
		for( $bits_written = 0; $bits_written < $bits; $bits_written++ )
		{
			$lastbit = ($tval >> 7 )& 1;
			$tval = $tval * 2;
//			if( (int)($cur_break/$break) != (int)(($cur_break+1)/$break) )
//				$ret[$cur_break/$break] .= "&nbsp";
			$ret[$cur_break/$break] = $ret[$cur_break/$break].$lastbit;
//			$ret[$cur_break/$break] = $lastbit.$ret[$cur_break/$break];
			$cur_break++;
		}
//		$ret[$cur_break/$break] .= "&nbsp";
	}
	return $ret;
}

function convert_line_to_elements($p_line)
{
	$ret="";
	$pieces=explode(" ",$p_line);
	if(is_array($pieces))
	foreach($pieces as $index => $val)
//		if($index>3) //use this line if line contains len+opcode
//			$ret[$index-4]=convert_from_hex_to_dec($val);
		{
			if($val[0]=="A")	$num=10;
			else if($val[0]=="B")	$num=11;
			else if($val[0]=="C")	$num=12;
			else if($val[0]=="D")	$num=13;
			else if($val[0]=="E")	$num=14;
			else if($val[0]=="F")	$num=15;
			else $num=$val[0];
			if($val[1]=="A")	$num1=10;
			else if($val[1]=="B")	$num1=11;
			else if($val[1]=="C")	$num1=12;
			else if($val[1]=="D")	$num1=13;
			else if($val[1]=="E")	$num1=14;
			else if($val[1]=="F")	$num1=15;
			else $num1=$val[1];
//			$ret[$index-4]=$num*16+$num1;
			$ret[$index]=$num*16+$num1;
		}
	return $ret;
}
?>