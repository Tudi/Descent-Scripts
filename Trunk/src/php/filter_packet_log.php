<?php
set_time_limit( 5*60 );
foreach($_REQUEST as $request_name=>$request_value)
	$$request_name=mysql_escape_string($request_value);
?>
You cannot undo changes. Make a backup ! Input is <b>Log.txt</b><br>
<table>
	<form name="get_data" action="<?=$_SERVER['PHP_SELF'];?>" method="post">
	<tr>
	  <td width="150" height="30" align="right">remove packets(line ID) : &nbsp;</td>
	  <td width="240"><input type="text" name="filter" style="width:1024" value=""></td>
	</tr>
	<tr><td><input type="submit" value="filter logs"></td></tr>
	</form>
</table>
<?php
$distinct_packet_types="";

$whole_file = "";
$skip_lines = 0;
$logfile = @fopen("log.txt","r");
if( $logfile )
{
	while( !feof($logfile) ) 
	{
		//read
		$line = fgets($logfile);
		//filtering this one
		if($skip_lines)
		{
			$skip_lines--;
			continue;
		}
		//check if we need to remove it
		if($filter != "" && strpos("#".$line,$filter) != 0 )
			$skip_lines = 2;
		else
		{
			for($i=0;$i<500;$i++)
			{
				$var_name = "rem_$i";
				if( $$var_name != "" && strpos("#".$line,$$var_name ) != 0 )
				{
//echo "Will skip packet ".($$var_name)."<br>";
					$skip_lines = 2;
					break;
				}
			}
		}
		if( $skip_lines )
			continue;
		//check for packet type and register it
		if( strpos("#".$line,"0x") != 0 )
		{
			$pieces=explode("(",$line);
			$pieces=explode(")",$pieces[1]);
			$distinct_packet_types[$pieces[0]]=$line;
		}
		//we will write back the non filtered content
		$whole_file .= $line;
	}
	fclose($logfile);
}
$logfile = @fopen("log.txt","w");
if( $logfile )
{
	fputs($logfile,$whole_file);
	fclose($logfile);
}

?>
<table border="1">
	<form name="get_data2" action="<?=$_SERVER['PHP_SELF'];?>" method="post">
	<tr><td><input type="submit" value="filter logs"></td></tr>
	<tr>
	  <td >sel</td>
	  <td >content</td>
	</tr>
	<?php
	$type_counter = 0;
	foreach($distinct_packet_types as $hexk=>$line)
	{
	?>
		<tr>
		  <td><input type="checkbox" name="rem_<?=$type_counter;?>" value="<?=$hexk;?>"></td>
		  <td><?=$line;?></td>
		</tr>
	<?php 
		$type_counter++;
	} 
	?>
	<tr><td><input type="submit" value="filter logs"></td></tr>
	</form>
</table>

<?php
//phpinfo();
?>