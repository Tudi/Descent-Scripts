<?php
foreach($_REQUEST as $request_name=>$request_value)
	$$request_name=mysql_escape_string($request_value);
?>

<table>
	<form name="get_data" action="<?=$_SERVER['PHP_SELF'];?>" method="post">
	<tr>
	  <td width="150" height="30" align="right">Found exisitng GM accounts at last parse : &nbsp;</td>
	  <td width="240"><input type="text" name="gm_accs" style="width:1024" value=""></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">line must have one (use "&" for separation):&nbsp;</td>
	  <td width="240"><input type="text" name="must_have_one" style="width:300" value="<?=$must_have_one?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">line must NOT have one (use "&" for separation)::&nbsp;</td>
	  <td width="240"><input type="text" name="must_not_have_one" style="width:300" value="<?=$must_not_have_one?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">line must have all (use "&" for separation):&nbsp;</td>
	  <td width="240"><input type="text" name="must_have_all" style="width:300" value="<?=$must_have_all?>"></td>
	</tr>
	<tr>
	  <td width="150" height="30" align="right">line must NOT have all (use "&" for separation)::&nbsp;</td>
	  <td width="240"><input type="text" name="must_not_have_all" style="width:300" value="<?=$must_not_have_all?>"></td>
	</tr>
	<tr><td><input type="submit" value="filter logs"></td></tr>
	</form>
</table>
<?php
$distinct_gm_names="";
for($i=-1;$i<100;$i++)
{
	if( $i >= 0 )
		$logfile = @fopen("gmcommand_$i.log","r");
	else
		$logfile = @fopen("gmcommand.log","r");
	if( !$logfile )
		continue;

	$must_have_one_list=explode("&",$must_have_one);
	$must_not_have_one_list=explode("&",$must_not_have_one);
	$must_have_all_list=explode("&",$must_have_all);
	$must_not_have_all_list=explode("&",$must_not_have_all);
	while(!feof($logfile)) 
	{
		$line = fgets($logfile);

		//check for GM names 
		$pieces=explode("[",$line);
		$pieces=explode("]",$pieces[2]);
		$distinct_gm_names[$pieces[0]]=1;

		//check if we have any terms to match
		if($must_have_one != "" && count($must_have_one_list))
		{
			$have_match = 0;
			foreach($must_have_one_list as $key =>$value)
			{
	//echo "$value -> $line <br>";
				if( strpos("@".$line,$value) != 0 )
				{
					$have_match = 1;
					break;
				}
			}
	//echo "test 1 have match = $have_match<br>";
			if( $have_match == 0 )
				continue;
		}
		//do not have this
		if( $must_not_have_one != "" && count($must_not_have_one_list))
		{
			$have_match = 0;
			foreach($must_not_have_one_list as $key =>$value)
				if( strpos("@".$line,$value) != 0 )
			{
				$have_match = 1;
				break;
			}
	//echo "test 2 have match = $have_match<br>";
			if( $have_match == 1 )
				continue;
		}
		//have all this
		if($must_have_all != "" && count($must_have_all_list))
		{
			$have_match = 0;
			foreach($must_have_all_list as $key =>$value)
				if( strpos("@".$line,$value) != 0 )
					$have_match++;
	//echo "test 3 have match = $have_match<br>";
			if( $have_match != count($must_have_all_list) )
				continue;
		}
		//do not have any of these this
		if($must_not_have_all != "" && count($must_not_have_all_list) )
		{
			$have_match = 0;
			foreach($must_not_have_all_list as $key =>$value)
				if( strpos("@".$line,$value) != 0 )
			{
				$have_match++;
				break;
			}
	//echo "test 4 have match = $have_match<br>";
			if( $have_match != 0 )
				continue;
		}
		echo $line."<br>";

	}
	fclose($logfile);
}

$gm_accs="";
foreach($distinct_gm_names as $gm_acc=>$key)
	$gm_accs .="$gm_acc,";
//echo $gm_accs;
?>

<script language="JavaScript" type="text/javascript">
     get_data.gm_accs.value="<?=$gm_accs;?>";
</script>
