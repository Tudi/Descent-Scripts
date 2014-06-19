<?php
include("include/header.inc.php");
include("include/functions.php");
?>
<br>
Step : 1 <br>
Step Name : Sampling packets<br>
<?php
//since format can change from 1 packet to another we decide that first
$processing_file=get_next_file_to_process();
if($processing_file=="")
{
	echo "no more files <br>";
	die();
}
$inp_h = fopen($processing_file, "r");
if($inp_h)
{
	$l1 = fgets($inp_h);
	$l2 = fgets($inp_h);
	$l3 = fgets($inp_h);
}
fclose($inp_h);
if(	strpos($l1,"------------")
	|| strpos($l2,"------------")
	|| strpos($l3,"------------")
	)
	$next_file_name="s1_break_apart_b.php"; //normal dumps from most of the people
else

	$next_file_name="s1_break_apart_sh.php"; //short version of dump used to reduce file size
proceedtonextstep();
?>