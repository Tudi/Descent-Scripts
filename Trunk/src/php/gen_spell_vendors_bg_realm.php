<?php

$VENDOR_FIRST_ENTRY = $VENDOR_FIRST_ENTRY2 = 102000;
$ITEMS_PER_VENDOR = 119;
$vendor_look=array(20761,20705,29054,17374,18482,4532,1355,16203,17372,1859,3808,15516,18222,1882,4209,16610,3346,3798,1878,18324,11900,1728);
$vendor_look_count=22;
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$inf=fopen("SkillLineAbility.dbc.csv","r");
$line = fgets($inf);
$entry_list="";
$selected_skillines="26,256,257,594,267,184,50,163,51,253,38,39,633,613,78,56,770,771,772,776,375,373,374,237,8,6,355,354,593,574,134,573";
while($inf && !feof($inf)) 
{
   	$line = fgets($inf);

	//check requirements
	$pieces=explode(",",$line);
	$skilline=$pieces[ 1 ];

	if( strpos("#".$selected_skillines,$skilline)==0 )
		continue;

	$spell = $pieces[2];
	if( (int)($spell) > 0)
	{
		$get_query = "select Reagent_1,Reagent_2,Reagent_3,Reagent_4,Reagent_5,Reagent_6,Reagent_7,Reagent_8 from dbc_spell where id = $spell ";
		$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		$row=mysql_fetch_row($get_res);
		for($i=0;$i<8;$i++)
			if( $row[$i] > 0 )
				$entry_list[$row[$i]]=1;
	}
}
fclose($inf);

if( $dbi)
	mysql_close($dbi);

echo "delete from vendors where entry=$VENDOR_FIRST_ENTRY;<br>";
echo "replace into creature_names (entry,name,subname,info_str,type,male_displayid) values ( $VENDOR_FIRST_ENTRY,'Spell reagent vendor','','Buy',7,20761);<br>";
echo "replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $VENDOR_FIRST_ENTRY,90,90,35,100,100,1,129+4096,1);<br>";
$price_query="update items set buyprice=10,maxcount=35000 where entry in (";
foreach($entry_list as $key => $value)
{
	echo "insert into vendors (entry,item,amount) values ($VENDOR_FIRST_ENTRY,$key,10); <br>";
	$price_query.=$key.",";
}
echo "insert into vendors (entry,item,amount) values ($VENDOR_FIRST_ENTRY,5178,1); <br>";
echo "insert into vendors (entry,item,amount) values ($VENDOR_FIRST_ENTRY,5175,1); <br>";
echo "insert into vendors (entry,item,amount) values ($VENDOR_FIRST_ENTRY,5176,1); <br>";
echo "insert into vendors (entry,item,amount) values ($VENDOR_FIRST_ENTRY,5177,1); <br>";
$price_query = substr($price_query,0,-1).");<br>";
echo $price_query;
?>