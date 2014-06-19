<?php
$y="";
$x="";
$m=261;
$n=730;
$A=563;
$B=947;
$N=$A*$B;

//research result : right now it is worst then guessing A and B directly since it generates 2 times more new seends because of mirrored results instead 10x when we know A < B

echo "A = $A, B = $B, N = A*B = $N, n = $n, m = $m <br>";
//echo "test 2 digit mul = ".multiply_N_digits($A,$B,2)." <br>";
//echo "test 2 digit sub = ".sub_N_digits($n,$m,2)." <br>";
//echo "test 2 digit add = ".add_N_digits(90,50,2)." <br>";

$digitc=3;
$generate_number_digit_count=pow(10,$digitc)-1;

$digitcount = $digitc;
//for($digit=0;$digit<$digitc;$digit++)
{
	$seeds_for_next_level=0;
	echo "=====================================================<br>";
//	$digitcount = $digit;
	for($xc=0;$xc<=$generate_number_digit_count;$xc++)
	{
		$txs = "".$xc;
		$xn = multiply_N_digits($txs,$n,$digitcount);		
		$xn4 = multiply_N_digits($xn,4,$digitcount);		
		$m4 = multiply_N_digits(4,$m,$digitcount);			
		$x2 = multiply_N_digits($txs,$txs,$digitcount);		
		$x24xn = add_N_digits($xn4,$x2,$digitcount);		
		$tty = sub_N_digits_force_pos($x24xn,$m4,$digitcount);	
debug_x_y_step( $txs, $tys, "x*n", $xn );
debug_x_y_step( $txs, $tys, "x*n*4", $xn4 );
debug_x_y_step( $txs, $tys, "m*4", $m4 );
debug_x_y_step( $txs, $tys, "x*x", $x2 );
debug_x_y_step( $txs, $tys, "x*x+4*x*n", $x24xn );
debug_x_y_step( $txs, $tys, "x*x+4*x*n-4*m", $tty );
		for($yc=0;$yc<=$generate_number_digit_count;$yc++)
		{
			$tys = "".$yc;
			$y2 = multiply_N_digits($tys,$tys,$digitcount);
debug_x_y_step( $txs, $tys, "tty", $tty );
debug_x_y_step( $txs, $tys, "y*y", $y2 );
			if( $y2 == $tty )
			{
				//check 1 : k (note that 1 digit is lost due to the division !
				$k_2 = sub_N_digits_force_pos( $tys, $txs, $digitcount );
				$k = div_2_N_digits( $k_2, $digitcount );
				//this check should be always true
				$k2 = multiply_N_digits($k,$k,$digitcount-1);
				$k2m = add_N_digits($k2,$m,$digitcount-1);
				$xk = multiply_N_digits($txs,$k,$digitcount-1);
				$k2mxk = add_N_digits($k2m,$xk,$digitcount-1);
				$t1 = chop_to_length( $k2mxk, $digitcount-1);
				$t2 = chop_to_length( $xn, $digitcount-1);
				if( $t1 != $t2 )
				{
//					echo "$seeds_for_next_level) FAILED K TEST : digit match for x = $txs($xc) and y = $tys($yc) <br>";
					continue;
				}
				$tA = sub_N_digits_force_pos( $n, $k, $digitcount - 1 );
				$kn = add_N_digits($k,$n,$digitcount-1);
				$knx = add_N_digits($kn,$txs,$digitcount-1);
				$tB = $knx;
				$AB = multiply_N_digits($tA,$tB,$digitcount-1);
				$t1 = chop_to_length( $AB, $digitcount-1);
				$t2 = chop_to_length( $N, $digitcount-1);
				if( $t1 != $t2 )
				{
//					echo "$seeds_for_next_level) FAILED N TEST : digit match for x=$txs,y=$tys,n=$n,k=$k,A=$tA,k+n=$kn,k+n+x=$knx,B=$tB,AB=$AB <br>";
					continue;
				}
				$seeds_for_next_level++;
				echo "$seeds_for_next_level) digit match for x = $txs($xc) and y = $tys($yc) <br>";
//echo "........y^2 = x^2 - 4(m-xn) => $y2 = $x2-4($m-$txs*$n) = $x2-4($m-$xn) = $x2-4*$mxn = $x2 - $mxn4 = $tty <br>";
echo "........y^2 = x^2-4m+4xn => $y2 = $x2-$m4-4*$txs*$n) = $x2-$m4+$xn4 = $x24xn-$m4 = $tty => A = $tA B = $tB N = $t1 <br>";
//				break;
			}
//if( $yc == 3 ) die();
		}
	}
}/**/

function debug_x_y_step($x,$y,$name,$val)
{
//	if( chop_to_length($x,2) == "50" || chop_to_length($y,2) == "84" )
//		echo "($x),($y) name = $name , val = $val <br> ";
//	if( chop_to_length($x,3) == "50" && ( chop_to_length($y,3) == "384" || chop_to_length($y,3) == "999" ) )
//		echo "($x),($y) name = $name , val = $val <br> ";
}

function vect_to_str($vect)
{
	$res="";
	foreach($vect as $key => $value )
		$res .= $value;
	return $res;
}

function chop_to_length( $a, $digitc )
{
	if( strlen($a) > $digitc )
		$A = substr("".$a,(strlen($a)-$digitc));
	else
		$A = "".$a;
	return $A;
}

function div_2_N_digits( $a, $digitc )
{
	$A = chop_to_length($a,$digitc);
	$res = (int)$A / 2;
//echo "a = $a, b = $b, A = $A, B = $B, res = $res <br>";
	$res = chop_to_length($res,$digitc);
//echo "a = $a, b = $b, A = $A, B = $B, res = $res <br>";
	return $res;
}

function multiply_N_digits( $a, $b, $digitc )
{
	$A = chop_to_length($a,$digitc);
	$B = chop_to_length($b,$digitc);
	$res = (int)$A * (int)$B;
//echo "a = $a, b = $b, A = $A, B = $B, res = $res <br>";
	$res = chop_to_length($res,$digitc);
//echo "a = $a, b = $b, A = $A, B = $B, res = $res <br>";
	return $res;
}

function sub_N_digits( $a, $b, $digitc )
{
	$A = chop_to_length($a,$digitc);
	$B = chop_to_length($b,$digitc);
//echo "a = $a, b = $b, A = $A, B = $B, res = $res <br>";
	$res = (int)$A - (int)$B;
//echo "a = $a, b = $b, A = $A, B = $B, res = $res <br>";
	$res = chop_to_length($res,$digitc);
	return $res;
}

function add_N_digits( $a, $b, $digitc )
{
	$A = chop_to_length($a,$digitc);
	$B = chop_to_length($b,$digitc);
	$res = (int)$A + (int)$B;
	$res = chop_to_length($res,$digitc);
	return $res;
}

//since we do not know the exact size of the number we presume that the number A is always larger then B
function sub_N_digits_force_pos( $a, $b, $digitc )
{
	$A = chop_to_length($a,$digitc);
	$B = chop_to_length($b,$digitc);
	$res = (int)$A - (int)$B;
	if( $res < 0 )
		$res = pow(10,strlen($a)+1) + $res;
	$res = chop_to_length($res,$digitc);
	return $res;
}
?>