<?php

#define SKILL_FIRST_AID                 129
#define SKILL_BLACKSMITHING             164
#define SKILL_LEATHERWORKING            165
#define SKILL_ALCHEMY                   171
#define SKILL_HERBALISM                 182
#define SKILL_COOKING                   185
#define SKILL_MINING                    186
#define SKILL_TAILORING                 197
#define SKILL_ENGINERING                202
#define SKILL_ENCHANTING                333
#define SKILL_FISHING                   356
#define SKILL_SKINNING                  393
#define SKILL_JEWELCRAFTING             755
#define SKILL_INSCRIPTION				773
#define SKILL_RUNEFORGING				776

$inf=fopen("SkillLineAbility.dbc.csv","r");
$for_trainer=1;
$selected_skilline=778;
$spell_cost = 1;
$avoid_learning=",7620,7731,7732,18248,33095,3273,3274,7924,10846,27028,2550,3102,3413,18260,33359,8613,8617,8618,10768,32678,2575,2576,3564,10248,29354,2108,3104,3811,10662,32549,2366,2368,3570,11993,28695,2018,3100,3538,9785,29844,2259,3101,3464,11611,28596,25229,25230,28894,28895,28897,4036,4037,4038,12656,30350,3908,3909,3910,12180,26790,7411,7412,7413,13920,28029,45357,45358,45359,45360,45361,";

while($inf && !feof($inf)) 
{
   	$line = fgets($inf);
	if( strpos("#".$line,",$selected_skilline,")==0 )
		continue;

	//check requirements
	$pieces=explode(",",$line);
	if( $pieces[ 1 ] != $selected_skilline )
		continue;

	$spell = $pieces[2];

	if( strpos("#".$avoid_learning,"$spell")!=0 )
		continue;
//	echo "insert into trainer_spells values ($for_trainer,0,$spell,$spell_cost,0,$selected_skilline,1,0,0,0);<br>";
	echo "$spell,";

}
fclose($inf);
echo "<br>".$found."<br>";

?>