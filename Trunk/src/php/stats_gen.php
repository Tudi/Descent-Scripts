<?php
//Generate lineary increasing stats list
$float_precission=100000000000;
$first_value=0.01365125591*$float_precission;
$increase_per_step = -0.00008*$float_precission;
$random_per_step = -0.000001*$float_precission;
$steps_to_generate = 137 - 70;
$new_line_after_x_values = 1;
$value_formating_length = 12;
$value_formating_filler = ' ';
$prev_value=$first_value;
$values_written_on_line=0;
for($i=0;$i<$steps_to_generate;$i++)
{
	$cur_value_min = $prev_value + $increase_per_step;
	$cur_value_max = $cur_value_min + $random_per_step*$float_precission;
	if( abs($cur_value_max) > abs( $first_value + $increase_per_step*$i ) )
		$cur_value_max = $first_value + $increase_per_step*$i;
//	$randomization = $cur_value_max - $cur_value_min;
	$cur_value = rand( $cur_value_min, $cur_value_max);
	$prev_value = $cur_value;
	if( $float_precission > 1 )
		$string_value = ( $cur_value / $float_precission ) ."f";
	else
		$string_value = $cur_value;
	for($j=0;$j<$value_formating_length - strlen($cur_value);$j++)
		$string_value.=$value_formating_filler;
	echo "$string_value,";
	$values_written_on_line++;
	if( $values_written_on_line == $new_line_after_x_values )
	{
		echo "<br>";
		$values_written_on_line = 0;
	}
}
?>