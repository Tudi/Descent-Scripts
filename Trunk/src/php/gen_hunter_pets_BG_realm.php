<?php

$VENDOR_FIRST_ENTRY = $VENDOR_FIRST_ENTRY2 = 101000;
$ITEMS_PER_VENDOR = 119;
$VENDOR_NAME = "Reagent Vendor";
$vendor_look=array(20761,20705,29054,17374,18482,4532,1355,16203,17372,1859,3808,15516,18222,1882,4209,16610,3346,3798,1878,18324,11900,1728);
$vendor_look_count=22;
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="test311_world";
$creature_spawns_middle_x = -1863.74;
$creature_spawns_middle_y = 5429.98;
$creature_spawns_z = -11.8;
$creature_spawns_radius =  150; //50 is great

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

//echo $unique_list."<br>";
$get_query = "select creature_names.entry from creature_names,creature_proto where creature_names.entry=creature_proto.entry group by family order by minlevel asc ";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$entry_list="";
$entry_list_count=0;
$set_mob_levels = "update creature_proto set minlevel=1,maxlevel=1,faction=7,minhealth=100000000,maxhealth=100000000,mindamage=10,maxdamage=10,respawntime=1000 where entry in (";
while($row=mysql_fetch_row($get_res) )
{
	$entry_list[$entry_list_count++]=$row[0];
	$set_mob_levels .= $row[0].",";
}
$set_mob_levels = substr($set_mob_levels,0,-1).");<br>";
echo $set_mob_levels;

if( $dbi)
	mysql_close($dbi);

//now to spawns them
$count_to_spawn = $entry_list_count;
$base_angle = 90;
//$angle_step = 360 / $count_to_spawn;
$angle_step = 60 / $count_to_spawn;
$base_facing = 1.57;
//$facing_angle_step = 6.28 / $count_to_spawn;
$facing_angle_step = 1.04 / $count_to_spawn;
for($i=0;$i<$count_to_spawn;$i++)
{
	$angle_now = $base_angle + $angle_step * $i;
	$radian_angle = $angle_now / 180 * 3.14;
	$facing = $base_facing + 3.14 + $i * $facing_angle_step;
	$x_now = $creature_spawns_middle_x + $creature_spawns_radius * cos( $radian_angle );
	$y_now = $creature_spawns_middle_y + $creature_spawns_radius * sin( $radian_angle );
	$entry=$entry_list[$i];
	$display = $vendor_look[$i % $vendor_look_count];
	echo "delete from creature_spawns where entry=$entry;<br>";
	echo "insert into creature_spawns (entry,map,position_x,position_y,position_z,orientation,faction) values ($entry,530,$x_now,$y_now,$creature_spawns_z,$facing,7);<br>";
}

?>