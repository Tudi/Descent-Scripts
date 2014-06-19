<?php
include("include/header.inc.php");
?>
!Create an empty table "creature_spawns_dest" to store results. After done drop old table, rename dest table to old one and clean up formations and waypoints <br>
This tool is designed to parse an existing spawn table and remove duplicate spawns <br>
This tool is yet in an experimental stage !! <br>
<br>
<?php

//we just fetch all existing spawns :D (damn that will be huge)
if($map_id!="")
{
	$last_id=0;
	$get_query="select * from creature_spawns where map='$map_id' and id>$last_id order by id asc";
//	$get_query.=" limit 0,100 ";
	$fetches_made=0;
	$inserts_made=0;
	$get_res=mysql_query($get_query,$dbi) or die(" Q20070820180311 ".mysql_error($dbi)."<br>".$get_query);
	while($row=mysql_fetch_row($get_res))
	{
		$fetches_made++;
		list($id,$entry,$map_id,$x,$y,$z)=$row;
		//now get a handfull of spanws near us
		$min_x=$x-$min_spawn_dist;
		$max_x=$x+$min_spawn_dist;
		$min_y=$y-$min_spawn_dist;
		$max_y=$y+$min_spawn_dist;
		$min_z=$z-$min_spawn_dist;
		$max_z=$z+$min_spawn_dist;
//		$get_query2="select count(id) from creature_spawns_dest where map='$map_id' and entry=$entry and position_x>=$min_x and position_x<=$max_x and position_y>=$min_y and position_y<=$max_y and position_z>=$min_z and position_z<=$max_z limit 0,1";
		$get_query2="select count(id) from creature_spawns_dest where map='$map_id' and position_x>=$min_x and position_x<=$max_x and position_y>=$min_y and position_y<=$max_y and position_z>=$min_z and position_z<=$max_z limit 0,1";
		$get_res2=mysql_query($get_query2,$dbi) or die(" Q20070820180312 ".mysql_error($dbi)."<br>".$get_query2);
		list($is_covered)=mysql_fetch_row($get_res2);
		if($is_covered==0)
		{
			$ins_q="insert into creature_spawns_dest values (";
			foreach($row as $key =>$value)
				$ins_q .="'".$value."',";
			$ins_q=substr($ins_q,0,-1).")";
//			echo $ins_q."<br>";
			mysql_query($ins_q,$dbi) or die(" Q20070820180313 ".mysql_error($dbi)."<br>".$ins_q);
			$inserts_made++;
		}
	}
echo "finished removing double spawns , removed ".($fetches_made - $inserts_made)." spanws <br>";
}

?>
<br>
<table>
	<form name="get_data" action="index.php" method="post">
	<tr><td><input type="submit" value="Start eliminating"></td></tr>
	<tr>
	  <td width="250" height="30" align="right">specify map which will be cleansed</td>
	  <td width="240"><input type="text" name="map_id" style="width:180" value="0"></td>
	</tr>
	<tr>
	  <td width="250" height="30" align="right">specify distance from 1 spawn to another that means double spawn</td>
	  <td width="240"><input type="text" name="min_spawn_dist" style="width:180" value="1"></td>
	</tr>
	<tr>
	  <td width="250" height="30" align="right">use Z in distance calc </td>
	  <td width="240"><input type="text" name="use_Z" style="width:180" value="10"></td>
	</tr>
	<tr><td><input type="submit" value="Start eliminating"></td></tr>
	</form>
</table>
