<?php

$UNUSED_TO_BIG_VALUE = 999999999;

$MERGER_debug=0;

function findbestmatch($x,$y,$z,$path2,$lastmatch_at)
{
	global $MIN_WP_GAP,$UNUSED_TO_BIG_VALUE;
	$WP_EXACT_MATCH_ERROR = $MIN_WP_GAP;//this is due to float precision calculations
	$match_dist=$UNUSED_TO_BIG_VALUE;
	$match_key=$UNUSED_TO_BIG_VALUE;
	$exactmatch=0;
	$best_match=$UNUSED_TO_BIG_VALUE; //maybe later used for adaptive algorithms
	$res="";
	for($i=$lastmatch_at+1;$i<$path2["lk"];$i++)
	{
//		$dist=sqrt(($x-$value2['x'])*($x-$value2['x'])+($y-$value2['y'])*($y-$value2['y'])+($z-$value2['z'])*($z-$value2['z']));
		$dist=sqrt(($x-$path2[$i]['x'])*($x-$path2[$i]['x'])+($y-$path2[$i]['y'])*($y-$path2[$i]['y']));
		if($dist<$match_dist)
		{
			$match_dist=$dist;
			$match_key = $i;
			if($dist<$WP_EXACT_MATCH_ERROR)
				$exactmatch=1;
		}
		if($dist<$best_match)
			$best_match = $dist;
	}
	$res['d']=$best_match;
	$res['k']=$match_key;
	$res['e']=$exactmatch;
	return $res;
}

//to be honest the chances of merging 2 paths are very slick 
function mergepaths($path1,$path2)
{
	global $UNUSED_TO_BIG_VALUE;
	//get the points that theoretically owerlay
	$firstmatch=$UNUSED_TO_BIG_VALUE;
	$lastmatch=$UNUSED_TO_BIG_VALUE;
	$best_match=$UNUSED_TO_BIG_VALUE;
	$lastmatch_at=1;
	for($i=1;$i<$path1["lk"];$i++)
	{
		//get best match
		$res=findbestmatch($path1[$i]['x'],$path1[$i]['y'],$path1[$i]['z'],$path2,$lastmatch_at);
		$path1[$i]["mk"]=$res['k']; //the match key
//		$path1[$i]["em"]=$res['e']; //if this is an exaact match
		if($res['e']==1)
		{
			if($firstmatch==$UNUSED_TO_BIG_VALUE)
				$firstmatch=$i;
			$lastmatch=$i;
			$lastmatch_at = $res['k'];//we try to not find this match again
		}
		if($res['d']<$best_match)
			$best_match = $res['d'];
	}
	$path1['mr']=0;
	//in case we got no possible merging point. Since we call this function only in case we found a match point before, it is good to debug this
	if($firstmatch==$UNUSED_TO_BIG_VALUE)
	{
//		parse_debug_report("no merge path points found for creature ".$path1['spid']." with ".$path2['spid']." firstm = $firstmatch , lastm = $lastmatch, lastkey1 = ".$path1['lk']." ,lastkey2 = ".$path2['lk'].", bestmatch = '".$best_match."'");
		return $path1;
	}
	//only 1 point is match
	if($firstmatch==$lastmatch)	
		if($lastmatch==$path1['lk']-1 && $path1[$lastmatch]['mk']==1)//path2 is a continuation of path1
		{
			for($j=$path1[$lastmatch]['mk']+1;$j<$path2['lk'];$j++)
			{
				$path1[$path1['lk']]=$path2[$j];
				$path1[$path1['lk']]['wpid']=$path1['lk']-1;
				$path1['lk']++;//next free key
			}
			$path1['mr']=1;//keep only path1
//			parse_debug_report("succefully merged path points for creature (continuation of path1)".$path1['spid']." with ".$path2['spid']." firstm = $firstmatch , lastm = $lastmatch, lastkey1 = ".$path1['lk']." ,lastkey2 = ".$path2['lk'].", bestmatch = '".$best_match."'");
			return $path1;
		}
	//path1 fully covers path2 (or inverse path2)
	if($path1[$firstmatch]['mk']==1 && $path1[$lastmatch]['mk']==$path2['lk']-1)
	{
		$path1['mr']=1;//keep only path1
//		parse_debug_report("succefully merged path points for creature (fullcover)".$path1['spid']." with ".$path2['spid']." firstm = $firstmatch , lastm = $lastmatch, lastkey1 = ".$path1['lk']." ,lastkey2 = ".$path2['lk'].", bestmatch = '".$best_match."' path1 covers path2");
		return $path1;
	}
	//if path2 is a continuation of path1 but more then 1 point is the same
	if($path1[$firstmatch]['mk']==1)
	{
		//more then 1 point mathes at the end of paths
		$succesful_continuation=1;
		$mdif=$path1[$firstmatch]['mk'];
		for($i=0;$i<=$lastmatch-$firstmatch;$i++)
			if($path1[$i+$firstmatch]['mk']!=$i+$mdif)
			{
				$succesful_continuation=0;
				break;
			}
		//we can merge the 2 paths
		if($succesful_continuation)
		{
			for($i=$path1[$lastmatch]['mk']+1;$i<$path2["lk"];$i++)
			{
				$path1[$path1['lk']]=$path2[$i];
				$path1[$path1['lk']]['wpid']=$path1['lk']-1;
				$path1['lk']++;//next free key
			}
			$path1['mr']=1;//keep only path1
//			parse_debug_report("succefully merged path points for creature (multiple cover)".$path1['spid']." with ".$path2['spid']." firstm = $firstmatch , lastm = $lastmatch, lastkey1 = ".$path1['lk']." ,lastkey2 = ".$path2['lk'].", bestmatch = '".$best_match."'");
			return $path1;
		}
	}
	return $path1;
}

