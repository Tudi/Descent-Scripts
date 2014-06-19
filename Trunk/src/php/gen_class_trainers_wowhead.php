<?php
set_time_limit (3000);

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$trainer_entry = 101000;

echo "replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+0,'Warrior Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry +1,'Paladin Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+2,'Hunter Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+3,'Rogue Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+4,'Priest Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+5,'DeathKnight Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+6,'Shaman Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+7,'Mage Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+8,'Warlock Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
replace into creature_names (entry,name,subname,info_str,type,male_displayid,female_displayid,male_displayid2,female_displayid2) values ( $trainer_entry+9,'Druid Trainer','Let me teach you a little something','Trainer',7,4842,7274,15523,17375);<br>
<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+0,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+1,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+2,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+3,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+4,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+5,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+6,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+7,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+8,90,90,35,100,100,1,1+2+16+32,1);<br>
replace into creature_proto (entry,minlevel, maxlevel, faction, minhealth, maxhealth,scale,npcflags,respawntime) values ( $trainer_entry+9,90,90,35,100,100,1,1+2+16+32,1);<br>
delete from trainer_spells where entry in ($trainer_entry,$trainer_entry+1,$trainer_entry+2,$trainer_entry+3,$trainer_entry+4,$trainer_entry+5,$trainer_entry+6,$trainer_entry+7,$trainer_entry+8,$trainer_entry+9);<br>
";

$classes = array(1,2,3,4,5,6,7,8,9,11);				
//$classes = array(2);				
foreach( $classes as $key => $class)
{
	$spell_list = fetch_wowhead_spell_list( $class );
	echo "insert into `trainer_defs` (`entry`, `req_class`, `trainer_type`, `trainer_ui_window_message`, `can_train_gossip_textid`, `cannot_train_gossip_textid`) values($trainer_entry,'$class','0','Hello, \$c! Ready for some training?','1040','5721');<br>";
	foreach($spell_list as $key => $spell_id)
	{
		//get spell base_level = req_level
		$get_query2 = "select baselevel,id,name,description from dbc_spell where id=$spell_id";
		$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." <br>'$get_query2'<br> ");
		$row2=mysql_fetch_row($get_res2);
		$req_level = $row2[0];
//echo "{$row2[1]} = {$row2[2]} = {$row2[3]} = {$row2[4]} = {$row2[5]} <br>";

		//get required_spell
		$get_query2 = "select col_2 from skilllineabilitydbccsv where col_8=$spell_id";
		$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)."  <br>'$get_query2'<br> ");
		$row2=mysql_fetch_row($get_res2);
		$req_spell = $row2[0];
		if( $req_spell == "" )
			$req_spell = 0;

		//spell cost
		$spell_cost = $req_level * 3000;

		if( $class == 1 || $class == 4 )
			$delete_spell = $req_spell;
		else 
			$delete_spell = 0;

		$sql =  "insert into trainer_spells (entry,learn_spell,reqlevel,reqspell,deletespell,spellcost ) values ( $trainer_entry, $spell_id, $req_level, $req_spell, $delete_spell, $spell_cost);";
		echo "$sql<BR>";
	}
	$trainer_entry++;
}
/////////////////////////////////////////////////////
function fetch_wowhead_spell_list( $class )
{
	$ret="";$retc=0;
	$pagename = "http://www.wowhead.com/?spells=7.".$class;
#echo "!!!!!! $pagename <br>";
	$file=@fopen($pagename,"r");
	if($file)
	{
		while(!feof($file)) 
		{
			$line = fgets($file);
			if( strpos("#".$line,"class=\"listview-void\">") )
			{
				$line=str_replace("\"","",$line);
				$parts=explode("spell=",$line);
				//print_r($parts);
				foreach($parts as $key => $value )
				if( $key>0 )
				{
					$subpart=explode(">",$value);
					$ret[$retc++] = $subpart[0];
				}
				break;
			}
		}
		fclose( $file );
	}
	return $ret;
}
?>