<?php
$spellentry="   uint32    Id;                                           // 0        m_ID
    uint32    Attributes;                                   // 1        m_attribute
    uint32    AttributesEx;                                 // 2        m_attributesEx
    uint32    AttributesEx2;                                // 3        m_attributesExB
    uint32    AttributesEx3;                                // 4        m_attributesExC
    uint32    AttributesEx4;                                // 5        m_attributesExD
    uint32    AttributesEx5;                                // 6        m_attributesExE
    uint32    AttributesEx6;                                // 7        m_attributesExF
    uint32    unk_320_1;                                    // 8        3.2.0 (0x20 - totems, 0x4 - paladin auras, etc...)
    uint32    someFlags;                                    // 9        4.0.0
    uint32    unk_400_1;                                    // 10       4.0.0
    uint32    CastingTimeIndex;                             // 11       m_castingTimeIndex
    uint32    DurationIndex;                                // 12       m_durationIndex
    uint32    powerType;                                    // 13       m_powerType
    uint32    rangeIndex;                                   // 14       m_rangeIndex
    float     speed;                                        // 15       m_speed
    uint32    SpellVisual[2];                               // 16-17    m_spellVisualID
    uint32    SpellIconID;                                  // 18       m_spellIconID
    uint32    activeIconID;                                 // 19       m_activeIconID
    char*	  SpellName;                                    // 20       m_name_lang
    char*	  Rank;                                         // 21       m_nameSubtext_lang
    char*	  Description;                                // 22       m_description_lang not used
    char*	  ToolTip;                                    // 23       m_auraDescription_lang not used
    uint32    SchoolMask;                                   // 24       m_schoolMask
    uint32    runeCostID;                                   // 25       m_runeCostID
    uint32    spellMissileID;                             // 26       m_spellMissileID not used
    uint32	  spellDescriptionVariableID;                   // 27       3.2.0
    uint32    SpellDifficultyId;                              // 28       m_spellDifficultyID - id from SpellDifficulty.dbc
    float     unk_f1;                                         // 29
    uint32    SpellScalingId;                                  // 30       SpellScaling.dbc
    uint32    SpellAuraOptionsId;                              // 31       SpellAuraOptions.dbc
    uint32    SpellAuraRestrictionsId;                         // 32       SpellAuraRestrictions.dbc
    uint32    SpellCastingRequirementsId;                      // 33       SpellCastingRequirements.dbc
    uint32    SpellCategoriesId;                               // 34       SpellCategories.dbc
    uint32    SpellClassOptionsId;                             // 35       SpellClassOptions.dbc
    uint32    SpellCooldownsId;                                // 36       SpellCooldowns.dbc
    uint32    unkIndex7;                                     // 37       all zeros...
    uint32    SpellEquippedItemsId;                            // 38       SpellEquippedItems.dbc
    uint32    SpellInterruptsId;                               // 39       SpellInterrupts.dbc
    uint32    SpellLevelsId;                                   // 40       SpellLevels.dbc
    uint32    SpellPowerId;                                    // 41       SpellPower.dbc
    uint32    SpellReagentsId;                                 // 42       SpellReagents.dbc
    uint32    SpellShapeshiftId;                               // 43       SpellShapeshift.dbc
    uint32    SpellTargetRestrictionsId;                       // 44       SpellTargetRestrictions.dbc
    uint32    SpellTotemsId;                                   // 45       SpellTotems.dbc
    uint32    ResearchProject;                               // 46  
";

$lines = explode("\n",$spellentry);
$formatstr="";
foreach($lines as $key => $value )
{
//	echo "or line is : $value<br>";
	$t=str_replace(array("\t","\n")," ",$value);
	$t=trim($t);
	$t=str_replace("*"," ",$t);
	$t=str_replace(";"," ",$t);
	$len = strlen($t);
	for($i=0;$i<$len;$i++)
		$t=str_replace("  "," ",$t);
//	echo "new line is : $t1<br>";
	$parts = explode(" ",$t);
//	echo $parts[0]." + ".$parts[1]." + ".$parts[2]." <br> ";

	//check type
	$format="!";
	if(strpos("#".$parts[0],"//") != 0 )
		$format="x";
	else if(strpos("#".$parts[0],"uint32") != 0 )
		$format="u";
	else if(strpos("#".$parts[0],"int32") != 0 )
		$format="i";
	else if(strpos("#".$parts[0],"float") != 0 )
		$format="f";
	else if(strpos("#".$parts[0],"char") != 0 )
		$format="s";

	//check count
	$formatc=1;
	$name="";
	if( strpos("#".$parts[1],"[") )
	{
		$subp=substr($parts[1],strpos($parts[1],"["));
		$subp=explode("[",$parts[1]);
		$subp=explode("]",$subp[1]);
		$formatc=(int)($subp[0]);
		$name = substr($parts[1],0,strpos($parts[1],"["));
	}
	else $name = $parts[1];
	if($name=="")
	{
		echo $parts[0]." + ".$parts[1]." + ".$parts[2]." <br> ";
		echo "or line is : $value<br>";
	}


	//add to format string
	for($i=0;$i<$formatc;$i++)
	{
		$formatstr.=$format;
		$outl="";
		$outl .= "{\"";
		if( $format == "u" )
			$outl .= "uint32";
		else if( $format == "i" )
			$outl .= "int32";
		else if( $format == "f" )
			$outl .= "float";
		else if( $format == "s" )
			$outl .= "str";
		else if( $format == "x" )
			continue;
		$outl .= "\",\"".$name;
		if( $formatc > 1 )
			$outl .= "_".($i+1);
		$outl .= "\"},<br>";
		echo $outl;
	}
}
echo "const char* spellentryFormat = \"$formatstr\";<br>";
?>