function getprocessed_path($cr_spawn_id)
{
	global $dbi;
	global $MIN_WP_GAP;
	$path1="";
	$wp_counter=0;

	$check_query3="select position_x,position_y,position_z from creature_spawns where id='$cr_spawn_id'";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506301 ".mysql_error($dbi));
	list($x,$y,$z)=mysql_fetch_row($check_res3);
	$path1[$wp_counter]['x']=$x;
	$path1[$wp_counter]['y']=$y;
	$path1[$wp_counter]['z']=$z;
	$path1[$wp_counter]['wait']=0;
	$path1[$wp_counter]['f']=0;
	$path1[$wp_counter]['wpid']=-1; //we should not store this as a waypoint. 
	$wp_counter++;

	//only load waypoints that we are actually going to use. Strength waypoints are ignored
	$check_query3="select position_x,position_y,position_z,waittime,flags,waypointid from creature_waypoints where spawnid='$cr_spawn_id' and waypointid<1000 order by waypointid asc";	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506301 ".mysql_error($dbi));
	while(list($x,$y,$z,$wt,$f,$wpid)=mysql_fetch_row($check_res3))
	{
		$path1[$wp_counter]['x']=$x;
		$path1[$wp_counter]['y']=$y;
		$path1[$wp_counter]['z']=$z;
		$path1[$wp_counter]['wait']=$wt;
		$path1[$wp_counter]['f']=$f;
		$path1[$wp_counter]['wpid']=$wpid;
		$wp_counter++;
	}
	$path1['wpcount']=$wp_counter;
	$path1['lk']=$wp_counter;
	$path1['biglk']=$wp_counter; //contains strengthening wp too
	$path1['spid']=$cr_spawn_id;
	if($wp_counter>1000)
		parse_error_report("!!!path had more then 1000 waypoints($wp_counter). This will result in a crash at spawning creatures");
	return $path1;
}

