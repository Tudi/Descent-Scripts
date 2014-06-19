<?php

$search_for_set="Salvation";
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

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="test311_world";
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$missing_items="";
$missing_items_count=0;
echo "INSERT INTO `rewards` (`server`, `name`, `item1`, `item2`, `item3`, `item4`, `item5`, `item6`, `item7`, `item8`, `gold`, `price`, `am1`, `am2`, `am3`, `am4`, `am5`, `am6`, `am7`, `am8`, `dao_name`, `item9`, `item10`, `item11`, `item12`, `item13`, `item14`, `item15`, `item16`, `item17`, `item18`, `item19`, `item20`) VALUES <br> 
	('1','------------Season 6--------------','0','0','0','0','0','0','0','0','0','0','1','1','1','1','1','1','1','1','none','0','0','0','0','0','0','0','0','0','0','0','0'); <br>";
foreach($class as $key => $class_name )
{
	debug_echo("#trying to scan sets for class $class_name <br>");
//	$file=@fopen("http://www.wowhead.com/?itemsets&filter=cl=$key#0-2+1","r");
	$file=@fopen("http://www.wowhead.com/?items&filter=na=$search_for_set;ub=$key#0-2+1","r");
	$item_class_subclass_to_item_entry="";
	if($file)
	{
		$whole_file="";
		while(!feof($file)) 
		{
			$line = fgets($file);
			$whole_file.=$line;

			if( strpos("#".$line,"new Listview({template:") == 0 )
				continue;

//new Listview({template: 'item', id: 'items', data: [{id:32451,name:'3Gladiator\'s Salvation',level:123,reqlevel:70,dps:41.4,speed:1.60,slot:21,classs:2,subclass:4,displayid:44858,slotbak:21},{id:32964,name:'3Merciless Gladiator\'s ............
			$id_pieces=explode("{id:",$line);
			$class_pieces=explode("classs:",$line);
			$subclass_pieces=explode("subclass:",$line);
			$slot_pieces=explode("slot:",$line);
			foreach($id_pieces as $key => $value)
			{
				$justid = explode(",",$id_pieces[$key]);
				$justid = (int)$justid[0];
debug_echo("Inspecting id $justid from piece ${id_pieces[$key]}<br>");
				if( $justid < 1 )
				{
					debug_echo("Not suitable for usage<br>");
					continue;	//probably garbage
				}
				$justclass = explode(",",$class_pieces[$key]);
				$justclass = (int)($justclass[0]);
debug_echo("Inspecting justclass $justclass from piece ${class_pieces[$key]}<br>");
				$justsubclass = explode(",",$subclass_pieces[$key]);
				$justsubclass = (int)($justsubclass[0]);
debug_echo("Inspecting justsubclass $justsubclass from piece ${subclass_pieces[$key]}<br>");
				$justslot = explode(",",$slot_pieces[$key]);
				$justslot = (int)($justslot[0]);
debug_echo("Inspecting justslot $justslot from piece ${slot_pieces[$key]}<br>");
				//we already have a stronger item for this class / subclass so we skip this one
				$mykey = $justclass + $justsubclass*100;
//				$mykey = $justslot;
debug_echo("Inspecting key value : $mykey<br>");
debug_echo("Inspecting saved value : ".$item_class_subclass_to_item_entry[$mykey]."<br>");
				if( $item_class_subclass_to_item_entry[$mykey] > 0 )
				{
debug_echo("Already have value for this slot : ".$item_class_subclass_to_item_entry[$mykey]." instead $justid <br>");
					continue;
				}
				$item_class_subclass_to_item_entry[$mykey] = $justid;
			}
		}
	}
	debug_echo("List of ids that we could use : ");
	foreach($item_class_subclass_to_item_entry as $key => $value )
		debug_echo("$value,");
	debug_echo("<br>");

	echo "INSERT INTO `rewards` (`server`, `name`, `item1`, `item2`, `item3`, `item4`, `item5`, `item6`, `item7`, `item8`, `gold`, `price`, `am1`, `am2`, `am3`, `am4`, `am5`, `am6`, `am7`, `am8`, `dao_name`, `item9`, `item10`, `item11`, `item12`, `item13`, `item14`, `item15`, `item16`, `item17`, `item18`, `item19`, `item20`) VALUES <br>";
	$line2 = "('1','$class_name $search_for_set +".count($item_class_subclass_to_item_entry)." pieces,";
	$added_count=0;
	foreach($item_class_subclass_to_item_entry as $key => $val)
	{
		$added_count++;
		$line2.="'$val',";
	}
	for(;$added_count<8+1;$added_count++)
		$line2.="'0',";
	$line2.="'99','1','1','1','1','1','1','1','1','none','0','0','0','0','0','0','0','0','0','0','0','0');";
	echo $line2."<br>";
}

function debug_echo($what)
{
//	echo $what;
}
?>
