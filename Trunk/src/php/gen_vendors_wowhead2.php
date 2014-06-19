<?php
set_time_limit( 30*60 );

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

	$fout = fopen("vendors_out.sql","wt");
//	$vendor_entries="19065,18822,5049,34093,23483,46555";
	$vendor_entries="58155";
	$vendors_vect = explode( ",", $vendor_entries );
	asort( $vendors_vect );
	foreach( $vendors_vect as $key => $t_entry )
		if( $t_entry )
			process_file( $t_entry );
	fclose( $fout );
	
function process_file( $t_entry )
{
	global $dbi,$fout;
	$new_line = "<br>";
	$new_line = "\n";	
	$UNIT_NPC_FLAG_GOSSIP = 1;
	$UNIT_NPC_FLAG_VENDOR			= 0x00000080;
	
		$link = "./wowhead npc/npc=".$t_entry.".htm";
//echo " Getting information for npc: ".$t_entry." from wowhead : $link <br> ";
		$file=@fopen($link,"r");
		if($file)
		{
//echo "have file<br>";		
			while(!feof($file)) 
			{
				$line = fgets($file);
//echo $line;				
				if( strpos("#".$line,"template: 'item', id: 'sells'") == 0 )
					continue; //this is eighter not the row we are looking for or this npc is not a trainer at all 
				
				$output = "";
//new Listview({template: 'item', id: 'sells', name: LANG.tab_sells, tabs: tabsRelated, parent: 'lkljbjkb574', extraCols: [Listview.extraCols.cost], note: $WH.sprintf(LANG.lvnote_filterresults, '/items?filter=cr=129;crs=0;crv=32509'), data: [{"classs":2,"displayid":23248,"dps":120.0,"id":42944,"level":1,"name":"0Balanced Heartseeker","slot":13,"slotbak":13,"source":[5],"speed":1.70,"subclass":15,react:[1,1],stack:[1],avail:-1,cost:[0,[[395,2175]],[]]},{"classs":2,"displayid":23904,"dps":156.0,"id":42943,"level":1,"name":"0Bloodied Arcanite Reaper","slot":17,"slotbak":17,"source":					
//{"classs":2,"displayid":23248,"dps":120.0,"id":42944,"level":1,"name":"0Balanced Heartseeker","slot":13,"slotbak":13,"source":[5],"speed":1.70,"subclass":15,react:[1,1],stack:[1],avail:-1,cost:[0,[[395,2175]],[]]}
				$line = str_replace(array("}",")",";","\""),"",$line);
//echo $line."<br>";				
				$pieces=explode(",id:",$line);
//print_r( $pieces );				
//echo "<br>";
				foreach($pieces as $index => $val)
					{
//echo $val."<br>";									
						$pieces2=explode(",",$val);
						$itemid=$pieces2[0];
						if( $itemid <= 0 )
							continue;

						$pieces2=explode("stack:[",$val);
						$pieces2=explode("]",$pieces2[1]);
						$stack = $pieces2[0];

						$pieces2=explode("avail:",$val);
						$pieces2=explode(",",$pieces2[1]);
						$max_am = $pieces2[0];

						if( $max_am > 0 )
							$inc_time = 30*60;
						else
							$inc_time = 0;
				
						//reset to make sure we are not inheriting values from previous vendor
						$gold_cost = 0;
						$pieces2=explode("cost:",$val);
						$cost_list = get_cost_list( $pieces2[1] );
//echo "list str : ${pieces2[1]}<br>";
//print_r( $cost_list );
//echo "<br>";
						$gold_cost = $cost_list[0][0][0];
						$virtual_type_1 = $cost_list[1][0][0];
						$virtual_type_1_count = $cost_list[1][1][0];
						$virtual_type_2 = $cost_list[1][2][0];
						$virtual_type_2_count = $cost_list[1][3][0];
						
						if( $gold_cost <= 0 )
							$gold_cost = 0;

						$item_as_str = "";
						$item_count_as_str = "";
						$currency_as_str = "";
						$currency_count_as_str = "";
						$item_sum = 0;
						$item_count_sum = 0;
						$currency_sum = 0;
						$currency_count_sum = 0;						
						for( $i=0;$i<5;$i++)
						{
							$item_cost[$i] = $cost_list[2][$i*2][0];
							$item_cost_count[$i] = $cost_list[2][$i*2+1][0];
//echo "item cost : ${item_cost[$i]}<br>";						
							if( $item_cost[$i] > 0 )
							{
								$item_as_str .= $item_cost[$i].",";
								$item_sum += $item_cost[$i];
							}
							if( $item_cost_count[$i] > 0 )
							{
								$item_count_as_str .= $item_cost_count[$i].",";
								$item_count_sum += $item_cost_count[$i];
							}
						}
						//idiot wowhead does not use the last 2 value precision
						//the issue is that currency is actually multiplied by 100 in dbc but not all values match exactly (ex 99 ) so we need to loose some precision
						if( $virtual_type_1_count >= 100 )
						{
							$currency_precision_lost = 100;
							$currency_precision_gain = 1;
						}
						else if( $virtual_type_1_count >= 10 )
						{
							$currency_precision_lost = 10;
							$currency_precision_gain = 10;
						}
						else
						{
							$currency_precision_lost = 1;
							$currency_precision_gain = 100;
						}
						//aaaah the special cases for backward compatibility
						if( $virtual_type_1 == 61 || $virtual_type_1 == 81 || $virtual_type_2 == 61 || $virtual_type_2 == 81 
							|| $virtual_type_1 == 241 || $virtual_type_2 == 241
							|| $virtual_type_1 == 402 || $virtual_type_2 == 402
							|| $virtual_type_1 == 361 || $virtual_type_2 == 361
							|| $virtual_type_1 == 391 || $virtual_type_2 == 391
						)
						{
							$currency_precision_lost = 1;
							$currency_precision_gain = 1;
						}
						
						if( $virtual_type_1 > 0 )
						{
							$currency_as_str .= $virtual_type_1.",";
							$currency_sum += $virtual_type_1;
						}
						if( $virtual_type_1_count > 0 )
						{
							$currency_count_as_str .= ($virtual_type_1_count*$currency_precision_gain).",";
							$currency_count_sum += ($virtual_type_1_count*$currency_precision_gain);
						}
						if( $virtual_type_2 > 0 )
						{
							$currency_as_str .= $virtual_type_2.",";
							$currency_sum += $virtual_type_2;
						}
						if( $virtual_type_2_count > 0 )
						{
							$currency_count_as_str .= ($virtual_type_2_count*$currency_precision_gain).",";
							$currency_count_sum += ($virtual_type_2_count*$currency_precision_gain);
						}
//echo "#found item $itemid cost gold $gold_cost , item cost in ($item_as_str) and currency in ($currency_as_str)<br>";
//echo "$query <br>";
						$extended_entry = 0;
						if( $currency_as_str != "" || $item_as_str!="" )
						{
							$item_as_str .= "0";
							$item_count_as_str .= "0";
							$currency_as_str .= "0";
							$currency_count_as_str .= "0";
							$query = "select entry from `itemextendedcostcsv` where 
							item_1 in ($item_as_str) and item_2 in ($item_as_str) and item_3 in ($item_as_str) and item_4 in ($item_as_str) and item_5 in ($item_as_str)
							and item_count_1 in ($item_count_as_str) and item_count_2 in ($item_count_as_str) and item_count_3 in ($item_count_as_str) and item_count_4 in ($item_count_as_str) and item_count_5 in ($item_count_as_str)
							and currency_1 in ($currency_as_str) and currency_2 in ($currency_as_str)and currency_3 in ($currency_as_str)and currency_4 in ($currency_as_str)and currency_5 in ($currency_as_str)
							and (item_1+item_2+item_3+item_4+item_5)=$item_sum 
							and (item_count_1+item_count_2+item_count_3+item_count_4+item_count_5)=$item_count_sum 
							and (currency_1+currency_2+currency_3+currency_4+currency_5)=$currency_sum 
							";
							//fucking wowhead outdated honor costs
							if( $virtual_type_1 == 392 || $virtual_type_2 == 392 )
								$query .= "and FLOOR((currency_1_count+currency_2_count+currency_3_count+currency_4_count+currency_5_count)/$currency_precision_lost)>=$currency_count_sum 
											order by (currency_1_count+currency_2_count+currency_3_count+currency_4_count+currency_5_count) asc limit 0,1";
							else
								$query .= "and FLOOR((currency_1_count+currency_2_count+currency_3_count+currency_4_count+currency_5_count)/$currency_precision_lost)=$currency_count_sum 
									      and FLOOR(currency_1_count/$currency_precision_lost) in ($currency_count_as_str) and FLOOR(currency_2_count/$currency_precision_lost in ($currency_count_as_str)and currency_3_count/$currency_precision_lost) in ($currency_count_as_str)and FLOOR(currency_4_count/$currency_precision_lost) in ($currency_count_as_str)and FLOOR(currency_5_count/$currency_precision_lost) in ($currency_count_as_str)
								";
//echo "$query<br>";						
							$get_res1=mysql_query($query,$dbi) or die(" Q200901211542 ".mysql_error($dbi)." $query<br>");
							list($extended_entry)=mysql_fetch_row($get_res1);
							if( $extended_entry== "" )
							{
echo "list str : ${pieces2[1]}<br>";
print_r( $cost_list );
echo "<br>";
								echo "#!!warning, could not get extended cost entry for vendor $t_entry : item=$itemid, item cost in ($item_as_str) and currency in ($currency_as_str), query = $query<br>";
								continue;
							}
						}
						$output .= "REPLACE into vendors (entry,item,amount,max_amount,inctime,extended_cost) values ($t_entry,$itemid,$stack,$max_am,$inc_time,$extended_entry);$new_line";
					}

			}
			fclose($file);
			if( strlen( $output ) > 10 )
			{
				//cataclysm vendor extended costs have been remade. We need to dump old vendors and remake them :(
				$output = "delete from vendors where entry=$t_entry; $new_line ".$output;
				$output = "Update creature_proto set NpcFlags= NpcFlags | $UNIT_NPC_FLAG_VENDOR where entry = $t_entry; $new_line ".$output;
//				echo $output;
				fputs( $fout, $output);
			}
//			die();
		}
//		else
	//		die( "Could not find vendor $t_entry $new_line");
}	
	/**/
