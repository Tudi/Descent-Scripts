<?php $dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";

//database connection 
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

$SpellDurationFile = "1,10000,0,10000,
2,300000010,0,30000,
3,60000,0,60000,
4,120000,0,120000,
5,300000,0,300000,
6,600000,0,600000,
7,5000000,0,5000,
8,15000,0,15000,
9,30000,0,30000,
10,60000000,0,60000,
11,100000000,200,15000,
12,30000000,200,40000,
13,6000000,200,80000,
14,12000000,10000,180000,
15,30000000,10000,420000,
16,230000,0,230000,
17,5000000,100,7000,
18,20000,0,20000,
19,3000000,500,40000,
20,60000000,1000,80000,
21,-1,0,-1,
22,45000,0,45000,
23,90000,0,90000,
24,160000,0,160000,
25,180000,0,180000,
26,240000,0,240000,
27,3000,0,3000,
28,5000,0,5000,
29,12000,0,12000,
30,1800000,0,1800000,
31,8000,0,8000,
32,6000,0,6000,
35,4000,0,4000,
36,1000,0,1000,
37,1,0,1,
38,11000,0,11000,
39,2000,0,2000,
40,1200000,0,1200000,
41,360000,0,360000,
42,3600000,0,3600000,
62,75000,0,75000,
63,25000,0,25000,
64,40000,0,40000,
65,1500,0,1500,
66,2500,0,2500,
85,18000,0,18000,
86,21000,0,21000,
105,9000,0,9000,
106,24000,0,24000,
125,35000,0,35000,
145,2700000,0,2700000,
165,7000,0,7000,
185,6000,0,21000,
186,2000,0,22000,
187,0,0,5000,
205,27000,0,27000,
225,604800000,0,604800000,
245,50000,0,50000,
265,55000,0,55000,
285,1000,0,6000,
305,14000,0,14000,
325,36000,0,36000,
326,44000,0,44000,
327,500,0,500,
328,250,0,250,
347,900000,0,900000,
367,7200000,0,7200000,
387,16000,0,16000,
407,100,0,100,
427,-600000,60000,600000,
447,2000,0,6000,
467,22000,0,22000,
468,26000,0,26000,
487,1700,0,1700,
507,1100,0,1100,
508,1100,0,1100,
527,14400000,0,14400000,
547,5400000,0,5400000,
548,10800000,0,10800000,
549,3800,0,3800,
550,2147483647,0,2147483647,
551,3500,0,3500,
552,210000,0,210000,
553,6000,0,16000,
554,155000,0,155000,
555,4500,0,4500,
556,28000,0,28000,
557,165000,0,165000,
558,114000,0,114000,
559,53000,0,53000,
560,299000,0,299000,
561,3300000,0,3300000,
562,150000,0,150000,
563,20500,0,20500,
564,13000,0,13000,
565,70000,0,70000,
566,8500,0,8500,
567,135000,0,135000,
568,1250,0,1250,
569,280000,0,280000,
570,32000,0,32000,
571,5500,0,5500,
572,100000,0,100000,
573,11999900,0,11999900,
574,200,0,200,
575,17000,0,17000,
576,43200000,0,43200000,
577,160000,0,160000,
578,14250,0,14250,
579,170000,0,170000,
580,64800000,0,64800000,
581,9000,0,34000,
582,3200,0,3200,
583,600,0,600,
584,800,0,800,
585,0,0,0,
586,0,0,25000,
587,31000,0,31000,
588,0,0,30000,
589,6500,0,6500,
590,330000,0,330000,
591,80000,0,80000,
592,400,0,400,
593,300,0,300,
594,660000,0,660000,
595,302000,0,302000,
596,900,0,900,
597,4700,0,4700,
598,3600,0,3600,
599,6000,0,30000,
600,42000,0,42000,
601,0,0,50000,
602,480000,0,480000,
603,0,0,0,
607,48000,0,48000,
608,64000,0,64000,
609,58000,0,58000,
610,73000,0,73000,
611,2250,0,2250,
612,13400,0,13400,
613,19000,0,19000,
614,51000,0,51000,
615,67000,0,67000,
616,76000,0,76000,
617,79000,0,79000,
618,23000,0,23000,
619,63000,0,63000,
620,15500,0,15500,
621,34000,0,34000,
622,7500,0,7500,
628,2800,0,2800,
629,86400000,0,86400000,
630,1900,0,1900,
631,4800,0,4800,
637,59000,0,59000";

