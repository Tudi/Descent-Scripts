<?php

$look_for['OBJECT_']='TYPEID_OBJECT';
$look_for['ITEM_']='TYPEID_ITEM';
$look_for['CONTAINER_']='TYPEID_CONTAINER';
$look_for['UNIT_']='TYPEID_UNIT';
$look_for['PLAYER_']='TYPEID_PLAYER';
$look_for['GAMEOBJECT_']='TYPEID_GAMEOBJECT';
$look_for['DYNAMICOBJECT_']='TYPEID_DYNAMICOBJECT';
$look_for['CORPSE_']='TYPEID_CORPSE';

$list_info="";
$list_values=0;
//read up the file and convert it
$inp_h = @fopen("Updatefields.h", "r");
while($inp_h && !feof($inp_h)) 
{
	//take 1 line
   	$line = fgets($inp_h);
	$line = ltrim($line);
	foreach($look_for as $key => $value )
		if( strpos("^".$line,$key)==1 )
		{
			$line2 = $line;
			$line2 = str_replace("#define ","",$line2);
			$line2 = str_replace(",","",$line2);
			//!!!! this is diff between my updatefield extractor and general one
			if( strpos($line2,"=") )
				$line2 = substr($line2, 0,strpos($line2,"=")); //cut the comment from the end

			if( strpos("^".$line,"PUBLIC")!=0 ||  strpos("^".$line,"DYNAMIC")!=0 )			
				echo "create_for_other_masks[ $value ]->SetBit( $line2 ); <br>";
		}
}
?>