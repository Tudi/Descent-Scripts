<?php
	set_time_limit (15000);
	$dbhost="localhost";
	$dbuname="root";
	$dbupass="";
	$dbname="wowbeez_DB";
	$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbi) or die("Q 200603201239");

	$InrangeRadius = 80;
	$AvgSizeSum = 0;
	$SampleCount = 0;
	$MaxSize = 0;
	$NotAloneAvgSize = 0;
	$NotAloneAvgSizeCount = 0;
	
	$DebugLimitStart = 0;
	$DebugLimit = 50000;
	$DebugLimitSql = " limit $DebugLimitStart,$DebugLimit";

	$get_query2 = "SELECT count(*) from creature_spawns $DebugLimitSql"; 
	$get_res2 = mysql_query($get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$row=mysql_fetch_row($get_res2);
	$TotalRowsToProcess = $row[0];
	if( $DebugLimit > 0 && $DebugLimit < $TotalRowsToProcess )
		$TotalRowsToProcess = $DebugLimit;
	$PrevPctShown = 0;

	$get_query = "SELECT id,map,position_x,position_y from creature_spawns order by ID $DebugLimitSql";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	while(list($id,$map,$x,$y)=mysql_fetch_row($get_res))
	{
		$get_query2 = "SELECT count(*) from creature_spawns where map=$map and position_x>$x-$InrangeRadius and position_x<$x+$InrangeRadius and position_y>$y-$InrangeRadius and position_y<$y+$InrangeRadius"; 
		$get_res2 = mysql_query($get_query2,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
		$row=mysql_fetch_row($get_res2);
		$SampleCount++;
		$AvgSizeSum += $row[0];
		if( $row[0] > $MaxSize )
			$MaxSize = $row[0];
		if( $row[0] > 1 )
		{
			$NotAloneAvgSize += $row[0];
			$NotAloneAvgSizeCount++;
		}
		$PctProgressNow = (int)($SampleCount * 100 / $TotalRowsToProcess);
//echo "Pct now $PctProgressNow $PrevPctShown $TotalRowsToProcess $SampleCount<br>";	
		if( (int)$PctProgressNow != (int)$PrevPctShown )
		{
			$PrevPctShown = $PctProgressNow;
			echo "Pct proccessed : $PrevPctShown <br>";
			echo "Avg size ".($AvgSizeSum/$SampleCount)." max size $MaxSize from $SampleCount samples<br>";
			echo "Avg size when not alone ".($NotAloneAvgSize/$NotAloneAvgSizeCount)." from $NotAloneAvgSizeCount samples<br>";
			echo "====================================<br>";
		}
	}
	echo "Avg size ".($AvgSizeSum/$SampleCount)." max size $MaxSize from $SampleCount samples<br>";
	echo "Avg size when not alone ".($NotAloneAvgSize/$NotAloneAvgSizeCount)." from $NotAloneAvgSizeCount samples<br>";
?>
