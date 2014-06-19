<?php
foreach($_REQUEST as $request_name=>$request_value)
$$request_name=mysql_escape_string($request_value);
?>
Please use only 1 version of input
<table>
	<form name="get_data" action="number_formater.php" method="post">
	<tr>
	  <td width="150" height="30" align="right">Number from log:&nbsp;</td>
	  <td width="240"><input type="text" name="as_log" style="width:180" value="<?=$as_log?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">hex:&nbsp;</td>
	  <td width="240"><input type="text" name="as_hex" style="width:180" value="<?=$as_hex?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">uint32:&nbsp;</td>
	  <td width="240"><input type="text" name="as_uint" style="width:180" value="<?=$as_uint?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">hex to text:&nbsp;</td>
	  <td width="240"><input type="text" name="text" style="width:180" value="<?=$text?>"></td>
	</tr>
	<tr><td><input type="submit" value="convert"></td></tr>
	</form>
</table>
<?php
//echo "'$as_log'<br>";
$number=0;
$asci_text ='';
if($as_log)
{
	//bytes are in inverted order and in hex
	$parts="";
	$parts=explode(" ",$as_log);
	$part_no=count($parts);
//echo "'$part_no'<br>";
	$normal_order_hex="";
	for($i=0;$i<$part_no;$i++)
		$normal_order_hex=$parts[$i].$normal_order_hex;
//echo "'$normal_order_hex'<br>";
	$number_as_int=convert_from_hex_to_dec($normal_order_hex);
}
else if($as_hex)
{
	$as_hex=str_replace(" ","",$as_hex);
	$number_as_int=convert_from_hex_to_dec($as_hex);
}
else if($as_uint)
{
	$number_as_int=$as_uint;
}
else if($text)
{
	$parts="";
	$parts=explode(" ",$text);
	$part_no=count($parts);
//echo "'$part_no'<br>";
	for($i=0;$i<$part_no;$i++)
	{
		$number_as_int=convert_from_hex_to_dec($parts[$i]);
		if( $number_as_int )
			$asci_text .=chr($number_as_int);
	}
}
echo "the number as int : $number_as_int <br>";
echo "the number as signed int : ".(int)$number_as_int." <br>";
$as_float = convert_to_float($number_as_int);
echo "the number as float : $as_float <br>";
echo "the hex as text : $asci_text <br>";
$as_bin_mask = convert_to_binMask($number_as_int);
echo "the hex bin mask : $as_bin_mask <br>";

//////////////////////////////////////////////////////////////////////////////////////////////////////////
function convert_to_binMask($value)
{
	for( $i=0;$i<32;$i++ )
	{
		$str_out .= ( (int)$value & 1 );
		$value = $value / 2;
	}
	return $str_out;
}

function convert_to_float($value)
{
	$val2=$value;
	settype($val2, "integer");
	$packed = pack("L*", $val2);
	list(,$unpacked) = unpack("f*", $packed);
	return $unpacked;
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
?>