<?php
$inp_h = @fopen("t3.txt", "r");
$out=0;
while($inp_h && !feof($inp_h)) 
{
	$line=fgets($inp_h);
	if(strpos("#".$line,"FPS = "))
	{
		$pieces = explode(" = ",$line);
		echo $pieces[1]."<br>";
	}/**/
	if( strpos("#".$line,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!") 
		|| strpos("#".$line,"###########################")
//		|| strpos("#".$line,"perforce\mainline")
		)
		echo "<br>";
/*	if( strpos("#".$line,"SNR Y(dB)"))
	{
		$pieces = explode(" : ",$line);
		$out++;
		if($out % 2 == 0)
			echo $pieces[1]."<br>";
	}/**/
/*	if( strpos("#".$line,"Reencode cost"))
	{
		$pieces = explode(" : ",$line);
		echo $pieces[1]."<br>";
	}/**/
}
fclose($inp_h);
?>