$SpellRadiusFile = "7,2.0,,2.0,
8,5.0,,5.0,
9,20.0,,20.0,
10,30.0,,30.0,
11,45.0,,45.0,
12,100.0,,100.0,
13,10.0,,10.0,
14,8.0,,8.0,
15,3.0,,3.0,
16,1.0,,1.0,
17,13.0,,13.0,
18,15.0,,15.0,
19,18.0,,18.0,
20,25.0,,25.0,
21,35.0,,35.0,
22,200.0,,200.0,
23,40.0,,40.0,
24,65.0,,65.0,
25,70.0,,70.0,
26,4.0,,4.0,
27,50.0,,50.0,
28,50000.0,,50000.0,
29,6.0,,6.0,
30,500.0,,500.0,
31,80.0,,80.0,
32,12.0,,12.0,
33,99.0,,99.0,
35,55.0,,55.0,
36,0.0,,0.0,
37,7.0,,7.0,
38,21.0,,21.0,
39,34.0,,34.0,
40,9.0,,9.0,
41,150.0,,150.0,
42,11.0,,11.0,
43,16.0,,16.0,
44,0.5,,0.5,
45,10.0,,10.0,
46,5.0,,10.0,
47,15.0,,15.0,
48,60.0,,60.0,
49,90.0,,90.0,
50,15.0,,15.0,
51,60.0,,60.0,
52,5.0,,5.0,
53,60.0,,60.0,
54,50000.0,,50000.0,
55,130.0,,130.0,
56,38.0,,38.0,
57,45.0,,45.0,
58,50000.0,,50000.0,
59,32.0,,32.0,
60,44.0,,44.0,
61,14.0,,14.0,
62,47.0,,47.0,
63,23.0,,23.0,
64,3.5,,3.5,
65,80.0,,80.0,
66,100.0,,100.0,
67,30.0,,30.0,
72,20.0,,20.0,
73,1.5,,1.5,
74,0.10000000149,,0.10000000149,
77,6.0,,6.0,
78,30.0,,30.0,
79,50000.0,,50000.0,
80,50000.0,,50000.0,
81,50000.0,,50000.0,
82,110.0,,110.0,
83,50.0,,50.0,
84,17.0,,17.0,
85,30.0,,30.0,
86,30.0,,30.0,
87,30.0,,30.0,
88,300.0,,300.0,
89,400.0,,400.0,
90,0.0,,300.0,
91,20.0,,20.0,
92,80.0,,80.0,
94,15.0,,15.0,
95,1.0,,1.0,
98,2.0,,8.0,
99,2.5,,2.5,
100,4.5,,4.5,
101,50000.0,,50000.0,
102,500.0,,500.0,
103,50000.0,,50000.0,
105,50.0,,50.0,
107,50000.0,,50000.0,
110,50000.0,,50000.0,
111,0.00999999977648,,0.00999999977648,
112,3.75,,3.75,
113,1000.0,,1000.0,";

//single target DMG
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,0,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 2
AND col_22 = 6
and col_8 = 0
AND col_5 > 1000
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//AOE DMG
$get_query2 = "
select dbc_spell.id,col_5,col_3,1,0,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 2
AND col_22 in ( 8, 15, 16, 22, 28, 29 )
and col_8 = 0
AND col_5 > 1000
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//single DOT DMG
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,1,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 6 and col_3 = 3
AND col_22 = 6
and col_8 = 0
AND col_5 > 1000
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//aoe DOT DMG
$get_query2 = "
select dbc_spell.id,col_5,col_3,1,1,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 6 and col_3 = 3
AND col_22 in ( 8, 15, 16, 22, 28, 29 )
and col_8 = 0
AND col_5 > 1000
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//persistent aoe DOT DMG
$get_query2 = "
select dbc_spell.id,col_5,col_3,1,1,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 27 and col_3 = 3
AND col_22 in ( 8, 15, 16, 22, 28, 29 )
and col_8 = 0
AND col_5 > 1000
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//single target heal
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,0,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 in ( 10, 67, 136 )
AND col_22 = 1
and col_8 = 0
AND col_5 > 1
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//AOE target heal
$get_query2 = "
select dbc_spell.id,col_5,col_3,1,0,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 in ( 10, 67, 136 )
AND col_22 in ( 20, 30, 33, 34 )
and col_8 = 0
AND col_5 > 1
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//self buff pos 2 effect
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,60000,2 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 6 and col_3 in ( 9, 13, 15, 21, 22, 24, 28, 29, 31, 34, 35, 47, 49, 51, 52, 54, 55, 57, 65, 70, 79, 80, 83, 85, 90, 99, 101, 110, 118, 124, 127, 135, 136, 137, 138, 140, 149, 150, 161, 166, 167, 168, 174, 175, 189, 193, 199, 216, 230, 237, 238, 240, 250, 282, 317, 320 )
AND col_22 in ( 1 )
and col_8 = 0
AND col_5 > 1
AND col_5 < 10000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//self buff pos
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,60000,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 6 and col_3 in ( 9, 13, 15, 21, 22, 24, 28, 29, 31, 34, 35, 47, 49, 51, 52, 54, 55, 57, 65, 70, 79, 80, 83, 85, 90, 99, 101, 110, 118, 124, 127, 135, 136, 137, 138, 140, 149, 150, 161, 166, 167, 168, 174, 175, 189, 193, 199, 216, 230, 237, 238, 240, 250, 282, 317, 320 )
AND col_22 in ( 1 )
and col_8 = 0
AND col_5 > 1
AND col_5 < 10000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//shield buff pos
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,120000,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 6 and col_3 in ( 15, 69, 97, 150, 158 )
AND col_22 in ( 1 )
and col_8 = 0
AND col_5 > 1000
AND col_5 < 100000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//persistent aoe HOT
$get_query2 = "
select dbc_spell.id,col_5,col_3,1,60000,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 27 and col_3 = 8
and col_8 = 0
AND col_5 > 500
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//single target HOT heal
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,60000,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 = 6 and col_3 in ( 8  )
AND col_22 = 1
and col_8 = 0
AND col_5 > 1
AND col_5 < 50000
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//single target CC bad for target instant
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,0,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND col_1 in ( 8, 9, 19, 62, 68, 98 ) 
AND col_22 = 6
and col_8 = 0
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

