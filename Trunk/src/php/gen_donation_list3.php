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

$min_accepted_level = 359;
$max_accepted_level = 3590;

/*
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="bg_world";
$dbi = mysql_connect("localhost", "root", "mysql",true) or die("Couldn't connect to database server!");
mysql_select_db("arcemu4_world", $dbi) or die("Q 200603201239"); 
$missing_items="";
$missing_items_count=0;
*/
$key=0;
foreach($class as $key => $class_name )
{
	$link = "http://www.wowhead.com/?itemsets&filter=cl=$key#0-2+1";
	//$link = "item_sets_1.htm";
	echo "---------------------------------------------------------------------------------------------\n<br>";
	debug_echo("#trying to scan sets for class $class_name using link $link<br>");
	//$file=@fopen("item_sets_1.htm","r");
	$file=@fopen($link,"r");
	if($file)
	{

		$s=stream_get_contents($file);
		fclose($file);

$search="var _ = g_items;";
$pos=strpos($s,$search);
$s=substr($s,$pos);

$search="</script>";
$pos=strpos($s,$search);
$s=substr($s,0,$pos);
//$s now contains only the important javascript

$search="new Listview({template: 'itemset', id: 'itemsets', data: [";
$pos=strpos($s,$search);
$s=substr($s,$pos+strlen($search));
//row is like {"classes":[1,4,6],"id":699,"maxlevel":156,"minlevel":156,"name":"2The Twin Blades of Azzinoth","pieces":[32838,32837],"reqclass":41,"type":10}
//start parseing 
$itemsets=explode("},{",$s);

//print_r($itemsets);
//$cis=$itemsets[0];
foreach($itemsets as $cis)
		{
		$itemsets_array=array();
			$cis=$cis.",";//helps the parsing algorithm at the last case
			//parse 1 itemset. first clean anything that might be in the begining
			$search='"';
			$pos=strpos($cis,$search);
//			echo "cis=$cis";
			$cis=substr($cis,$pos);
			//paseing goes like this:
			//1. search for ":" - this delimits name of paramether from the value
			//2. Get the char after the ":" and there are 3 cases
			//a) it's "[" - an array of values are specified
			//b) it's '"' - a string delimitered by "" is specified
			//c) anything else - should be a number until,
//			echo $cis;
			while(strpos($cis,":")!==false)
			{
				$pos=strpos($cis,":");
				$variable_name=substr($cis,0,$pos);
				$variable_name=trim($variable_name,'"');
				$cis=substr($cis,$pos+1);
				if($cis[0]=="[")//case a
				{
					$pos=strpos($cis,"]");
					$variable_value=substr($cis,1,$pos-1);
					$variable_value=explode(",",$variable_value);
					$cis=substr($cis,$pos+2);
				}
				else
				if($cis[0]=='"')//case b
				{
					$pos=strpos($cis,",");
					$variable_value=substr($cis,0,$pos);
					$variable_value=trim($variable_value,'"');
					$cis=substr($cis,$pos+1);
				}
				else
				//case c
				{
					$pos=strpos($cis,",");
					$variable_value=substr($cis,0,$pos);
					$cis=substr($cis,$pos+1);
				}
				$row_name="cis_$variable_name";
				$$row_name=$variable_value;
//				echo "variable_name=$variable_name\n";
//				echo "variable_value=";
//				print_r($variable_value);
//				echo"\n";
				
			}//end of while
			//we parsed the row, now let's populate the variables for the mysql query
			//the available information is:
			//cis_classes, cis_id, cis_maxlevel, cis_minlevel, cis_name, cis_pieces, cis_reqclass, cis_type, 

			//let's put all the available information into one big array
			$array["classes"]=$cis_classes;
			$array["id"]=$cis_id;
			$array["maxlevel"]=$cis_maxlevel;
			$array["minlevel"]=$cis_minlevel;
			$array["name"]=$cis_name;
			$array["pieces"]=$cis_pieces;
			$array["reqclass"]=$cis_reqclass;
			$array["type"]=$cis_type;

			$itemsets_array[]=$array;


			
		}//end of foreach

		//in our big array we have all the itemsets, let's sort it by maxlevel
		$n=count($itemsets_array);
		$g=1;
		while($g==1)
		{
			$g=0;
			for($i=0;$i<$n-1;$i++)
				if($itemsets_array[$i]["maxlevel"]<$itemsets_array[$i+1]["maxlevel"])
				{
					$j=$i+1;
					$g=1;
					$temp=$itemsets_array[$i];
					$itemsets_array[$i]=$itemsets_array[$j];
					$itemsets_array[$j]=$temp;
				}
	
		}

		//let's print out the sql for the best 10
		for($j=0;$j<10;$j++)
//		if( $itemsets_array[$j]["maxlevel"] >= $min_accepted_level && $itemsets_array[$j]["maxlevel"] <= $min_accepted_level )
		{
			//get our variables
			$cis_name=$itemsets_array[$j]["name"];


			$cis_pieces=$itemsets_array[$j]["pieces"];
			//The cis_name contains a number at the begining this line removes it

			$cis_name=substr($cis_name,1);
			$cis_name=mysql_escape_string($cis_name);
//echo "$cis_name=$cis_name<br>\n";

			for($i=1;$i<=8;$i++)
			{
				$item="item$i";
				$$item="0";
			}
			foreach($cis_pieces as $id=>$item_id)
			{
				$item="item".($id+1);//index goes from 0 and items go from 1
				$$item=$item_id;
			}

	echo "INSERT INTO `rewards` (`server`, `name`, `item1`, `item2`, `item3`, `item4`, `item5`, `item6`, `item7`, `item8`, `gold`, `price`, `dao_name`, `) VALUES <br> 	&nbsp&nbsp&nbsp&nbsp('1','$cis_name','$item1','$item2','$item3','$item4','$item5','$item6','$item7','$item8','0','0','none'); <br> ";

		}

	}//end of parse file
	else 
		die(" Q201101041053 ");
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
	echo $what;
}
?>
