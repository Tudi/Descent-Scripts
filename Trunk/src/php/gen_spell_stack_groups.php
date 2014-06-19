<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); 

//each group contains a list of spells
//each spell can have multiple groups
//groups can contain subgroups
$get_query = "SELECT id, spell_id FROM spell_group";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
$spell_groups_counter = 0;
$spell_groups_counter = 0;
while($row=mysql_fetch_row($get_res) )
{
	$group_id = $row[0];
	$spell_id = $row[1];
	$groups_mask[ $group_id ] = 1; //default value, will overwrite it
	if( $spell_id > 0 )
	{
		$spell_groups[ $spell_groups_counter ]['s'] = $spell_id;
		$spell_groups[ $spell_groups_counter ]['g'] = $group_id;
		$spell_groups_counter++;
	}
	else
	{
		$link_to_group = -$spell_id;
		$group_subgroups[ $group_subgroups_counter ]['g'] = $group_id;
		$group_subgroups[ $group_subgroups_counter ]['l'] = $link_to_group;
		$group_subgroups_counter++;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//data copy pasted from some site about raid buff stacking rules
$spell_names_list=Array("Ancient Hysteria,Bloodlust,Time Warp",
"Battle Shout,Horn of Winter,Roar of Courage,Strength of Earth Totem",
"Devotion Aura,Stoneskin Totem",
"Abomination's Might,Blessing of Might,Trueshot Aura,Unleashed Rage",
"Elemental Oath,Furious Howl,Terrifying Roar,Honor Among Thieves,Leader of the Pack,Rampage",
"Arcane Tactics,Communion,Ferocious Inspiration",
"Arcane Brilliance,Fel Intelligence",
"Blessing of Might,Fel Intelligence,Mana Spring Totem",
"Hunting Party,Improved Icy Talons,Windfury Totem",
"Ancestral Healing,Inspiration",
"Concentration Aura,Totem of Tranquil Mind",
"Communion,Enduring Winter,Revitalize,Soul Leech,Vampiric Touch",
"Mind Quickening,Moonkin Form,Wrath of Air Totem",
"Arcane Brilliance,Demonic Pact,Flametongue Totem,Totemic Wrath",
"Aspect of the Wild,Elemental Resistance Totem,Resistance Aura,Shadow Protection",
"Blood Pact,Commanding Shout,Power Word: Fortitude,Qiraji Fortitude",
"Blessing of Kings,Embrace of the Shale Spider,Mark of the Wild",
"Corrosive Spit,Tear Armor,Expose Armor,Faerie Fire,Sunder Armor",
"Dust Cloud,Tailspin,Earth Shock,Icy Touch,Infected Wounds,Judgements of the Just,Thunder Clap,Waylay",
"Blood Frenzy,Gore,Stampede,Tendon Rip,Hemorrhage,Mangle",
"Curse of Tongues,Lava Breath,Spore Cloud,Mind-numbing Poison,Necrotic Strike,Slow",
"Furious Attacks,Improved Mind Blast,Legion Strike,Monstrous Bite,Widow Venom,Mortal Strike,Permafrost,Wound Poison",
"Curse of Weakness,Demoralizing Roar,Demoralizing Roar,Demoralizing Screech,Demoralizing Shout,Scarlet Fever,Vindication",
"Acid Spit,Ravage,Blood Frenzy,Brittle Bones,Savage Combat",
"Critical Mass,Shadow and Flame",
"Curse of the Elements,Earth and Moon,Ebon Plaguebringer,Fire Breath,Lightning Breath,Master Poisoner");

foreach( $groups_mask as $key => $value )
	if( $key > $max_group_id )
		$max_group_id = $key;
foreach($spell_names_list as $key => $val )
{
	$parts = explode( "," , $val );
	$groups_mask[ $max_group_id ] = 1;
	foreach( $parts as $key2 => $val2 )
	{
		$get_query = "SELECT id FROM dbc_spell where name like '".mysql_escape_string($val2)."'";
		$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		list($spell_id)=mysql_fetch_row($get_res);
		if( $spell_id == 0 )
			echo "missing name $val2 : $get_query<br>";
		else
		{
			$spell_groups[ $spell_groups_counter ]['s'] = $spell_id;
			$spell_groups[ $spell_groups_counter ]['g'] = $max_group_id;
			$spell_groups_counter++;
		}
	}
	$max_group_id++;
}
//////////////////////////////////////////////////////////////////////////////////////

//generate proper masks for each group
$group_count = count( $groups_mask );
echo "Required bits to represent mask : $group_count = ".((int)((($group_count+31)/32)))." uint32 = ".((int)((($group_count+63)/64)))." uint64 <br>";
echo "PHP test to support 32 bits : ".(1<<31)."<br>";

$mask_index = 0;
foreach( $groups_mask as $key => $value )
{
//	$sub_mask_index = $mask_index % 64;
	$groups_mask[ $key ] = $mask_index;
	$mask_index++;
}
//parse groups in groups list

foreach( $group_subgroups as $key => $val )
{
	//check all spells that have this subgroup
	foreach( $spell_groups as $key2 => $val2 )
		if( $val['l'] == $val2['g'] )
		{
//echo "should match<br>";		
			$spell_groups[ $spell_groups_counter ]['s'] = $val2['s'];
			$spell_groups[ $spell_groups_counter ]['g'] = $val['g'];
			$spell_groups_counter++;
		}
}/**/
//generate spellfixes for each spell
foreach( $spell_groups as $key => $val )
{
	$small_ind = $groups_mask[ $val['g'] ] / 32;
	$mask_ind = $groups_mask[ $val['g'] ] % 32;
	$mask = 1 << $mask_ind;
	$spell_mask[ $val['s'] ][$small_ind] |= $mask;
}
//debug results 
/*
foreach( $groups_mask as $key => $val )
{
	$small_ind = $val / 32;
	$mask_ind = $val % 32;
	$mask = 1 << $mask_ind;
	//search all spells that have this mask
	echo "group $key : ";
	foreach( $spell_mask as $key2 => $val2 )
		if( $val2[$small_ind] & $mask )
		{
//			echo "$key2,";
			$get_query = "SELECT name FROM dbc_spell where id=$key2";
			$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
			list($spell_name)=mysql_fetch_row($get_res);
			echo "$spell_name,";
		}
	echo "<br><br>";
}/**/
//print results
$nl = "\n";
foreach( $spell_mask as $key => $val )
{
	echo "\tsp = dbcSpell.LookupEntryForced( $key ); $nl";
	echo "\t{ $nl";
		echo "\t\tsp->HasStackGroupFlags=1;$nl";
	foreach( $val as $skey => $mask )
		echo "\t\tsp->StackGroupMasks32[$skey]=$mask;$nl";
	echo "\t} $nl";
}/**/
?>