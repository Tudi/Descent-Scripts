<?php
set_time_limit ( 2*60*60*10 ); 

$search_min_level = 378;
$search_min_level2 = 378;	//relics list did not match above level :(
$search_min_level_high = 384;

$item_prices[397] = 15;
$item_prices[391] = 15;
$item_prices[384] = 15;
$item_prices[379] = 10;
$item_prices[378] = 10;

Parse_link( "http://www.wowhead.com/items=2?filter=ty=15;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Dagger" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=13;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Fist" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=0;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "One-Handed Axe" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=4;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "One-Handed Mace" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=7;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "One-Handed Sword" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=6;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "PoleArm" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=10;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Stave" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=1;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Two-Handed Axe" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=5;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Two-Handed Mace" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=8;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Two-Handed Sword" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=2;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Bow" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=18;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "CrossBow" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=16;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Thrown" );
Parse_link( "http://www.wowhead.com/items=2?filter=ty=19;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Wand" );
Parse_link( "http://www.wowhead.com/items=4.-3?filter=ty=19;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Amulet" );
Parse_link( "http://www.wowhead.com/items=4.-2?filter=ty=19;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Ring" );
Parse_link( "http://www.wowhead.com/items=4.-4?filter=ty=19;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Trinket" );
Parse_link( "http://www.wowhead.com/items=4.-6?filter=ty=19;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Cloack" );
Parse_link( "http://www.wowhead.com/items=4.-5?filter=ty=19;minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Offhand" );
Parse_link( "http://www.wowhead.com/items=4.11?filter=minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Relic" );
Parse_link( "http://www.wowhead.com/items=4.6?filter=minle=$search_min_level;maxle=600#0-2+1", $search_min_level_high, "Shield" );
Parse_link( "http://www.wowhead.com/items=4?filter=ty=19%3Bminle=378%3Bmaxle=600%3Bsl=8#0-2+1", $search_min_level_high, "Feet" );
Parse_link( "http://www.wowhead.com/items=4?filter=ty=19%3Bminle=378%3Bmaxle=600%3Bsl=9#0-2+1", $search_min_level_high, "Wrist" );
Parse_link( "http://www.wowhead.com/items=4?filter=ty=19%3Bminle=378%3Bmaxle=600%3Bsl=6#0-2+1", $search_min_level_high, "Waist" );

function Parse_link($link, $search_min_level, $section_name )
{
	echo "insert  into `rewards`(`id`,`server`,`name`,`item1`,`item2`,`item3`,`item4`,`item5`,`item6`,`item7`,`item8`,`gold`,`price`,`levels`,`talents`,`spell`,`am1`,`am2`,`am3`,`am4`,`am5`,`am6`,`am7`,`am8`) values (649,1,'------------ $section_name --------------',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);<br>";

	$highest_level_found = 0;
	$filter_highest_N = 3;
	global $item_prices;
	$file=@fopen($link,"r");
	if($file)
	{
		$whole_file="";
		while(!feof($file)) 
		{
			$line = fgets($file);
			$whole_file.=$line;

			if( strpos("#".$line,"new Listview({template: 'item', id: 'items'") == 0 )
				continue;

//{"classs":2,"displayid":79861,"dps":994.3,"id":67465,"level":372,"name":"3Vicious Gladiator's Touch of Defeat","reqlevel":86,"slot":15,"slotbak":26,"source":[5],"speed":1.90,"subclass":19,firstseenpatch:40003,cost:[1392495]}
			$line_stripped = str_replace( array( "\"", "'" ),"", $line );

			$pieces=explode("classs:",$line_stripped);
			foreach($pieces as $index => $val)
				if($index>0)
				{
					$pieces2=explode("level:",$val);
					$pieces3=explode(",",$pieces2[1]);
					$min_level=$pieces3[0];
					if( $min_level < $search_min_level )
						continue;
					if( $highest_level_found == 0 )
						$highest_level_found = $min_level;
					if( $min_level != $highest_level_found )
					{
						$filter_highest_N--;
						if( $filter_highest_N == 0 )
							continue;
						$highest_level_found = $min_level;
					}
					$price = $item_prices[ $min_level ];
					
					if( $price == 0 )
						echo "!!!!!!!!!!!!missing price for level $min_level<br>";
					$pieces2=explode("name:",$val);
					$pieces3=explode(",",$pieces2[1]);
					$name=substr($pieces3[0],1);
				
					$pieces2=explode(",id:",$val);
					$pieces3=explode(",",$pieces2[1]);
					$item_id = $pieces3[0];

					echo "INSERT INTO `rewards` (`server`, `name`, `item1`, `price`) VALUES <br>";
					$line2 = "&nbsp&nbsp&nbsp&nbsp('1','$name','$item_id','$price');";
					echo $line2."<br>";
//					echo "update rewards set item1=$item_id where name=\"$name ($section_name)\";<br>";
				}
		}
		fclose($file);
	}
}

function debug_echo($what)
{
//	echo $what;
}
?>