function getunprocessed_path($gl,$gh,$sp_x,$sp_y)
{
	global $dbi,$MERGER_debug;
	global $MIN_WP_GAP;
	global $MIN_WP_COUNT;
	global $MOVEMENTTYPE_NONE,$MOVEMENTTYPE_RANDOMWP,$MOVEMENTTYPE_CIRCLEWP,$MOVEMENTTYPE_FORWARDTHANBACK;

	//get creature walk speed
	$check_query3="select creature_template_id from creature_lookup where guid_low='$gl' and guid_high='$gh' limit 0,1";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
	list($entry)=mysql_fetch_row($check_res3);
	$check_query3="select speed from creature_template where id='$entry'";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
	list($speed)=mysql_fetch_row($check_res3);
	if($speed==0)
	{
		parse_debug_report("We could not get creature speed for spawn so we are asuming it has 7 walkspeed sp_x=$sp_x and sp_y=$sp_y");
		$speed=7;
	}

	$path1="";
	$wp_counter=0;

	$path1[$wp_counter]['x']=$sp_x;
	$path1[$wp_counter]['y']=$sp_y;
	$path1[$wp_counter]['z']=0;
	$path1[$wp_counter]['wait']=0;
	$path1[$wp_counter]['f']=0;
	$path1[$wp_counter]['wpid']=-1; //we should not store this as a waypoint. 
	$wp_counter++;

	$spawn_count=0;
	$spawn_timestamps="";
	$check_query3="select spawn_timestamp,sp_x,sp_y,sp_z from creature_lookup where guid_low='$gl' and guid_high='$gh'";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
	while(list($spawn_timestamp,$x,$y,$z)=mysql_fetch_row($check_res3))
	{
		$spawn_timestamps[$spawn_count]['ts']=$spawn_timestamp;
		$spawn_timestamps[$spawn_count]['x']=$x;
		$spawn_timestamps[$spawn_count]['y']=$y;
		$spawn_timestamps[$spawn_count]['z']=$z;
		$spawn_count++;
	}
	$spawn_timestamps[$spawn_count]['ts']=4294967295;//make sure this is the last time stamp
	$path1['spcount']=$spawn_count;
	$spawn_itr=0;

	$prev_x=$sp_x;
	$prev_y=$sp_y;
	$prev_tstamp=0;

	//there are cases in city when you see citizen walk a portion of he's path then see him again walk some more. In case we manage to detect these interuption we should use biggest continues portion
	$segment_count=0;
	$longest_segment_len=0;
	$longest_segment_start_id=0;
	$longest_segment_end_id=0;

	$check_query3="select x,y,z,walkback,flags,timestamp from creature_movement where guid_low='$gl' and guid_high='$gh' order by id asc";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
	$wp_with_wait_time=0;
	while(list($x,$y,$z,$wb,$f,$timestamp)=mysql_fetch_row($check_res3))
	{
		if( $wp_counter>1 && $spawn_timestamps[$spawn_itr+1]['ts']<=$timestamp)
		{
//echo "!!!!!$spawn_itr) $spawn_count we are inserting spawn point to path $wp_counter ".$spawn_timestamps[$spawn_itr]['ts']." < ".$timestamp." < ".$spawn_timestamps[$spawn_itr+1]['ts']."<br>";
			$path1[$wp_counter]['x']=$spawn_timestamps[$spawn_itr]['x'];
			$path1[$wp_counter]['y']=$spawn_timestamps[$spawn_itr]['y'];
			$path1[$wp_counter]['z']=$spawn_timestamps[$spawn_itr]['z'];
			$path1[$wp_counter]['f']=0;
			$path1[$wp_counter]['wpid']=-1;
			$path1[$wp_counter]['ts']=$spawn_timestamps[$spawn_itr]['ts'];
			$path1[$wp_counter]['wait']=0;
			$wp_counter++;
			$spawn_itr++;
		}

		$dist=sqrt(($x-$prev_x)*($x-$prev_x)+($y-$prev_y)*($y-$prev_y));
		//we used first WP as our spawning point, so we skip that
		if($dist<$MIN_WP_GAP && $wp_counter>1)
		{
			$prev_x=$x;
			$prev_y=$y;
			$prev_tstamp=$timestamp;
			continue;
		}
		$prev_x=$x;
		$prev_y=$y;
		if(($f & 0x100))
			$wt=0; //this is for running creatures
		else 
		{
			$timediff=$timestamp-$prev_tstamp;
			$distance_could_walk=$speed*$timediff/1000;
			$distance_left_to_wait=$distance_could_walk-$dist;
			$time_for_wait=$distance_left_to_wait/$speed*1000;
//parse_debug_report("timestamp=$timestamp,timed=$timediff,distance_could_walk=$distance_could_walk,dist=$dist,distance_left_to_wait=$distance_left_to_wait,time_for_wait=$time_for_wait");
			if($time_for_wait<500)	//client lag in milliseconds (0.5 second seems to be a reasonable value since this is inluenced only by lag fluctuations)
				$wt=0;	//we are asuming this short delay is because of server/client lag
			else if($time_for_wait<1000)//something must have went wrong here
			{
				$wt=1000+$time_for_wait;
				$wp_with_wait_time++;
			}
			else if($timediff>20000)//something must have went wrong here
				$wt=0;
			else 
			{
				$wt=$time_for_wait;
				$wp_with_wait_time++;
			}
		}
		$prev_tstamp=$timestamp;
		$path1[$wp_counter]['x']=$x;
		$path1[$wp_counter]['y']=$y;
		$path1[$wp_counter]['z']=$z;
		$path1[$wp_counter]['wait']=$wt;
		$path1[$wp_counter]['f']=$f;
		$path1[$wp_counter]['wpid']=$wp_counter;
		$path1[$wp_counter]['ts']=$timestamp;
		$wp_counter++;
	}

	if($wp_with_wait_time<$wp_counter)
	{

		$check_query3="select count(distinct(spawnid)) from creature_waypoints where creature_entry=$entry and waittime!=0";
		$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
		list($not_0_examples)=mysql_fetch_row($check_res3);
		$check_query3="select count(distinct(spawnid)) from creature_waypoints where creature_entry=$entry and waittime=0";
		$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
		list($with_0_examples)=mysql_fetch_row($check_res3);
		if(($not_0_examples>$with_0_examples && $not_0_examples>2) || $wp_with_wait_time>=$wp_counter/2)
		for($i=0;$i<$wp_counter;$i++)
			if($path1[$i]['wait']<1000)
				$path1[$i]['wait']=2000 + rand(1000,2000);
	}

	$path1['wpcount']=$wp_counter;
	$path1['lk']=$wp_counter;
	$path1['biglk']=$wp_counter; //contains strengthening wp too

	$path1 = select_longest_continues_path($path1,$wp_counter);

	if($path1['wpcount']<$MIN_WP_COUNT)
		$path1['wpcount']=0;//do not save path if it is too small

//echo "wp counter now is $wp_counter and the var val = ".get_config_var("spline_waypoints")." and gl=$gl<br>";
	if( ($path1['wpcount']<=1 && get_config_var("spline_waypoints")==1) )
		$path1 = getunprocessed_spline_path($gl,$gh,$sp_x,$sp_y);

	if($path1['wpcount']>1000)
		parse_error_report("!!!path had more then 1000 waypoints($wp_counter). This will result in a crash at spawning creatures");

	if($path1['wpcount']==0)
		$path1['walktype'] = $MOVEMENTTYPE_NONE;

	return $path1;
}

