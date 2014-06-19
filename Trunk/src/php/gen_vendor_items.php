<?php
set_time_limit ( 2*60*60*10 ); 

$search_min_level = 358;
$search_min_level2 = 358;	//relics list did not match above level :(
$search_min_level_high = 384;
$vendor_entry = 666666;

Parse_link( "http://www.wowhead.com/items=4?filter=qu=3;minle=358;maxle=358", $search_min_level, "" );

function Parse_link($link, $search_min_level, $section_name )
{
	$highest_level_found = 0;
	$filter_highest_N = 3;
	global $item_prices,$vendor_entry;
	$file=@fopen($link,"r");
	if($file)
	{
		$whole_file="";
		while(!feof($file)) 
		{
			$line = fgets($file);
			$whole_file.=$line;

//echo "$line<br>";
			if( strpos("#".$line,"new Listview({template: 'item', id: 'items'") == 0 )
				continue;

//echo "$line<br>";
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
					$pieces2=explode("name:",$val);
					$pieces3=explode(",",$pieces2[1]);
					$name=substr($pieces3[0],1);
				
					$pieces2=explode(",id:",$val);
					$pieces3=explode(",",$pieces2[1]);
					$item_id = $pieces3[0];

					echo "INSERT INTO `vendors` ( entry, item ) VALUES <br>";
					$line2 = "&nbsp&nbsp&nbsp&nbsp( $vendor_entry, '$item_id');";
					echo $line2."<br>";
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