function get_cost_list( $in )
{
	$depth = 0;
	$index = 0;
	$length = strlen( $in );
	for( $i=0;$i<$length;$i++)
	{
		if( $in[$i] == '[' )
		{
			$value_start=$i+1;
			if( $depth == 1 )
			{
				$index++;
				$index2=0;
				$ret_counter[$index][$index2] = 0;
			}
			$depth++;
			if( $ret_counter[$index][$index2] <= 0 )
				$ret_counter[$index][$index2] = 0;
		}
		else if( $in[$i] == ']' )
		{
			$value_end = $i;
			if( $value_end > $value_start )
			{
				$value = substr( $in, $value_start, $value_end-$value_start );
				$ret[$index][$index2][$ret_counter[$index][$index2]] = $value;
				$ret_counter[$index][$index2]++;
			}
			$depth--;			
			$value_start=$i+1;
		}
		else if( $in[$i] == ',' )
		{
			$value_end = $i;
			if( $value_end > $value_start )
			{
				$value = substr( $in, $value_start, $value_end-$value_start );
				$ret[$index][$index2][$ret_counter[$index][$index2]] = $value;
				$ret_counter[$index][$index2]++;
			}
			$index2++;
			$ret_counter[$index][$index2] = 0;
			$value_start=$i+1;
		}
			
		if( $depth == 0 )
			break;
	}
	return $ret;
}
?>