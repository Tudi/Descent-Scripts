<?php
$counts="";
$limts="";
for($i=0;$i<10;$i++)
	$limits[$i]=pow(10,$i);
for($i=0;$i<100000;$i++)
{
	$num=$i*$i;
	for($j=0;$j<10;$j++)
		if($limits[$j]<=$num && $limits[$j+1]>=$num)
		{
			$counts[$j]++;
			break;
		}
}
for($i=0;$i<10;$i++)
	echo "$i = {$counts[$i]} = ".pow(10,$i)."<br>";
?>