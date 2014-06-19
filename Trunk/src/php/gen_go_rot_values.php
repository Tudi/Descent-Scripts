<?php
/*
           8      7      6      5      4      3      2          
k(x) := a·x  + b·x  + c·x  + d·x  + e·x  + g·x  + h·x  + j·x + l

k(2.7838)=1031841
k(2.60926)=1011652
k(2.43473)=983765
k(2.37364)=972222
k(1.78896)=817768
k(1.56207)=738213
k(1.48353)=708407
k(1.25664)=616337
k(1.24791)=612630
k(0.863937)=438996
k(0.67195)=345705
k(0.575957)=297811
k(0.0698136)=36595
k(0.0261791)=13725

*/

$step = 3.14 / 50.0;
$values_per_line=10;
$value_len = 8;
$values_counter=0;
/*
for($angle=0;$angle<=3.14;$angle+=$step)
{
	$as_int = (int)(k($angle));
	$val=$as_int."";
	$extend_count = $value_len - strlen($val);
	for($i=0;$i<$extend_count;$i++)
		$val.="&nbsp;";
	$val.=",";
	echo $val;
	$values_counter++;
	if($values_counter % $values_per_line == 0 )
		echo "<br>";
}/**/

for($angle=-3.11539;$angle<-0.445;$angle+=0.004)
	echo "$angle = ".test_rot($angle)." = ".test_rot2($angle)."<br>";

function k($x)
{
	$a = 10.077151253302390520638810007626261868064075208595;
	$b = -100.65894545764895182328772711568069460446621635099;
	$c = 389.49021778511806431414270000713309406781732059605;
	$d = -510.08818898649912807527753746096743851864180624405;
	$e = 853.51810055236405239290285476340731657744151368467;
	$g = -2.2333773115749590463552733345926752027400965547842*10000;
	$h = 126.57324161330901670780915165728625979803073795053;
	$j = 5.2427894215136712708688926762677771718164532407794*100000;
	$l = 0.16271076404252928720017001716006309995536402803364;
	$kx = $a*pow($x,8)+$b*pow($x,7)+$c*pow($x,6)+$d*pow($x,5)+$e*pow($x,4)+$g*pow($x,3)+$h*pow($x,2)+$j*pow($x,1)+$l*pow($x,0);
	return $kx;
}

function test_rot($ori)
{
	if($ori==0)
		return 0;
	$values_0_PI = array(
0       ,32919   ,65807   ,98630   ,131355  ,163951  ,196385  ,228626  ,260641  ,292399  ,
323868  ,355019  ,385820  ,416240  ,446250  ,475820  ,504921  ,533524  ,561601  ,589124  ,
616067  ,642402  ,668103  ,693146  ,717506  ,741159  ,764081  ,786249  ,807643  ,828241  ,
848023  ,866968  ,885059  ,902278  ,918606  ,934028  ,948529  ,962094  ,974710  ,986364  ,
997045  ,1006743 ,1015450 ,1023158 ,1029861 ,1035556 ,1040241 ,1043916 ,1046584 ,1048249, 1048249 );
	//normalize it : 0 - 6.28
	if( $ori > 6.28 )
		$ori = $ori - ((int)($ori/6.28))*6.28;
	if( $ori < 0.0 )
		$ori = $ori - ((int)($ori/6.28))*6.28;
	//convert it to have system : [-3.14,4.14]
	if( $ori > 3.14 )
		$new_ori = $ori - 6.14;
	else 
		$new_ori = $ori;
debug_echo("new ori = $new_ori<br>");
	//get the "int" special value for blizz
	if( $new_ori >= 0 )
	{
		$pi = 3.14159;
		$mat_ind = 50.0 / $pi * $new_ori;
debug_echo("mat ind = $mat_ind <br>");
		$mat_ind = (int)($mat_ind);
debug_echo("mat ind = $mat_ind <br>");
		$lower_val = $values_0_PI[$mat_ind];
debug_echo("lower_val= $lower_val <br>");
		$upper_val = $values_0_PI[$mat_ind+1];
debug_echo("upper_val= $upper_val <br>");
		//interpolate for precision
		$dist_1 = abs(($mat_ind + 0)* $pi / 50.0 - 50.0 / $pi * $new_ori);
debug_echo("dist_1= $dist_1 <br>");
		$dist_2 = abs(($mat_ind + 1)* $pi / 50.0 - 50.0 / $pi * $new_ori);
debug_echo("dist_2= $dist_2 <br>");
		$dist_fact_1 = $dist_1 / ($dist_1 + $dist_2);
debug_echo("dist_fact_1= $dist_fact_1 <br>");
		$dist_fact_2 =$dist_2 / ($dist_1 + $dist_2);
debug_echo("dist_fact_2= $dist_fact_2 <br>");
		$blizz_val = (int)(( $lower_val * $dist_fact_1 + $upper_val * $dist_fact_2));
	}
	return $blizz_val;
}

function test_rot2($ori)
{
//    static double const atan_pow = atan(pow(2.0f, -20.0f));
	$atanpow=0.0000009536743164;
	$pi = 3.14159;
//	float f_rot1 = sin(ang / 2.0f); // bullcrap, only works half way !
	if( $ori < 0 )
	{
//		$frot1 = sin( $pi / 2 );
		$frot1 = 1;
//echo $frot1."<br>";
		$new_ori = ($pi + $ori);
		$frot2 = sin( $new_ori / 2.3648 );
//		$frot3 = sin( -$ori / 4 ) / 82;
		$frot3 = 1 + $ori / $pi;
//echo $frot2."<br>";
		$frot = $frot1+$frot2*$frot3;
//		$frot = 1 - $ori - $ori/2 - $ori/4;
echo " $frot1 + $frot2 - $frot3 <br>";
	 }
	else 
		$frot = sin( $ori / 2 );
//	int64 i_rot1 = (int64)( f_rot1 / atan_pow );
	$frot2 = (int)($frot / $atanpow);
//	m_rotation |= (i_rot1 << 43 >> 43) & 0x00000000001FFFFF;
	return $frot2;
}

function debug_echo($what)
{
//	echo $what;
}