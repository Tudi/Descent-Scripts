<?php
ini_set("memory_limit","160M");
set_time_limit( 30*60 );
	
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wowbeez_db";

//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

 $drop_type=1;
 
	$fout = fopen("out.sql","wt");
	$fout_clean = fopen("out_clean.sql","wt");

	process_link( );
	
	fclose( $fout );
	fclose( $fout_clean );

function process_link( )
{
	$drop_type = 1;
	$link = "http://www.wowhead.com/spell=31252/prospecting";
	$file=@fopen($link,"r");
	if($file)
	{
//echo "have file<br>";		
		while(!feof($file)) 
		{
			$line = fgets($file);
//echo $line;				
			if( strpos("#".$line,"{template: 'item', id: 'prospected-from'") == 0 )
				continue;
			
//new Listview({template: 'item', id: 'prospected-from', name: LANG.tab_prospectedfrom, tabs: tabsRelated, parent: 'lkljbjkb574', hiddenCols: ['side', 'slot', 'source', 'type', 'level', 'reqlevel'], extraCols: [Listview.extraCols.yield, Listview.extraCols.reqskill, Listview.extraCols.count, Listview.extraCols.percent], sort:['reqskill', '-percent', 'name'], computeDataFunc: Listview.funcBox.initLootTable, data: [{"classs":7,"id":52185,"level":83,"name":"6Elementium Ore","reqlevel":75,"slot":0,"source":[2,3],"sourcemore":[{"p":3}],"subclass":7,count:381739,outof:2102071,yield:52182,reqskill:475},{"classs":7,"id":52185,"level":83,"name":"6Elementium Ore","reqlevel":75,"slot":0,"source":[2,3],"sourcemore":[{"p":3}],"subclass":7,count:381375,outof:2102071,yield:52181,reqskill:475},{"classs":7,"id":52185,"level":83,"name":"6Elementium Ore","reqlevel":75,"slot":0,"source":[2,3],"sourcemore":
//data: [{"classs":7,"id":52185,"level":83,"name":"6Elementium Ore","reqlevel":75,"slot":0,"source":[2,3],"sourcemore":[{"p":3}],"subclass":7,count:381739,outof:2102071,yield:52182,reqskill:475},
			$line = str_replace(array("}",")",";","\""),"",$line);
//echo $line."<br>";				
			$pieces=explode(",id:",$line);
//print_r( $pieces );				
//echo "<br>";
			$ind = 0;
			foreach($pieces as $index => $val)
			{
//echo $val."<br>";									
				$pieces2=explode(",",$val);
				$itemid=$pieces2[0];
//echo "$itemid<br>";				
				if( $itemid <= 0 )
					continue;
					
				$pieces2=explode("count:",$val);
				$pieces2=explode(",",$pieces2[1]);
				$number_of_drops=$pieces2[0];
				
				$pieces2=explode("outof:",$val);
				$pieces2=explode(",",$pieces2[1]);
				$total_loots=$pieces2[0];
				
				$chance = $number_of_drops * 100 / ($total_loots + 1);
				if( $chance < 1 )
					$chance = 1;

				$pieces2=explode("yield:",$val);
				$pieces2=explode(",",$pieces2[1]);
				$yields=$pieces2[0];
				
				echo "replace into loot_prospecting ( entryid, itemid, percentchance, mincount, maxcount ) values ( $itemid, $yields, $chance, 1, 2 );<br>";
			}

		}
		fclose($file);
	}
}

?>