//single target CC bad for target aur
$get_query2 = "
select dbc_spell.id,col_5,col_3,0,60000,1 from temp.dbc_spell,temp.spelleffectdbccsv where spelleffectdbccsv.col_24=dbc_spell.id  
AND SpellScalingId=0
AND ( col_1 = 6 and col_3 in ( 5, 7, 12, 14, 25, 26, 27, 33, 87, 99, 115, 138, 193 ) )
AND col_22 = 6
and col_8 = 0
and ( SpellVisual_1 != 0 or SpellVisual_2 != 0 )
GROUP BY spellname,SpellVisual_1
"; /**/

$SpellDurationForIndex = DBCFileToVal( $SpellDurationFile, 1 );
$SpellRadiusForIndex = DBCFileToVal( $SpellRadiusFile, 1 );

//print_r( $SpellDurationForIndex );
//print_r( $SpellRadiusForIndex );

$get_res2 = mysql_query($get_query2 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query2 " );
while (list($spellid,$val,$aur,$is_AOE,$has_dur,$oneeff)=mysql_fetch_row($get_res2) )
{
	//check for spell to have only 1 effect
	if( $oneeff )
	{
		$get_query3 = "select count(*) from temp.spelleffectdbccsv where spelleffectdbccsv.col_24=$spellid"; 
		$get_res3 = mysql_query($get_query3 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query3 " );
		list($effect_count)=mysql_fetch_row($get_res3);
		if( $effect_count != $oneeff )
			continue;
	}
	
	//make sure effect radius is not infinite
	if( $is_AOE )
	{
		$get_query3 = "select col_15 from temp.spelleffectdbccsv where spelleffectdbccsv.col_24=$spellid"; 
		$get_res3 = mysql_query($get_query3 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query3 " );
		list($effect_radius)=mysql_fetch_row($get_res3);
		if( $SpellRadiusForIndex[ $effect_radius ] < 10 || $SpellRadiusForIndex[ $effect_radius ] > 30 )
			continue;
	}
		
	//make sure effect duration is not infinite
	if( $has_dur )
	{
		$get_query3 = "select durationindex from temp.dbc_spell where dbc_spell.id=$spellid"; 
		$get_res3 = mysql_query($get_query3 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query3 " );
		list($durationindex)=mysql_fetch_row($get_res3);
		if( $SpellDurationForIndex[ $durationindex ] > $has_dur || $SpellDurationForIndex[ $durationindex ] < 1000 )
			continue;
	}
		
	//limit number of spells in this order of magnitude
	$magnitude = (int)( $val / 1000 );
	$magnitudecounter[$aur][ $magnitude ]++;
	if( $magnitudecounter[$aur][ $magnitude ] > 10 )
		continue;
		
	$SelectedSpells .= "$spellid,";
}

echo "( $SelectedSpells 0)<br>";

function DBCFileToVal( $file, $col )
{
	$lines = explode( "\n", $file );
	foreach( $lines as $key => $val )
	{
		$values = explode( ",", $val );
		$out[ $values[0] ] = $values[ $col ];
	}
	return $out;
}

?>