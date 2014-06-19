<?php
include("include/header.inc.php");
include("include/functions.php");
set_time_limit ( 5*60*1000 );
?>
<?php
$next_file_name="s1_break_apart.php";
$processing_file=get_next_file_to_process();
//echo $processing_file;
$inp_h = fopen($processing_file, "r");
$outf_comp = fopen("step3/compressed_line_timestamps.txt", "w");
$outf=0;
$linenr=0;
while($inp_h && !feof($inp_h)) 
{
   	$line = fgets($inp_h);
	if($linenr==0 && strpos($line,"version for parsing")) //may happen only at first line of the dump file
	   	$line = fgets($inp_h);
	$linenr++;
	if(strlen($line)>2)
	{
		//read opcode as hex number
		//{SERVER} Packet: (0x0096) SMSG_MESSAGECHAT PacketSize = 104
		$len = strlen($line);
		$from1 = strpos($line,"(")+1;
		$hex_opcode=substr($line,$from1,6);
		$from1 = $from1+8;
		$to1 = strpos($line," Pac",$from1);
		$str_opcode=substr($line,$from1,$to1 - $from1);
		$timestamp = substr($line, strpos($line,"p = ")+5);
		if(strlen($str_opcode)>3 && $str_opcode!="UNKNOWN") $packet_name=$str_opcode;
		else $packet_name=$hex_opcode;
		if($packet_name=="")
			continue;
		$file_name = "step2/".$packet_name.".txt";
		if(!($f_handlers[$file_name]>0))
		{
			$outf = fopen($file_name, "a+");
			if(!$outf)
				parse_debug_report("could not open outputfile($file_name)!<br>\n");
			else $f_handlers[$file_name]=$outf;
		}
		else $outf = $f_handlers[$file_name];
		if($outf>0)
		{
			$line = rtrim(fgets($inp_h)); //data line
			if(strlen($line)>2)
			{
				if($hex_opcode==0x01F6 && $outf_comp)
				{
					fputs($outf_comp,"$timestamp \r\n");
					fputs($outf,str_replace(array("\n", "\r"),"",rtrim($line))." \r\n");
				}
				else 
					fputs($outf,str_replace(array("\n", "\r"),"",rtrim($line))." ".$timestamp."\r\n");
			}
		}
	}
}
if($inp_h)
	fclose($inp_h);
if($outf_comp)
	fclose($outf_comp);
//die();
//delete the file that we processed already
unlink($processing_file); //maybe some packets have multiple chunks and will remain there and take space ;)
foreach($f_handlers as $key=>$value)
	fclose($value);
proceedtonextstep();
?>

