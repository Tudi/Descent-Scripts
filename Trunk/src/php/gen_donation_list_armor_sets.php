<?php

//$search_name="Wrathful";
$search_min_level = 365;

$class[1]="Warrior";
$class[2]="Paladin";
$class[3]="Hunter";
$class[4]="Rogue";
$class[5]="Priest";
$class[6]="Death Knight";
$class[7]="Shaman";
$class[8]="Mage";
$class[9]="Warlock";
$class[11]="Druid";

/*
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="bg_world";
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); 
$missing_items="";
$missing_items_count=0;
*/

foreach($class as $key => $class_name )
{
	$link = "http://www.wowhead.com/?itemsets&filter=cl=$key#0-2+1";
	debug_echo("#trying to scan sets for class $class_name using link $link<br>");
	$file=@fopen($link,"r");
	if($file)
	{
		$whole_file="";
		while(!feof($file)) 
		{
			$line = fgets($file);
			$whole_file.=$line;

//			if( strpos("#".$line,$search_for_set) == 0 )
			if( strpos("#".$line,"new Listview({template: 'itemset', id: 'itemsets'") == 0 )
				continue;

//{id:-79,name:'3Conqueror\'s Siegebreaker Battlegear',minlevel:226,maxlevel:226,pieces:[46146,46148,46149,46150,46151],type:4,classes:[1],note:25},{i...
			$line_stripped = str_replace( array( "\"", "'" ),"", $line );

			$pieces=explode("id:",$line_stripped);
//			$set_ids="";
			$set_names="";
			$set_pieces="";
			$setcounter=0;
			foreach($pieces as $index => $val)
				if($index>0)
				{
					$pieces2=explode("minlevel:",$val);
					$pieces3=explode(",",$pieces2[1]);
					$min_level=$pieces3[0];
					if( $min_level < $search_min_level )
						continue;
					
					$pieces2=explode("name:",$val);
					$pieces3=explode(",",$pieces2[1]);
//					$set_names[$setcounter]=$pieces3[0];
					$set_names[$setcounter]=substr($pieces3[0],1);
//					if( strpos("#".$set_names[$setcounter],$search_for_set) == 0 )
	//					continue;
	
//					$pieces2=explode("id:",$val);
//					$set_ids[$setcounter]=$pieces2[0];
					
					$pieces2=explode("pieces:[",$val);
					$pieces3=explode("]",$pieces2[1]);
					$set_pieces[$setcounter]=$pieces3[0];
					debug_echo("#found set id = {$set_ids[$setcounter]}, name = {$set_names[$setcounter]}, ids= {$set_pieces[$setcounter]}<br>");
					$setcounter++;
				}

			if( $setcounter != 0 )
			{
				echo "INSERT INTO `rewards` (`server`, `name`, `item1`, `item2`, `item3`, `item4`, `item5`, `item6`, `item7`, `item8`, `gold`, `price`, `dao_name`) VALUES <br> 
	&nbsp&nbsp&nbsp&nbsp('1','------------  $class_name  --------------','0','0','0','0','0','0','0','0','0','0','none'); <br>";
			}
			for($i=0;$i<$setcounter;$i++)
			{
				$pieces2=explode(",",$set_pieces[$i]);
/*				$has_all_pieces=1;
				//chek if we have all items in db
				foreach($pieces2 as $key => $val)
				{
					$get_query = "select count(*) from items where entry=$val ";
					$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
					$row=mysql_fetch_row($get_res);
					if( $row[0]==0 )
					{
						$has_all_pieces=0;
						$missing_items[$missing_items_count++]=$val;
						debug_echo("#missing item ID = $val for set {$set_names[$i]}<br>");
					}
				} 
				if($has_all_pieces==1) */
				{
					echo "INSERT INTO `rewards` (`server`, `name`, `item1`, `item2`, `item3`, `item4`, `item5`, `item6`, `item7`, `item8`, `gold`, `price`, `dao_name`) VALUES <br>";
					$line2 = "&nbsp&nbsp&nbsp&nbsp('1','{$set_names[$i]}',";
					$added_count=0;
					foreach($pieces2 as $key => $val)
					{
						$added_count++;
						$line2.="'$val',";
					}
					for(;$added_count<8+1;$added_count++)
						$line2.="'0',";
					$line2.="'99','none');";
					echo $line2."<br>";
				}
			}
		}
		fclose($file);
	}
}
/*
debug_echo("Need to rescan these : <br>");
for($i=0;$i<$missing_items_count;$i++)
	debug_echo("{$missing_items[$i]}<br>");
for($i=0;$i<$missing_items_count;$i++)
	debug_echo("{$missing_items[$i]},");
*/
function debug_echo($what)
{
//	echo $what;
}
?>
