<?php
	set_time_limit ( 60 * 5 ); 
	
	$dbhost = "localhost";
	$dbname = "characters";
	$dbuname = "root";
	$dbupass = "lofaszka";
	$dbtype = "MySQL";

	$dbim =mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbim) or die("Q 200503271826");

	$result=mysql_query("select * from pvpstatuestatus order by points desc",$dbim) or die("Q 200503271826");
	
	$rowcounter = 0;
	while( $row=mysql_fetch_row($result) )
	{
		$result2=mysql_query("delete from pvpstatuestatus where player=".$row[0]." and NPCType=".$row[2]." and points<".$row[1]."");		
		echo $rowcounter."<br>";
		$rowcounter++;
	}
	if( $dbi )
	{
		mysql_close($dbi);
		$dbi = 0;
	}
?>