function select_longest_continues_path($path1,$wp_counter)
{
	global $dbi,$MERGER_debug;
	global $MIN_WP_GAP;
	global $MIN_WP_COUNT;
	global $MOVEMENTTYPE_NONE,$MOVEMENTTYPE_RANDOMWP,$MOVEMENTTYPE_CIRCLEWP,$MOVEMENTTYPE_FORWARDTHANBACK;
	$path2="";

	//break the path down to smaller paths
	$section2_counter=0;
	$longest_section=0;
	$longest_section_at=0;
	$circles_found=0;
	$forthbacks_found=0;
	$dist_sum=0;
	$halfs_found=0;
//echo "we arrived at select_longest_continues_path with len $wp_counter<br>";
	for($i=0;$i<$wp_counter;$i++)
	{
		$path1[$i]['section2']=$section2_counter;
		if( $path1[$i]['wpid']==-1 )
		{
//echo "we marked a new spawnpoint at $i with len $len_counter<br>";
			if($len_counter>$longest_section)
			{
				$longest_section=$len_counter;
				$longest_section_at=$section2_counter;
			}
			$len_counter=0;
			$count_len=1;
		}
		if($count_len)
			$len_counter++;

		$dist_next=sqrt(	($path1[$i]['x']-$path1[$i+1]['x'])*($path1[$i]['x']-$path1[$i+1]['x'])
						+
							($path1[$i]['y']-$path1[$i+1]['y'])*($path1[$i]['y']-$path1[$i+1]['y']));
		//if the distance is big and there was a new spawn meantime then we segment this
		if($dist_next>$dist_sum/$len_counter+$dist_sum/$len_counter/2 && $path1[$i+1]['wpid']==-1)
		{
			$section2_counter++;
//echo "we marked a new section2 at $i with len $len_counter<br>";
			continue;
		}
//else if( $path1[$i+1]['spawnpointpresent']==1 )
//	echo "we are end of path1 and will merge with path2 because distance is small = $dist_next and avg distance is ".($dist_sum/$len_counter)."<br> ";
		$dist_sum+=$dist_next;
		//circle walk type : if we arived where we started it but we made a full circle 
		if( $i > 1)
		{
			$dist1=sqrt(	($path1[0]['x']-$path1[$i]['x'])*($path1[0]['x']-$path1[$i]['x'])
							+
							($path1[0]['y']-$path1[$i]['y'])*($path1[0]['y']-$path1[$i]['y']));
			if($dist1<($MIN_WP_GAP+$MIN_WP_GAP/2)) //if we are closer then 1.5 wp dist then it is worth to check it
			{
				$dist2=sqrt(	($path1[1]['x']-$path1[$i+1]['x'])*($path1[1]['x']-$path1[$i+1]['x'])
								+
								($path1[1]['y']-$path1[$i+1]['y'])*($path1[1]['y']-$path1[$i+1]['y']));
				if($dist2<($MIN_WP_GAP+$MIN_WP_GAP/2) && abs($dist1-$dist2)<=$MIN_WP_GAP)
				{
					if($MERGER_debug)
						parse_debug_report("elimitated repeating path : old waypoint count from $wp_counter to $i, dist1=$dist1 and dist2=$dist2 ");
					$path1[$i]['madecircle']=1;
					//this section will be limited to this len
					$count_len=0;
//echo "we marked a new circlewp at $i and len is $len_counter<br>";
					$circles_found++;
					continue;
				}
			}
		}
		//forth->back walk type : ! This is most common blizz uses ;)
		$dist1=sqrt(	($path1[$i]['x']-$path1[$i+2]['x'])*($path1[$i]['x']-$path1[$i+2]['x'])
						+
						($path1[$i]['y']-$path1[$i+2]['y'])*($path1[$i]['y']-$path1[$i+2]['y']));
		if($dist1<($MIN_WP_GAP+$MIN_WP_GAP/2)) //if we are closer then 1.5 wp dist then it is worth to check it
		{
			$dist2=sqrt(	($path1[$i-1]['x']-$path1[$i+3]['x'])*($path1[$i-1]['x']-$path1[$i+3]['x'])
							+
							($path1[$i-1]['y']-$path1[$i+3]['y'])*($path1[$i-1]['y']-$path1[$i+3]['y']));
			if($dist2<($MIN_WP_GAP+$MIN_WP_GAP/2) && abs($dist1-$dist2)<=$MIN_WP_GAP)
			{
				if($MERGER_debug)
					parse_debug_report("elimitated walkback path : old waypoint count from $wp_counter to $i, dist1=$dist1 and dist2=$dist2 ");
				$path1[$i]['halfwalk']=1;
				$halfs_found++;
				if( $halfs_found==2 ) //this section is limited to this len
				{
					$count_len=0;
//echo "we marked a new forth-back at $i and len is $len_counter<br>";
				}
				$forthbacks_found++;
				continue;
			}
		}
	}
	if($len_counter>$longest_section)
	{
		$longest_section=$len_counter;
		$longest_section_at=$section2_counter;
//echo "we marked a new section2 at $i with len $len_counter<br>";
	}

	//if creature is a random wp walker then we will keep all and get rid of them later
	if( ($forthbacks_found+$circles_found)>=$wp_counter/2 || $path1['spcount']<=1 || ($forthbacks_found==0 && $circles_found==0))
	{
		$start_at=1;
		$end_at=$wp_counter;
		$path1['walktype'] = $MOVEMENTTYPE_RANDOMWP;
	}
	else
	{
//echo "This creature is not a random wp walker , it has forthbacks=$forthbacks_found and circles=$circles_found!<br>";
		//keep only the best choice and leave the rest
		$halfwalks_found=0;
		$start_at=-1;
		$end_at=$wp_counter;
		for($i=0;$i<$wp_counter-1;$i++)
		{
			if($start_at==-1 && $path1[$i]['section2']==$longest_section_at)
				$start_at=$i;
			if($path1[$i]['halfwalk']==1)
				$halfwalks_found++;
			if($halfwalks_found==2)
			{
				$end_at=$i;
				$path1['walktype'] = $MOVEMENTTYPE_FORWARDTHANBACK; //this would be great if we would spawn our unit on the end 
				break;
			}
			if( $path1[$i]['madecircle']==1 )
			{
				$end_at=$i;
				$path1['walktype'] = $MOVEMENTTYPE_CIRCLEWP;
				break;
			}
		}
		if($start_at<0)
			$start_at=1;
//echo "we will keep path from $start_at to $end_at <br>";
//if($end_at!=$wp_counter)
//	echo "we are not keeping all the waypoints !<br>";
	}

	$wp_counter2=0;
	$path2[$wp_counter2]['x']=$path1[$wp_counter2]['x'];
	$path2[$wp_counter2]['y']=$path1[$wp_counter2]['y'];
	$path2[$wp_counter2]['wpid']=-1; //we should not store this as a waypoint. 
	$wp_counter2++;
	for($i=$start_at;$i<$end_at;$i++)
	{
		$path2[$wp_counter2]['x']=$path1[$i]['x'];
		$path2[$wp_counter2]['y']=$path1[$i]['y'];
		$path2[$wp_counter2]['z']=$path1[$i]['z'];
		$path2[$wp_counter2]['wait']=$path1[$i]['wait'];
		$path2[$wp_counter2]['f']=$path1[$i]['f'];
		if($path1[$i]['wpid']==-1)
			$path2[$wp_counter2]['wpid']=-1;
		else
			$path2[$wp_counter2]['wpid']=$wp_counter2;
		$path2[$wp_counter2]['ts']=$path1[$i]['ts'];
		$wp_counter2++;
	}

	$path2['wpcount']=$wp_counter2;
	$path2['lk']=$wp_counter2;
	$path2['biglk']=$wp_counter2; //contains strengthening wp too

	return $path2;
}

