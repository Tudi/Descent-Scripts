<?php
$start_values=array(0.000000 , 5.000000 , 5.000000 , 5.000000 , 5.000000 , 5.000000 , 5.000000 , 5.000000 , 5.000000 , 5.000000 , 0.000000 , 5.000000 );
$end_values = array(0.000000 , 73.529412 , 52.083333 , 75.187970 , 41.493776 , 52.083333 , 73.529412 , 52.083333 , 51.282051 , 52.083333 , 0.000000 , 47.87 );
$start_values_for_level=1;
$end_values_for_level=80;

$gen_for_max_level = 137;
for($level=0;$level<$gen_for_max_level;$level++)
{
	echo "{";
	for($class=0;$class<=11;$class++)
	{
		$step = ( $end_values[$class] - $start_values[$class] ) / ($end_values_for_level-$start_values_for_level);
		$val_now = $level * $step + $start_values[$class];
		echo FixedLengthFloat( $val_now, 8 );
		if( $class==11 )
			echo "},<br>";
		else
			echo", ";
	}
}

function FixedLengthFloat($val,$len)
{
	//sorry we cannot make it longer
	if( strpos("@".$val,".") == 0 && strlen($val) > $len + 2 )
		return $val;
	//this is not float but int
	if( strpos("@".$val,".") == 0 )
		$val .= ".";
	//to much precision we pinch it off
	if( strlen($val) > $len - 1 )
		$val = substr( $val,0, $len - 1 );
	//too short ? add 0 at the end
	$len_now = strlen($val);
	for(;$len_now<$len-1;$len_now++)
		$val .= "0";
	//c++ style float :P
	$val .= "f";
	return $val;
}
?>