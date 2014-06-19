<?php

$inf=fopen("SpellItemEnchantment.dbc.csv","r");
$results="";
$result_counts="";
$found=0;
$lines_read=0;

/*
	WARRIOR = 1,
	PALADIN = 2,
	HUNTER = 3,
	ROGUE = 4,
	PRIEST = 5,
	DK = 6,
	SHAMAN = 7,
	MAGE = 8,
	WARLOCK = 9,
	DRUID = 11,
*/
$gen_for_class = 11;

function print_line($line)
{
//	echo $line."<br>";
	$parts = explode("\"",$line);
	$name = $parts[1];
//	echo $name."<br>";
}

while($inf && !feof($inf)) 
{
   	$line = fgets($inf);
	$lines_read++;

	//check requirements
	$pieces=explode(",",$line);
	$count=count($pieces);
	//echo $count."...".$pieces[ $count-3 ]."<br>";
	if( $pieces[ $count-3 ] )
		continue;

	if( $gen_for_class == 1 )
	{
	//warrior
	//	echo "<br> warr <br>";
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Stre")!=0
			|| strpos($line,"Agi")!=0
			|| strpos($line,"Attack P")!=0
			|| strpos($line,"Rating")!=0 
			|| strpos($line,"Dodge")!=0
			|| strpos($line,"Parry")!=0
			|| strpos($line,"Block")!=0
	//		|| strpos($line,"Armo")!=0
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Spell")==0
			&& strpos($line,"Resilience")==0
			&& strpos($line,"Haste")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& strpos($line,"Skill")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& !( strpos($line,"Defense Rating")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Agility")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Strength")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Attack Power")!=0 && ( $lines_read % 4 != 0 ) )
			&&  strpos($line," vs ")==0
			)
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
//	$found=0;
	if( $gen_for_class == 2 )
	{
		//paladin
	//	echo "<br> paladin <br>";
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Stre")!=0
			|| strpos($line,"Int")!=0
//			|| strpos($line,"Attack P")!=0
			|| (strpos($line,"Power")!=0 && ( $lines_read % 4 == 0 ) )
			|| strpos($line,"Rating")!=0 
			|| strpos($line,"Dodge")!=0
			|| strpos($line,"Parry")!=0
			|| strpos($line,"Block")!=0
	//		|| strpos($line,"Armo")!=0
			|| ( strpos($line,"Resistance")!=0 && ( $lines_read % 4 == 0 ) )
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Defense Rating")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Strength")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Attack Power")!=0 && ( $lines_read % 4 != 0 ) )
			&&  strpos($line," vs ")==0
			)
			
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 3 )
	{
	//	$found=0;
		//hunt
	//	echo "<br> hunt <br>";
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Stre")!=0
			|| strpos($line,"Agi")!=0
			|| strpos($line,"Int")!=0
			|| strpos($line,"Ranged Attack")!=0
			|| strpos($line,"Rating")!=0 
			|| strpos($line,"Dodge")!=0
			|| strpos($line,"Parry")!=0
			)
			&& 
			(
			strpos($line,"Resilience")==0
			&& strpos($line,"Haste")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Defense Rating")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Agility")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Strength")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Attack Power")!=0 && ( $lines_read % 4 != 0 ) )
			&&  strpos($line," vs ")==0
			)
			
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 4 )
	{
		//rog
	//	echo "<br> rog <br>";
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Agi")!=0
			|| strpos($line,"Rating")!=0 
			|| strpos($line,"Dodge")!=0
			|| strpos($line,"Parry")!=0
	//		|| strpos($line,"Armo")!=0
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Spell")==0
			&& strpos($line,"Haste")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Defense Rating")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Agility")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Attack Power")!=0 && ( $lines_read % 4 != 0 ) )
			&&  strpos($line," vs ")==0
			)
			
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 5 )
	{
	//	$found=0;
	//	echo "<br> priest <br>";
		//priest
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Int")!=0
			|| strpos($line,"Spir")!=0
			|| strpos($line,"Rating")!=0 
			|| ( strpos($line,"Resistance")!=0 && ( $lines_read % 8 == 0 ) )
			|| (strpos($line,"Power")!=0 && ( $lines_read % 4 == 0 ) )
			|| strpos($line,"Mana")!=0 
			|| strpos($line,"Haste")!=0 
			|| strpos($line,"Spell")!=0 
			|| strpos($line,"Stats")!=0 
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"Block")==0
			&& strpos($line,"Dodge")==0
			&& strpos($line,"Parry")==0
			&& strpos($line,"Defens")==0
			&& strpos($line,"Attack")==0
			&& strpos($line,"Strike")==0
			&& strpos($line,"Hit")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Spirit")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Shadow Spell")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Holy Spell")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Healing and Spell ")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Healing Spells")!=0 && ( $lines_read % 4 != 0 ) )
			&&  strpos($line," vs ")==0
			)		
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 6 )
	{
	//	$found=0;
	//	echo "<br> priest <br>";
		//DK
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Int")!=0
			|| strpos($line,"Spir")!=0
			|| strpos($line,"Rating")!=0 
			|| ( strpos($line,"Resistance")!=0 && ( $lines_read % 8 == 0 ) )
			|| (strpos($line,"Power")!=0 && ( $lines_read % 4 == 0 ) )
			|| strpos($line,"Mana")!=0 
			|| strpos($line,"Haste")!=0 
			|| strpos($line,"Spell")!=0 
			|| strpos($line,"Stats")!=0 
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"Block")==0
			&& strpos($line,"Parry")==0
			&& strpos($line,"Defens")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Mana")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Spirit")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Shadow Spell")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Holy Spell")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Healing and Spell ")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Healing Spells")!=0 && ( $lines_read % 4 != 0 ) )
			&&  strpos($line," vs ")==0
			)		
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 7 )
	{
	//	$found=0;
	//	echo "<br> sham <br>";
		//sham
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Stre")!=0
			|| strpos($line,"Int")!=0
			|| strpos($line,"Attack P")!=0
			|| strpos($line,"Rating")!=0 
			|| strpos($line,"Dodge")!=0
			|| strpos($line,"Parry")!=0
			|| strpos($line,"Block")!=0
	//		|| strpos($line,"Armo")!=0
			|| ( strpos($line,"Resistance")!=0 && ( $lines_read % 8 == 0 ) )
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Defense Rating")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Strength")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Attack Power")!=0 && ( $lines_read % 8 != 0 ) )
			&&  strpos($line," vs ")==0
			)
			
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 8 )
	{
	//	$found=0;
	//	echo "<br> mage <br>";
		//mage
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Int")!=0
			|| strpos($line,"Spir")!=0
			|| strpos($line,"Rating")!=0 
			|| ( strpos($line,"Resistance")!=0 && ( $lines_read % 8 == 0 ) )
			|| (strpos($line,"Power")!=0 && ( $lines_read % 4 == 0 ) )
			|| strpos($line,"Mana")!=0 
			|| strpos($line,"Haste")!=0 
			|| strpos($line,"Spell")!=0 
			|| strpos($line,"Stats")!=0 
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"Block")==0
			&& strpos($line,"Dodge")==0
			&& strpos($line,"Parry")==0
			&& strpos($line,"Defens")==0
			&& strpos($line,"Attack")==0
			&& strpos($line,"Strike")==0
			&& strpos($line,"Hit")==0
			&& strpos($line,"Healing")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Defense Rating")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Spirit")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Spirit")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Arcane S")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Fire S")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Frost S")!=0 && ( $lines_read % 8 != 0 ) )
			&&  strpos($line," vs ")==0
			)
			
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 9 )
	{
	//	$found=0;
	//	echo "<br> warl <br>";
		//warl
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Int")!=0
			|| strpos($line,"Rating")!=0 
			|| ( strpos($line,"Resistance")!=0 && ( $lines_read % 8 == 0 ) )
			|| (strpos($line,"Power")!=0 && ( $lines_read % 4 == 0 ) )
			|| strpos($line,"Mana")!=0 
			|| strpos($line,"Haste")!=0 
			|| strpos($line,"Spell")!=0 
			|| strpos($line,"Stats")!=0 
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"Block")==0
			&& strpos($line,"Dodge")==0
			&& strpos($line,"Parry")==0
			&& strpos($line,"Defens")==0
			&& strpos($line,"Attack")==0
			&& strpos($line,"Strike")==0
			&& strpos($line,"Hit")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Healing ")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Spirit")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Fire S")!=0 && ( $lines_read % 8 != 0 ) )
			&& !( strpos($line,"Shadow S")!=0 && ( $lines_read % 8 != 0 ) )
			&&  strpos($line," vs ")==0
			)		
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
	if( $gen_for_class == 11 )
	{
	//	$found=0;
		//druid
	//	echo "<br> druid <br>";
		if(
			(strpos("#".$line,"Stam")!=0
			|| strpos($line,"Spir")!=0
			|| strpos($line,"Int")!=0
			|| strpos($line,"Rating")!=0 
			|| strpos($line,"Dodge")!=0
			|| strpos($line,"Parry")!=0
			|| strpos($line,"Block")!=0
			|| ( strpos($line,"Resistance")!=0 && ( $lines_read % 8 == 0 ) )
			)
			&& 
			(
			strpos($line,"Ranged")==0
			&& strpos($line,"Level")==0
			&& strpos($line,"$")==0
			&& strpos($line,"Fire S")==0
			&& strpos($line,"Arcane S")==0
			&& strpos($line,"Frost S")==0
			&& strpos($line,"Shadow S")==0
			&& strpos($line,"Nature S")==0
			&& strpos($line,"Holy S")==0
			&& strpos($line,"Mana")==0
			&& strpos($line,"Health")==0
			&& !(strpos($line,"+4")!=0 && strpos($line,"Resistance")!=0)
			&& strpos($line,"Skill")==0
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Stamina")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Spirit")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Intellect")!=0 && ( $lines_read % 4 != 0 ) )
			&& !( strpos($line,"Defense Rating")!=0 && ( $lines_read % 8 != 0 ) )
			&&  strpos($line," vs ")==0
			)
			
		)
		{
			$pieces=explode(",",$line);
			echo $pieces[0].",";
			print_line ($line);
			$found++;
		}
	//	echo "<br>".$found."<br>";
		/**/
	}
//	$found=0;
}
fclose($inf);
echo "<br>".$found."<br>";

?>