function getunprocessed_spline_path($gl,$gh,$sp_x,$sp_y)
{
	global $dbi,$MERGER_debug;
	global $MIN_WP_GAP;
	global $MIN_WP_COUNT;
	global $MOVEMENTTYPE_NONE,$MOVEMENTTYPE_RANDOMWP,$MOVEMENTTYPE_CIRCLEWP,$MOVEMENTTYPE_FORWARDTHANBACK;

	$path1="";
	$wp_counter=0;

	$path1[$wp_counter]['x']=$sp_x;
	$path1[$wp_counter]['y']=$sp_y;
	$path1[$wp_counter]['z']=0;
	$path1[$wp_counter]['wait']=0;
	$path1[$wp_counter]['f']=0;
	$path1[$wp_counter]['wpid']=-1; //we should not store this as a waypoint. 
	$wp_counter++;

	$prev_x=$sp_x;
	$prev_y=$sp_y;

	$check_query3="select flags2,splineflags,time,time2,unk,wpcount from creature_movement_spline where guid_low='$gl' and guid_high='$gh' limit 0,1";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
	list($flags2,$splineflags,$time1,$time2,$unk,$wpcount)=mysql_fetch_row($check_res3);

	$check_query3="select x,y,z from creature_movement_spline where guid_low='$gl' and guid_high='$gh' order by id asc";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506302 ".mysql_error($dbi));
	while(list($x,$y,$z,$wb,$f,$timestamp)=mysql_fetch_row($check_res3))
	{

		if($x==$prev_x && $y==$prev_y)
			continue; //was spawn pos included in the list or not after all ?

		$dist=sqrt(($x-$prev_x)*($x-$prev_x)+($y-$prev_y)*($y-$prev_y));
//	echo "we are in spline wp table row with dist $dist<br>";
		//we used first WP as our spawning point, so we skip that
		if( $dist<$MIN_WP_GAP ) //flying creatures can fly over great distances without crossing anthing... no dneed for max distance
		{
			$prev_x=$x;
			$prev_y=$y;
			continue;
		}
		$prev_x=$x;
		$prev_y=$y;
		$wt=0; //these are constantly moving creatures
		if(($flags2 & 0x400))
			$f = 768; //flying creatures, this is how ascent notes it
		else $f = 0;
		$path1[$wp_counter]['x']=$x;
		$path1[$wp_counter]['y']=$y;
		$path1[$wp_counter]['z']=$z;
		$path1[$wp_counter]['wait']=$wt;
		$path1[$wp_counter]['f']=$f;
		$path1[$wp_counter]['wpid']=$wp_counter;
		$wp_counter++;
	}

	if($wp_counter<$MIN_WP_COUNT)
		$wp_counter=0;//do not save path if it is too small

	$path1['wpcount']=$wp_counter;
	$path1['lk']=$wp_counter;
	$path1['biglk']=$wp_counter; //contains strengthening wp too
	$path1['walktype'] = $MOVEMENTTYPE_FORWARDTHANBACK; //this would be great if we would spawn our unit on the end 

	if($wp_counter>1000)
		parse_error_report("!!!path had more then 1000 waypoints($wp_counter). This will result in a crash at spawning creatures");
	return $path1;
}

function delete_spawn_with_cleanup($sp_id)
{
	global $dbi;
	//we simply delete the spawn
	$check_query3="delete from creature_spawns where id='$sp_id'";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q20061123150461 ".mysql_error($dbi)."<br>".$check_query3."<br>");

	//also delete linked creatures to this one
	$check_query3="select spawn_id from creature_formations where target_spawn_id='$sp_id'";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q20061123150461 ".mysql_error($dbi)."<br>".$check_query3."<br>");
	list($pet_sp_id)=mysql_fetch_row($check_res3);
	if($pet_sp_id)
	{
		$check_query3="delete from creature_spawns where id='$pet_sp_id'";
		$check_res3=mysql_query($check_query3,$dbi) or die(" Q20061123150461 ".mysql_error($dbi)."<br>".$check_query3."<br>");
	}

	//and delete the waypoints too
	$check_query3="delete from creature_waypoints where spawnid='$sp_id'";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q20061123150461 ".mysql_error($dbi)."<br>".$check_query3."<br>");

}

