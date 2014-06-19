<?php
set_time_limit (3000);

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/


$get_query = "select * from vehicledbccsv";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query ");
while($row=mysql_fetch_row($get_res))
{
	$SeatsAvailable = 0;
	for($i=0;$i<8;$i++)
	{
		$seatid = $row[ 6 + $i ];
		if( $seatid <= 0 )
			continue;
		$get_query = "select count(*) from vehicleseatdbccsv where id = $seatid and (flags & 0x2000000)!=0";
		$get_res2=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query ");
		$row2=mysql_fetch_row($get_res2);
		if( $row2[0] > 0 )
			$SeatsAvailable++;
	}
	if( $SeatsAvailable == 0 )
		echo "# Vehicle ${row[0]} has NO seats <br>";
	else
	{
		echo "insert into vehicle_seat_data ( vehicleid, capacity ) values ( ${row[0]}, $SeatsAvailable );<br>";
	}
}
?>