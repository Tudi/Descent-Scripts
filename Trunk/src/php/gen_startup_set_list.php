<?php

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

global $dbi,$rescale_required_item_level,$unique_ids;
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="wowbeez_db";
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$item_level_selected=390;	//133-tier 5
$item_level_selected_max=390;	//133-tier 5
$min_pieces = 5;
$max_pieces = 5;
$add_bags[0] = 38145;
$add_bags[1] = 38145;
//$rescale_required_item_level=array(5,10,15,20,25,30,35,40,45,50,55,60);

$class_manual_items[1] = array( );
$class_manual_items[2] = array( );
$class_manual_items[3] = array( );
$class_manual_items[4] = array( );
$class_manual_items[5] = array( );
$class_manual_items[6] = array( );
$class_manual_items[7] = array( );
$class_manual_items[8] = array( );
$class_manual_items[9] = array( );
$class_manual_items[11] = array( );

$unique_ids="";
//update items set subclass=2 where subclass=3 and entry in ();
foreach($class as $class_id => $class_name )
{
	$link = "http://www.wowhead.com/itemsets&filter=qu=4;minle=$item_level_selected;maxle=$item_level_selected_max;cl=$class_id;minpi=$min_pieces;maxpi=$max_pieces";
	debug_echo("#trying to scan sets for class $class_name , link : $link<br>");
	$content = fetch_page( $link );
//echo "$content";break;	
	$lines = explode( "\n", $content );
	$whole_file = $content;
	foreach( $lines as $key => $line )
	{
		if( strpos("#".$line,"Listview({template:") == 0 )
			continue;

		debug_echo("#found list : $line <br>");
//{id:-79,name:'3Conqueror\'s Siegebreaker Battlegear',minlevel:226,maxlevel:226,pieces:[46146,46148,46149,46150,46151],type:4,classes:[1],note:25},{i...
//new Listview({template: 'itemset', id: 'itemsets', data: [{"classes":[1],"elite":false,"flexible":false,"heroic":false,"id":-448,"idbak":909,"maxlevel":390,"minlevel":390,"name":"3Cataclysmic Gladiator's Battlegear","note":37,"pieces":[73482,73481,73480,73479,73478],"raidfinder":false,"reqclass":1,"type":4},{"classes":[1],"elite":true,"flexible":false,"heroic":false,"id":-432,"idbak":909,"maxlevel":390,"minlevel":390,"name":"3Cataclysmic Gladiator's Battlegear","note":37,"pieces":[73655,73654,73653,73652,73651],"raidfinder":false,"reqclass":1,"type":4}]}); 
		$line = str_replace("\"","",$line );
		$pieces=explode("pieces:[",$line);
		$bag_counter=0;
		foreach($pieces as $index => $val)
			if($index>0)
			{
				$pieces2=explode("]",$val);
				$id_list=$pieces2[0];
				debug_echo("#found ids $id_list<br>");
				gen_playercreate_sqls($id_list,$class_id,$add_bags[$bag_counter],$bag_counter);
				$bag_counter++;
				$setcounter++;
			}
	}
}

$missing_items="";
$missing_items_count=0;
debug_echo("Need to rescan these : <br>");
for($i=0;$i<$missing_items_count;$i++)
	debug_echo("{$missing_items[$i]}<br>");
debug_echo("all ids : <br>");
$id_list = "";
foreach($unique_ids as $key => $value )
{
	debug_echo("$key,");
	$id_list.="$key,";
}
//convert plate to mail and mail to leather so shamans can wear them from the start
$id_list = substr($id_list,0,-1);
debug_echo("<BR> update items set subclass=2 where subclass=3 and entry in ($id_list);");
debug_echo("<br> update items set subclass=3 where subclass=4 and entry in ($id_list);");


function debug_echo($what)
{
//	echo $what;
}

function gen_playercreate_sqls($id_list,$selected_class,$add_bag,$bag_counter)
{
	global $dbi,$rescale_required_item_level,$unique_ids;
	echo "Delete from `playercreateinfo_items` where protoid in ($id_list);<br>";
	$get_query = "SELECT `index` FROM playercreateinfo WHERE `class`='$selected_class'"; //maybe we get trainers that we did not know before ?
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$add_items = explode(",",$id_list);
	while(list($t_index)=mysql_fetch_row($get_res))
	{
		if( $add_bag )
		{
			echo "Delete from `playercreateinfo_items` where protoid=$add_bag and slotid=19+$bag_counter and `indexid`=$t_index;<br>";
			echo "replace INTO `playercreateinfo_items` (`indexid`, `protoid`, `slotid`, `amount`) 	VALUES('$t_index','$add_bag',19+$bag_counter,'1');<br>";
		}
		foreach($add_items as $key => $value)
		{
			if( $rescale_required_item_level[$key] )
				echo "update items set requiredlevel=".$rescale_required_item_level[$key]." where entry=$value and requiredlevel>".$rescale_required_item_level[$key].";<br>";
			$unique_ids[$value]=1;

			$get_query1 = "SELECT inventorytype FROM items WHERE entry=$value"; //maybe we get trainers that we did not know before ?
			$get_res1=mysql_query($get_query1,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
			list($invt)=mysql_fetch_row($get_res1);
			
			echo "replace INTO `playercreateinfo_items` (`indexid`, `protoid`, `slotid`, `amount`) 	VALUES('$t_index','$value','$invt','1');<br>";
		}
	}
}
function fetch_page( $page )
{
	$ch = curl_init($page);
	curl_setopt ($ch, CURLOPT_AUTOREFERER, FALSE); 
	curl_setopt ($ch, CURLOPT_SSL_VERIFYPEER, FALSE); 
	curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, FALSE); 
	curl_setopt($ch,CURLOPT_HTTPHEADER,array (
        "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/534.10 (KHTML, like Gecko) Chrome/8.0.552.215 Safari/534.10",
		"Accept-Language: en-US,en;q=0.8",
		"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3",
		"Cookie: __utmz=186410998.1294997898.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); __qca=P0-2061096298-1294997898413; __gads=ID=e442c96b7525ae24:T=1294998662:S=ALNI_MZLnMdNQRhhhw-RhRSrQQlBhpDQLg; enabledTest; __utma=186410998.2105000444.1294997898.1294997898.1294997898.1; __utmc=186410998; __utmb=186410998.4.9.1294997982710"
    ));
	curl_setopt($ch, CURLOPT_HEADER, 0);
	curl_setopt($ch,CURLOPT_RETURNTRANSFER,TRUE);
	$s = curl_exec($ch); 
	curl_close($ch); 

	return $s;
}
?>