function delete_spawn_and_add_strengthening_waypoints($path_del,$path_add,$is_merged)
{
	global $dbi;
	$del_sp=$path_del['spid'];
	$add_sp=$path_add['spid'];

	//this should never happen
	if($del_sp==$add_sp)
	{
		parse_debug_report("merge spawns : we are adding and deleting the same spawn :  $add_sp while deleting $del_sp");
		//we can delete the waypoints since we are going to reinsert them at the end anyway
		$check_query3="delete from creature_waypoints where spawnid='$del_sp'";
		$check_res3=mysql_query($check_query3,$dbi) or die(" Q20061123150461 ".mysql_error($dbi)."<br>".$check_query3."<br>");
		return $path_add;
	}

	//now add strengthening waypoints
	$wp_counter = $path_add['biglk'];
	if($wp_counter==0)
		parse_error_report("merge spawns : wp counter is 0 for spawn $add_sp while deleting $del_sp wp_count=$wp_counter");
	$wp_ids = $path_add[$path_add['biglk']-1]['wpid']+1000;//last wpid+1000
	if($is_merged!=1) //if we managed to merge the 2 paths then we already added these points
		for($i=0;$i<$path_del['biglk'];$i++) //we also add spawning point here as a waypoint so next time we can eliminate it
		{
			$path_add[$wp_counter]['x']=$path_del[$i]['x'];
			$path_add[$wp_counter]['y']=$path_del[$i]['y'];
			$path_add[$wp_counter]['z']=$path_del[$i]['z'];
			$path_add[$wp_counter]['wait']=$path_del[$i]['wait'];
			$path_add[$wp_counter]['f']=$path_del[$i]['f'];
			$path_add[$wp_counter]['wpid']=$wp_ids+$path_del[$i]['wpid'];
			$wp_counter++;
		}

	//add rest of the unloaded wp from db
	//only load waypoints that we are actually going to use. Strength waypoints are ignored
	$check_query3="select position_x,position_y,position_z,waittime,flags,waypointid from creature_waypoints where spawnid='$del_sp' and waypointid>=1000 order by waypointid asc";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q200711231506301 ".mysql_error($dbi));
	while(list($x,$y,$z,$wt,$f,$wpid)=mysql_fetch_row($check_res3))
	{
		$path_add[$wp_counter]['x']=$x;
		$path_add[$wp_counter]['y']=$y;
		$path_add[$wp_counter]['z']=$z;
		$path_add[$wp_counter]['wait']=$wt;
		$path_add[$wp_counter]['f']=$f;
		$path_add[$wp_counter]['wpid']=$wpid+$wp_ids;
		$wp_counter++;
	}

	if($wp_counter==$path_add['biglk'])
		parse_debug_report("merge spawns : we did not add any new spawns while we deleted others ? for spawn $add_sp while deleting $del_sp wp_count=$wp_counter");
	$path_add['biglk'] = $wp_counter;
	
	//to avoid conflict we delete add waypoints too
	$check_query3="delete from creature_waypoints where spawnid='$add_sp'";
	$check_res3=mysql_query($check_query3,$dbi) or die(" Q20061123150461 ".mysql_error($dbi)."<br>".$check_query3."<br>");

	delete_spawn_with_cleanup($del_sp);

	return $path_add;
}

//when 2 spawns some way cover each other this function is called to decide which one we keep
function decide_spawn_conflict($path_res,$spwn_id)
{
	$path2=getprocessed_path($spwn_id);
	$path2['spid']=$spwn_id;
	$path_res=mergepaths($path_res,$path2); //we eighter use best man win theory or try to merge the 2 paths
	//delete old spawn since we merged the 2
	if($path_res['mr']==1)
		$path_res = delete_spawn_and_add_strengthening_waypoints($path2,$path_res,$path_res['mr']);
	else if($path2['lk']<=$path_res['lk'])//we could not merge the 2. We keep the stronger spawned one
		$path_res = delete_spawn_and_add_strengthening_waypoints($path2,$path_res,0);
	else
		$path_res = delete_spawn_and_add_strengthening_waypoints($path_res,$path2,0);
	return $path_res;
}

function Insert_as_formation_if_possible( $path1 )
{
}

