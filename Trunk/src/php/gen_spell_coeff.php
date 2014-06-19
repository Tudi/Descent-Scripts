<?php
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$input_damage=
"Healing Touch 		161.1
Lifebloom 		51.6
Regrowth 		53.90
Nourish 		67.1
Starfall 		1.19
Typhoon 		19.3
Wrath 			57.14
Arcane Blast 		71.43
Arcane Explosion 		21.28
Arcane Missiles 		28.57
Mana Shield 		80.53
Arcane Barrage 		71.43
Blast Wave 		19.36
Dragon's Breath 		19.36
Fire Blast 		42.86
Flamestrike 		23.57
Pyroblast 		115.0
Scorch 		42.86
Frostfire Bolt 		85.71
Blizzard 		115
Cone of Cold 		21.4
Frost Nova 		19.3
Frostbolt 		81.43
Ice Barrier 		80.53
Ice Lance 		14.29
Flash of Light 		100
Holy Light 		170
Holy Shock 		80.82
Sacred Shield 		75.00
Avenger's Shield 		7.0
Consecration 		32
Crusader Strike 		0
Divine Storm 		0
Exorcism 			15.0
Hammer of Wrath 	15.0
Holy Shield			9.0
Holy Shock 			42.86
Holy Wrath 			7.0
Retribution Aura 		3.3
Seal of Command 		0
Seal of Command (judgement) 		14
Seal of Corruption (judgement) 		24
Seal of Justice (judgement) 		27
Seal of Light 		15
Seal of Light (judgement) 		27
Seal of Righteousness (per second) 		9.5
Seal of Righteousness (judgement) 		35.5
Seal of Vengeance (judgement)		33
Seal of Wisdom (judgement) 		27
Shield of Righteousness 		0
Seal of Blood (judgement) 		25
Seal of the Martyr (judgement)		25
Binding Heal 		80.68
Circle of Healing 		40.20
Desperate Prayer 		80.68
Flash Heal 		80.68
Greater Heal 		161.35
Power Word: Shield 		80.68
Prayer of Healing 		52.6
Prayer of Mending (per charge) 		80.68
Renew 		37.6
Holy Fire 		57.11
Holy Nova 		16.1
Mana Burn 		0.0
Mind Blast 		42.85
Smite 		71.40
Chain Heal 		234.5
Earth Shield 		15
Healing Wave 		161.06
Lesser Healing Wave 		80.82
Riptide 		130
Chain Lightning 		125
Earth Shock 		38.58
Fire Nova Totem 		21.42
Flame Shock 		21.42
Frost Shock 		38.58
Lava Burst 		57.14
Lightning Bolt 		71.43
Lightning Shield 		100
Curse of Doom 		200
Shadowflame 		10.64
Conflagrate 		0
Death Coil 		21.40
Haunt 		47.93
Incinerate 		71.39
Searing Pain 		42.93
Shadow Bolt 		85.69
Shadowburn 		42.93
Shadowfury 		19.32
Soul Fire 		115
Chaos Bolt 		71.39
Dark Pact 		96.0
Shadow Ward 		30.0
Immolation Aura 		214.5
Shadow Cleave 		21.3
Gift of the Naaru 		188.5

Rain of Fire 		33.00
Hellfire 		14.3
Drain Soul		42.90
Drain Mana 		0
Drain Life 		14.30
Corruption 		20
Curse of Agony 		10
Searing Totem 		16.67
Magma Totem 		6.66
Vampiric Touch 		40.0
Shadow Word: Pain 		18.3
Penance 		22.9
Mind Flay 		25.70
Mind Sear 		28.61
Moonfire 		15.15
Insect Swarm 		20
Hurricane 		12.898
Force of Nature		3.5
Entangling Roots	10
Wild Growth 		11.505
Tranquility 		53.80
Rejuvenation 		37.604
";

$input_DOT=
"Living Bomb 			80.0
Seal of Corruption 		33
Starfall		4.58
Unstable Affliction 		20
Immolate 		20
Shadowflame 		6.67
Seed of Corruption 		25
Flametongue Weapon 		10
Frostbrand Weapon 		10
Flame Shock 		10
Holy Fire 		2.40
Flamestrike 		48.8
Seal of Vengeance			33
Thorns	 		3.3
";

$input_heal=
"Holy Nova 		30.35
Penance 		53.62
";

$input_hot=
"Regrowth 		18.80
Lifebloom 		9.518
Healing Stream Totem 		4.5
Health Funnel 		54.8
";

$output="";
$output .=ProcessList($input_damage,0);
$output .=ProcessList($input_heal,0);
$output .=ProcessList($input_DOT,1);
$output .=ProcessList($input_hot,1);

echo $output;

function ProcessList($input,$dot)
{
	$output = "";
	global $dbi;
	$lines = explode( "\n",$input);
	foreach($lines as $key => $line )
	{
		if( strlen( $line ) < 3 )
			continue;
		$parts = explode( "\t",$line);
		$name = trim($parts[0]);
		$pct = trim($parts[count($parts)-1]);
//echo "$name - $pct <br>";
		$get_query = "SELECT id,name FROM dbc_spell WHERE name='".mysql_escape_string($parts[0])."'"; 
		$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
		$t = "";
		while(list($id,$sname)=mysql_fetch_row($get_res))
		{
			if( $dot )
				$t .= "replace into spell_coef_override (id,name,OTspell_coef_override) values ($id,'".mysql_escape_string($sname).",".($pct/100).");<br>";
			else
				$t .= "replace into spell_coef_override (id,name,Dspell_coef_override) values ($id,'".mysql_escape_string($sname)."',".($pct/100).");<br>";
		}
		if( $t =="" )
			echo "#could not find spell $name - $line<br>";
		$output .= $t;
	}
	return $output;
}
?>