//main function, should check all conflict possibilitys with existing spawns in the pool. Very resource consuming procedure
function merge_spawn_into_spawn_pool($gl,$gh,$sp_x,$sp_y,$sp_z,$map,$cr_entry,$sp_id,$disp)
{
	global $dbi,$MINIMUM_SPAWN_DISTANCE,$MIN_WP_GAP,$MERGER_debug;
	$WP_EXACT_MATCH_ERROR = $MIN_WP_GAP;//this is due to float precision calculations
	
	//get the unprocessed path in our format
	$path1 = getunprocessed_path($gl,$gh,$sp_x,$sp_y,$sp_z);

	if ( get_config_var("No_double_spawn_check") )
	{
		$path1 = Insert_as_formation_if_possible($path1);
		return $path1; //these paths were made with mind vision without mob respawns. There is not need for double check
	}

	$inp_h = @fopen("spawn_debug.txt", "a");

	$already_merged_ids="";	//there is no point preocessing 1 spawn more then once

	$path1['spid']=$sp_id;

//	if($MERGER_debug)
//		parse_debug_report("wp count at step 0 ".$path1['lk']."");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//check to see if we were spawned on a path line (SP to WP check)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//let us find the biggest square what we can cover with our spawn location and path points
	$minx1=$sp_x-100;
	$maxx1=$sp_x+100;
	$miny1=$sp_y-100;
	$maxy1=$sp_y+100;
/*	for($i=0;$i<$path1['lk'];$i++)
	{
		if($path1[$i]['x']<$minx1)
			$minx1 = $path1[$i]['x'];
		if($path1[$i]['x']>$maxx1)
			$maxx1 = $path1[$i]['x'];
		if($path1[$i]['y']<$miny1)
			$miny1 = $path1[$i]['x'];
		if($path1[$i]['y']>$maxy1)
			$maxy1 = $path1[$i]['y'];
	}*/
	//now select all combination of waypoints from creatures 
	$check_query1="select spawnid,position_x,position_y,waypointid from creature_waypoints where position_x>='$minx1' and position_x<='$maxx1' and position_y>='$miny1' and position_y<='$maxy1' and position_z>='$minz1' and position_z<='$maxz1' and creature_entry='$cr_entry' and map_id='$map' order by spawnid,waypointid asc";
	$check_res1=mysql_query($check_query1,$dbi) or die(" Q200611231504 ".mysql_error($dbi));	
	$prev_spid=0;
	$prev_wp_id=0;
	while(list($spwn_id,$cur_x,$cur_y,$wp_id)=mysql_fetch_row($check_res1))
	{
		//check if this is a start of a new path
		if($prev_spid!=$spwn_id || ($prev_wp_id+1)!=$wp_id)
		{
			$prev_spid=$spwn_id;
			$prev_wp_id=$wp_id;
			$prev_x=$cur_x;
			$prev_y=$cur_y;
			continue;
		}
		if($cur_x==$prev_x)
			continue;
		$slope = ($cur_y-$prev_y)/($cur_x-$prev_x);
		$intercept = $cur_y-$slope*$cur_x;
		if(ABS($sp_y-$slope*$sp_x-$intercept)<=$WP_EXACT_MATCH_ERROR)
			$path1=decide_spawn_conflict($path1,$spwn_id);
		$prev_spid=$spwn_id;
		$prev_wp_id=$wp_id;
		$prev_x=$cur_x;
		$prev_y=$cur_y;
	}

//	if($MERGER_debug)
//		parse_debug_report("wp count at step 1 ".$path1['lk']."");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//check to see if we were spawned near an existing spawn (SP to SP check)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	$minx1=$sp_x-$MINIMUM_SPAWN_DISTANCE;
	$maxx1=$sp_x+$MINIMUM_SPAWN_DISTANCE;
	$miny1=$sp_y-$MINIMUM_SPAWN_DISTANCE;
	$maxy1=$sp_y+$MINIMUM_SPAWN_DISTANCE;
	$minz1=$sp_z-$MINIMUM_SPAWN_DISTANCE;
	$maxz1=$sp_z+$MINIMUM_SPAWN_DISTANCE;
	$check_query1="select id from creature_spawns where position_x>='$minx1' and position_x<='$maxx1' and position_y>='$miny1' and position_y<='$maxy1' and position_z>='$minz1' and position_z<='$maxz1' and entry='$cr_entry' and map='$map'";
	$check_res1=mysql_query($check_query1,$dbi) or die(" Q200611231504 ".mysql_error($dbi));	
	while(list($spwn_id)=mysql_fetch_row($check_res1))
		if($spwn_id!=$path1['spid']) //we are already here, do not compare with self
		{
			$path1=decide_spawn_conflict($path1,$spwn_id);
			$already_merged_ids[$spwn_id]=1;
		}
	
//	if($MERGER_debug)
//		parse_debug_report("wp count at step 2 ".$path1['lk']."");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//see if between 2 waypoints are spawns palced on the waypoint line. This happens when we see a moving creature created (WP to SP check)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	$path_res=$path1;	//i wonder if whole content is copied here or only reference
	for($i=0;$i<$path1['lk'];$i++)
		if($path1[$i]['x']!=$path1[$i-1]['x'])
		{
			$slope = ($path1[$i]['y']-$path1[$i-1]['y'])/($path1[$i]['x']-$path1[$i-1]['x']);
			$intercept = $path1[$i]['y']-$slope*$path1[$i]['x'];
			if($path1[$i]['y']>$path1[$i-1]['y'])
			{
				$min_y=$path1[$i-1]['y']-$WP_EXACT_MATCH_ERROR;
				$max_y=$path1[$i]['y']+$WP_EXACT_MATCH_ERROR;
			}
			else
			{
				$min_y=$path1[$i]['y']-$WP_EXACT_MATCH_ERROR;
				$max_y=$path1[$i-1]['y']+$WP_EXACT_MATCH_ERROR;
			}
			if($path1[$i]['x']>$path1[$i-1]['x'])
			{
				$min_x=$path1[$i-1]['x']-$WP_EXACT_MATCH_ERROR;
				$max_x=$path1[$i]['x']+$WP_EXACT_MATCH_ERROR;
			}
			else
			{
				$min_x=$path1[$i]['x']-$WP_EXACT_MATCH_ERROR;
				$max_x=$path1[$i-1]['x']+$WP_EXACT_MATCH_ERROR;
			}
			$check_query2="select distinct(id) from creature_spawns where ABS(position_y-$slope*position_x-$intercept)<=$WP_EXACT_MATCH_ERROR and position_x>=$min_x and position_x<=$max_x and position_y>=$min_y and position_y<=$max_y and entry='$cr_entry' and map='$map' and displayid='$disp' and id!='".$path_res['spid']."'";
	//echo "$check_query2 <br>";
			$check_res2=mysql_query($check_query2,$dbi) or die(" Q2006112315043 ".mysql_error($dbi)."<br>".$check_query2."<br>");	
			while(list($spwn_id)=mysql_fetch_row($check_res2))
				if($already_merged_ids[$spwn_id]!=1 && $spwn_id!=$path_res['spid'])
				{
					//try to merge the 2 paths
					$path_res=decide_spawn_conflict($path_res,$spwn_id);
					$already_merged_ids[$spwn_id]=1;
				}
		}
	$path1=$path_res;//i wonder if whole content is copied here or only reference
//	if($MERGER_debug)
//		parse_debug_report("wp count at step 3 ".$path1['lk']."");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//check intersecting paths and see which one will we keep (WP to WP check)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for($i=0;$i<$path1['lk'];$i++)
	{
		$minx=$path1[$i]['x']-$WP_EXACT_MATCH_ERROR;
		$maxx=$path1[$i]['x']+$WP_EXACT_MATCH_ERROR;
		$miny=$path1[$i]['y']-$WP_EXACT_MATCH_ERROR;
		$maxy=$path1[$i]['y']+$WP_EXACT_MATCH_ERROR;
		$check_query2="select distinct(spawnid),wpcount from creature_waypoints where position_x>='$minx' and position_x<='$maxx' and position_y>='$miny' and position_y<='$maxy' and creature_entry='$cr_entry' and map_id='$map' and forwardskinid='$disp'"; //should return max 1 value
		$check_res2=mysql_query($check_query2,$dbi) or die(" Q20061123150410 ".mysql_error($dbi)."<br>".$check_query2."<br>");	
		while(list($spwn_id,$wpcount)=mysql_fetch_row($check_res2))
			if($already_merged_ids[$spwn_id]!=1 && $spwn_id!=$path_re['spid'])
			{
				//try to merge the 2 paths
				$path_res=decide_spawn_conflict($path_res,$spwn_id);
				$already_merged_ids[$spwn_id]=1;
			}
	}
	$path1=$path_res;//i wonder if whole content is copied here or only reference
	if($MERGER_debug)
		parse_debug_report("wp count at step 4 ".$path1['lk']." and big count is ".$path1['lk']."");
//	$path1=eliminate_repeating_points($path1);
//fclose($inp_h);
	return $path1;
}

//this function examines if a path has more then 1 repeating paths inside of him
function eliminate_repeating_points($path1)
{
	global $MERGER_debug;
	$most_eliminated=0;
	$maxlen=$path1['lk']/2;
	if($maxlen>get_config_var("max_wp_count"))
		$maxlen=get_config_var("max_wp_count");
	for($len=1;$len<=$maxlen;$len++)
		for($slide=0;$slide<$len;$slide++)
		{	
			$t=get_eliminated_pointcount($slide,$len,$path1);
			if($t>$most_eliminated)
			{
				$most_eliminated=$t;
				$best_slide = $slide;
				$best_len = $len;
			}
		}
	if($most_eliminated!=0)
	{
		if($MERGER_debug)
			parse_debug_report("managed to find repetative paterns in path, eliminating wp count $most_eliminated from ".($path1['lk']-1)." (".($path1['biglk']-1).")for spawn ".$path1['spid']);
		$WP_EXACT_MATCH_ERROR=get_config_var("min_wp_dist");
//		$WP_EXACT_MATCH_ERROR=3;
		$iend=(int)(($path1['lk']-$best_slide)/$best_len);
		for($i=0;$i<=$iend;$i++)
		{
			$matchcount=0;
			$start=$best_slide + $best_len*$i;
			$end=$start + $len;
			for($j=$start;$j<$end;$j++)
			{
				$dist_x=ABS($path1[$j+$best_len]['x']-$path1[$j]['x']);
				$dist_y=ABS($path1[$j+$best_len]['y']-$path1[$j]['y']);
				$dist=sqrt($dist_x*$dist_x+$dist_y*$dist_y);
//parse_debug_report("len=$best_len, slide=$best_slide,iend=$iend, start=$start, end=$end,j=$j,j2=".($j+$best_len)." total nodes =".$path1['lk']." dist=$dist x1=".$path1[$j+$best_len]['x']." y1 =".$path1[$j+$best_len]['y']." x2=".$path1[$j]['x']." y2=".$path1[$j]['y']."");
				if($dist<$WP_EXACT_MATCH_ERROR)
					$matchcount++;
			}
			if($matchcount==$len)
				for($j=$start;$j<=$end;$j++)
					if($j<$path1['lk'])
					{
						$path1[$j]['del']=1;
						if($MERGER_debug)
						{
							$dist_x=ABS($path1[$j+$best_len]['x']-$path1[$j]['x']);
							$dist_y=ABS($path1[$j+$best_len]['y']-$path1[$j]['y']);
							$dist=sqrt($dist_x*$dist_x+$dist_y*$dist_y);
							parse_debug_report("node $j matches node ".($j+$best_len)." and distance is $dist");
						}
					}
		}
	}
	return $path1;
}

function get_eliminated_pointcount($slide,$len,$path1)
{
	$WP_EXACT_MATCH_ERROR=get_config_var("min_wp_dist");
//	$WP_EXACT_MATCH_ERROR=3;
	$pointcount=0;
	$iend=(int)(($path1['lk']-$slide)/$len);
	for($i=0;$i<$iend;$i++)
	{
		$matchcount=0;
		$start=$slide + $len*$i;
		$end=$start+$len;
		for($j=$start;$j<=$end;$j++)
		{
			$dist_x=ABS($path1[$j+$len]['x']-$path1[$j]['x']);
			$dist_y=ABS($path1[$j+$len]['y']-$path1[$j]['y']);
			$dist=sqrt($dist_x*$dist_x+$dist_y*$dist_y);
//parse_debug_report("len=$len, slide=$slide,iend=$iend, start=$start, end=$end,j=$j,j2=".($j+$len)." total nodes =".$path1['lk']." dist=$dist x1=".$path1[$j+$len]['x']." y1 =".$path1[$j+$len]['y']." x2=".$path1[$j]['x']." y2=".$path1[$j]['y']."");
			if($dist<$WP_EXACT_MATCH_ERROR)
				$matchcount++;
		}
		if($matchcount==$len)
			$pointcount += $len;
//		$pointcount += $matchcount;
	}
	return $pointcount;
}
?>