<?php
include("include/Updatefields.inc.php");

$POS_ERROR = 1.5;

//////////////// constants that depend on client version !

$U_FIELD_FLAG_PLAYER_CONTROLLED				= 0x08;
$U_FIELD_ANIMATION_LOOTING					= 0x400;
$U_FIELD_FLAG_DEAD							= 0x4000;
$U_FIELD_FLAG_PLAYER_CONTROLLED_CREATURE	= 0x1000000;
$U_FIELD_FLAG_WEAPON_OFF					= 0x40000000;
$donotsaveflagmask = ~($U_FIELD_FLAG_PLAYER_CONTROLLED | $U_FIELD_ANIMATION_LOOTING | $U_FIELD_FLAG_DEAD | $U_FIELD_FLAG_PLAYER_CONTROLLED_CREATURE | $U_FIELD_FLAG_WEAPON_OFF);

$UNIT_NPC_FLAG_NONE				= 0;
$UNIT_NPC_FLAG_GOSSIP			= 1;
$UNIT_NPC_FLAG_QUESTGIVER		= 2;
$UNIT_NPC_FLAG_TRAINER			= 16;
$UNIT_NPC_FLAG_VENDOR			= 128;
$UNIT_NPC_FLAG_ARMORER			= 4096;
$UNIT_NPC_FLAG_TAXIVENDOR		= 8192;
$UNIT_NPC_FLAG_TRAINER_PROF		= 16384;
$UNIT_NPC_FLAG_SPIRITHEALER		= 32768;
$UNIT_NPC_FLAG_INNKEEPER		= 65536;
$UNIT_NPC_FLAG_BANKER			= 131072;
$UNIT_NPC_FLAG_ARENACHARTER		= 262144;
$UNIT_NPC_FLAG_TABARDCHANGER	= 524288;
$UNIT_NPC_FLAG_BATTLEFIELDPERSON	= 1048576;
$UNIT_NPC_FLAG_AUCTIONEER		= 2097152;
$UNIT_NPC_FLAG_STABLEMASTER		= 4194304;
$UNIT_NPC_FLAG_GUILD_BANK		= 8388608;


$UPDATETYPE_VALUES				= 0;
$UPDATETYPE_MOVEMENT			= 1;
$UPDATETYPE_CREATE_OBJECT		= 2;
$UPDATETYPE_CREATE_OBJECT2		= 3;
$UPDATETYPE_OUT_OF_RANGE_OBJECTS= 4;
$UPDATETYPE_NEAR_OBJECTS		= 5;
$HIGHGUID_ITEM          = 0x40000070;
$HIGHGUID_CONTAINER     = 0x40000070;
$HIGHGUID_UNIT          = 0xF0001000;
$HIGHGUID_PLAYER        = 0x00000000;
$HIGHGUID_GAMEOBJECT    = 0xF0004000;
$HIGHGUID_DYNAMICOBJECT = 0xF000A000;
$HIGHGUID_CORPSE        = 0xF0007000;
$HIGHGUID_PLAYER_CORPSE = 0x80000000;
$TYPEID_OBJECT			= 0;
$TYPEID_ITEM			= 1;
$TYPEID_CONTAINER		= 2;
$TYPEID_UNIT			= 3;
$TYPEID_PLAYER			= 4;
$TYPEID_GAMEOBJECT		= 5;
$TYPEID_DYNAMICOBJECT	= 6;
$TYPEID_CORPSE			= 7;
$TYPEID_AIGROUP			= 8;
$TYPEID_AREATRIGGER		= 9;

////////////////

//$save_timestamp_to_diff_file['step2/SMSG_COMPRESSED_UPDATE_OBJECT.txt']="compressed_lines.stmp";
//$do_not_add_timestamp_packets['step2/SMSG_UPDATE_OBJECT.txt']=1;

////////////////

$MOVEMENTTYPE_NONE = 0;
$MOVEMENTTYPE_RANDOMWP = 1;
$MOVEMENTTYPE_CIRCLEWP = 2;
$MOVEMENTTYPE_WANTEDWP = 3;
$MOVEMENTTYPE_DONTMOVEWP = 4;
$MOVEMENTTYPE_QUEST = 10;
$MOVEMENTTYPE_FORWARDTHANSTOP = 11;
$MOVEMENTTYPE_FORWARDTHANBACK = 12;

////////////////

function convert_to_float($value)
{
	$val2=$value;
	settype($val2, "integer");
	$packed = pack("L*", $val2);
	list(,$unpacked) = unpack("f*", $packed);
	return $unpacked;
}

function convert_from_hex_to_dec($str)
{
	$ret=0;
	for($i=0;$i,strlen($str);$i++)
	{
		if($str[$i]<=9)	$num=$str[$i];
		else if($str[$i]=="A")	$num=10;
		else if($str[$i]=="B")	$num=11;
		else if($str[$i]=="C")	$num=12;
		else if($str[$i]=="D")	$num=13;
		else if($str[$i]=="E")	$num=14;
		else if($str[$i]=="F")	$num=15;
		$ret=$ret*16 + $num;
	}
	return $ret;
}
//we recive a line from one of the step 2 files.We convert that to a simple bytevector
function convert_line_to_elements($p_line)
{
	$ret="";
	$pieces=explode(" ",$p_line);
	if(is_array($pieces))
	foreach($pieces as $index => $val)
//		if($index>3) //use this line if line contains len+opcode
//			$ret[$index-4]=convert_from_hex_to_dec($val);
		{
			if($val[0]=="A")	$num=10;
			else if($val[0]=="B")	$num=11;
			else if($val[0]=="C")	$num=12;
			else if($val[0]=="D")	$num=13;
			else if($val[0]=="E")	$num=14;
			else if($val[0]=="F")	$num=15;
			else $num=$val[0];
			if($val[1]=="A")	$num1=10;
			else if($val[1]=="B")	$num1=11;
			else if($val[1]=="C")	$num1=12;
			else if($val[1]=="D")	$num1=13;
			else if($val[1]=="E")	$num1=14;
			else if($val[1]=="F")	$num1=15;
			else $num1=$val[1];
//			$ret[$index-4]=$num*16+$num1;
			$ret[$index]=$num*16+$num1;
		}
	$timestamp_index=count($pieces)-1;
//echo "timestampindex=$timestamp_index and value = ".$pieces[$timestamp_index]." <br>";
	if($pieces[$timestamp_index]>256) //last value is already in decimal and represents packet time stamp
		$ret[$timestamp_index]=$pieces[$timestamp_index];
	return $ret;
}

function get_row_timestamp($line)
{
	$pieces=explode(" ",$line);
	for($i=(count($pieces)-3);$i<=count($pieces);$i++)
		if(strlen($pieces[$i])>3)
			return $pieces[$i];

	return 0;
}

//we receive a bytevector and an index, we convert 4 bytes into 1 int
function convert_from_line_to_int($line)
{
	global $line_index;
	$ret = $line[$line_index++];
	$ret +=$line[$line_index++]*256;
	$ret +=$line[$line_index++]*65536;
	$ret +=$line[$line_index++]*16777216;
	return (int)($ret);//everything is just a signed int
}
//we receive a bytevector and an index, we convert 4 bytes into 1 int
function convert_from_line_to_int16($line)
{
	global $line_index;
	$ret = $line[$line_index++];
	$ret +=$line[$line_index++]*256;
	return $ret;
}
//we receive a bytevector and an index, we convert 4 bytes into 1 int
function convert_from_line_to_int8($line)
{
	global $line_index;
	$ret = $line[$line_index++];
	return $ret;
}
//we receive a bytevector and an index, we convert 4 bytes into 1 int
function convert_from_line_to_string($line)
{
	global $line_index;
	$ret="";
	$bytes=count($line);
	while($line[$line_index]!=0 && $line_index<=$bytes)
		$ret.=chr($line[$line_index++]);
	$line_index++;
	return $ret;
}

//we receive a bytevector and an index, we convert 4 bytes into 1 int
function convert_from_line_to_guid($line)
{
	global $line_index;
	$tguid_mask=(int)($line[$line_index++]);
//	echo "guid mask is $tguid_mask <br>";
	for($i=0;$i<9;$i++)
		$bytes[$i]=0;
	$counter=0;
	while($tguid_mask)
	{
//		echo (int)((int)($tguid_mask) % 2);
		if((int)((int)($tguid_mask) % 2)!=0)
			$bytes[$counter] = $line[$line_index++];
		$tguid_mask = (int)($tguid_mask / 2);
		$counter++;
	}
	$guid[0] = (int)($bytes[0] + ($bytes[1]<<8) + ($bytes[2]<<16) + ($bytes[3]<<24));
	$guid[1] = (int)($bytes[4] + ($bytes[5]<<8) + ($bytes[6]<<16) + ($bytes[7]<<24));
//	$fulguid = $guid[0] + ($guid[1]<<32);
	return $guid;
}

function read_block_move_part($line,$object_type_id)
{
	global $line_index,$bytes,$debug,$out_unk;
	$move_info="";
	$move_info->flags=(int)($line[$line_index++]);
    if ($move_info->flags & 0x20)
	{
		$move_info->flags2=(int)(convert_from_line_to_int($line));
		$move_info->unk1=(int)(convert_from_line_to_int($line)); //maybe time ?
		//flag2=1 , flag1=112 -> 64
		$move_info->unkbyte1=$line[$line_index++]; //introduced in 2.3.0
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x20 => flags2(uint32)=$move_info->flags2 + unk(uint32)=$move_info->unk1 + unk(uint8)=$move_info->unkbyte1\r\n");
    }
    if ($move_info->flags & 0x40)
    {
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x40 \r\n");
		if(($move_info->flags & 0x02))
		{
			if($debug==1)
				fputs($out_unk,"has move_info->flags & 0x40 and move_info->flags & 0x02 => 4*32bit  (bytes from first value : ".$line[$line_index]." ".$line[$line_index+1]." ".$line[$line_index+2]." ".$line[$line_index+3]."\r\n");
			$move_info->x=convert_to_float(convert_from_line_to_int($line)); // 0 
			$move_info->y=convert_to_float(convert_from_line_to_int($line)); // 0 
			$move_info->z=convert_to_float(convert_from_line_to_int($line)); // 0 
			$move_info->o=convert_to_float(convert_from_line_to_int($line)); // 0rientation
		}
		else
		{
			if($debug==1)
				fputs($out_unk,"has move_info->flags & 0x40 and position (4x float) => 4 * 32 bit  (bytes from first value : ".$line[$line_index]." ".$line[$line_index+1]." ".$line[$line_index+2]." ".$line[$line_index+3]."\r\n");
			$move_info->x=convert_to_float(convert_from_line_to_int($line));
			$move_info->y=convert_to_float(convert_from_line_to_int($line));
			$move_info->z=convert_to_float(convert_from_line_to_int($line));
			$move_info->o=convert_to_float(convert_from_line_to_int($line));
		}
		if(($move_info->flags & 0x20) && ($move_info->flags2 & 0x0200))
		{
			if($debug==1)
				fputs($out_unk,"has move_info->flags & 0x40 and (move_info->flags & 0x20) && (move_info->flags2 & 0x0200) => 6 * 32bit  (bytes from first value : ".$line[$line_index]." ".$line[$line_index+1]." ".$line[$line_index+2]." ".$line[$line_index+3]."\r\n");
			$move_info->transporterguid[0]=convert_from_line_to_int($line);
			$move_info->transporterguid[1]=convert_from_line_to_int($line);
			$move_info->tr_x=convert_to_float(convert_from_line_to_int($line));
			$move_info->tr_y=convert_to_float(convert_from_line_to_int($line));
			$move_info->tr_z=convert_to_float(convert_from_line_to_int($line));
			$move_info->tr_o=convert_to_float(convert_from_line_to_int($line));
			$move_info->tr_unk=convert_from_line_to_int($line);
		}
    }
    if ($move_info->flags & 0x20)
    {   
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x20 \r\n");
		if($move_info->flags2 & 0x00000001){} // will make client show running anymation for the unit all the time to specific creatures
		if($move_info->flags2 & 0x00000004){} //no action in 2.3.0 maybe it is for client only ?
		if($move_info->flags2 & 0x00000010){} //no action in 2.3.0 maybe it is for client only ?
		if($move_info->flags2 & 0x00000100){} // will make client show running anymation for the unit all the time to specific creatures
		if($move_info->flags2 & 0x00000400){} // will make client show npc layed down playing the wounded
		if($move_info->flags2 & 0x00000800){} // some state but it will make client crash if no aditional data provided ?
		if($move_info->flags2 & 0x00100000){} //no action in 2.3.0 maybe it is for client only ?
		if($move_info->flags2 & 0x01000000){} //no action in 2.3.0 maybe it is for client only ?

		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x20 => alwayts 0 (not always actually)\r\n");
		$move_info->unk22=(int)(convert_from_line_to_int($line)); //seems like always 0
        if ($move_info->flags2 & 0x00002000 || $move_info->flags2 & 0x00001000)
        {
			if($debug==1)
				fputs($out_unk,"has move_info->flags & 0x20 move_info->flags2 & 0x00002000  => 4*float (bytes from first value : ".$line[$line_index]." ".$line[$line_index+1]." ".$line[$line_index+2]." ".$line[$line_index+3]."\r\n");
			$move_info->punk1=convert_to_float(convert_from_line_to_int($line));//should be 0
			$move_info->punk2=convert_to_float(convert_from_line_to_int($line));//should be 0
			$move_info->punk3=convert_to_float(convert_from_line_to_int($line));//should be 1
			$move_info->punk4=convert_to_float(convert_from_line_to_int($line));//should be 0
        }
        if ($move_info->flags2 & 0x02000000)
		{
			if($debug==1)
				fputs($out_unk,"has move_info->flags & 0x20 move_info->flags2 & 0x02000000 => uint32\r\n");
			$move_info->unk_02000000=convert_from_line_to_int($line);
		}
        if ($move_info->flags2 & 0x00200000)
		{
			if($debug==1)
				fputs($out_unk,"has move_info->flags & 0x20 move_info->flags2 & 0x00200000 => uint32\r\n");
			$move_info->unk_00200000=convert_from_line_to_int($line);
		}
        if ($move_info->flags2 & 0x04000000)
		{
			if($debug==1)
				fputs($out_unk,"has move_info->flags & 0x20 move_info->flags2 & 0x04000000 => uint32 \r\n");
			$move_info->unk_04000000=convert_from_line_to_int($line);
		}
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x20 => 8 * float values as speed (bytes from first value : ".$line[$line_index]." ".$line[$line_index+1]." ".$line[$line_index+2]." ".$line[$line_index+3]."\r\n");
		$move_info->walks=convert_to_float(convert_from_line_to_int($line));
		$move_info->runs=convert_to_float(convert_from_line_to_int($line));
		$move_info->walkbacks=convert_to_float(convert_from_line_to_int($line));
		$move_info->swims=convert_to_float(convert_from_line_to_int($line));
		$move_info->swimbacks=convert_to_float(convert_from_line_to_int($line));
		$move_info->flys=convert_to_float(convert_from_line_to_int($line));
		$move_info->backflyS=convert_to_float(convert_from_line_to_int($line));
		$move_info->turns=convert_to_float(convert_from_line_to_int($line));
//        if($move_info->flags2 & 0x00400000)
  		//maybe not good at all but in case this is not updatemask then something else is here
//		if(!is_valid_update_mask($line[$line_index+4],$object_type_id) || ($move_info->flags2 & 0x00400000))//skip that wierd unk number and then get mask count
		if($move_info->flags2 & 0x08000000)
        {
			if($debug==1)
				fputs($out_unk,"has move_info->flags2 & 0x08000000 => splineflags\r\n");
			$move_info->splineFlags=(int)(convert_from_line_to_int($line));
            if ($move_info->splineFlags & 0x00010000)
            {
				if($debug==1)
					fputs($out_unk,"has move_info->flags2 & 0x08000000 move_info->splineFlags & 0x00010000 => 3 * float  (bytes from first value : ".$line[$line_index]." ".$line[$line_index+1]." ".$line[$line_index+2]." ".$line[$line_index+3]."\r\n");
				$move_info->splinex[0]=convert_to_float(convert_from_line_to_int($line));
				$move_info->spliney[0]=convert_to_float(convert_from_line_to_int($line));
				$move_info->splinez[0]=convert_to_float(convert_from_line_to_int($line));
            }
            if ($move_info->splineFlags & 0x00020000)
            {
				if($debug==1)
					fputs($out_unk,"has move_info->flags2 & 0x08000000 move_info->splineFlags & 0x00010000 => 2* 32bit\r\n");
				$move_info->splineguid[0]=convert_from_line_to_int($line);
				$move_info->splineguid[1]=convert_from_line_to_int($line);
            }
            if ($move_info->splineFlags & 0x00040000)
			{
				if($debug==1)
					fputs($out_unk,"has move_info->flags2 & 0x08000000 move_info->splineFlags & 0x00010000 => 1 value\r\n");
				$move_info->splineo=convert_to_float(convert_from_line_to_int($line));
			}
			if($debug==1)
				fputs($out_unk,"has move_info->flags2 & 0x08000000 => 4 values -> last is splinecount after that (splinecount+1)*3 uint32 (bytes from first value : ".$line[$line_index]." ".$line[$line_index+1]." ".$line[$line_index+2]." ".$line[$line_index+3]."\r\n");
			$move_info->splinetime1=convert_from_line_to_int($line);
			$move_info->splinetime2=convert_from_line_to_int($line);
			$move_info->splineunk1=convert_from_line_to_int($line);
			$move_info->splinecount=convert_from_line_to_int($line);
			if($move_info->splinecount>$bytes/4)
				$move_info->splinecount=0;//something went wrong
//echo "Spline found at $line_index,count ".$move_info->splinecount." investigate! <br>";
            for ($j = 0; $j <= $move_info->splinecount; $j++)
            {
				$move_info->splinex[$j]=convert_to_float(convert_from_line_to_int($line));
				$move_info->spliney[$j]=convert_to_float(convert_from_line_to_int($line));
				$move_info->splinez[$j]=convert_to_float(convert_from_line_to_int($line));
            }
            if ($move_info->splineFlags & 0x00000100){} //no action in 2.3.0
        }
    }
	if($move_info->flags & 0x08 && $move_info->flags & 0x10)
	{
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x08 && move_info->flags & 0x10 => full guid\r\n");
		$move_info->guid8[0]=convert_from_line_to_int($line);
		$move_info->guid8[1]=convert_from_line_to_int($line);
	}
	else if($move_info->flags & 0x10)
	{
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x10 => uint32\r\n");
		$move_info->packetrecipientcount[0]=convert_from_line_to_int($line); //value seems to related to flag1 value
	}
    if ($move_info->flags & 0x04)
	{
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x04 => compressed guid\r\n");
		$move_info->guid=convert_from_line_to_guid($line);
	}
    if ($move_info->flags & 0x02)
	{
		if($debug==1)
			fputs($out_unk,"has move_info->flags & 0x02 => uint32 \r\n");
		$move_info->assoc=convert_from_line_to_int($line); //-> maybe compressed guid ?
	}
	return $move_info;
}

$write_A9_decoded=1;
function read_block_data($line,$object_type_id,$guid_start_pos=0)
{
	global $line_index,$debug,$out_unk,$bc,$write_A9_decoded,$Player_updatefields_lookup;
	if($write_A9_decoded && ($object_type_id==3 || $object_type_id==4 || $object_type_id==66)) //note: 66 is uncare update type :P
		$do_a9_human_form=1;
	else $do_a9_human_form=0;
	$upd_mask_len=$line[$line_index++];
	if($debug==1 && $object_type_id!=0 && !is_valid_update_mask($upd_mask_len,$object_type_id))
	{
		fputs($out_unk,"!!!!!!!block $bc does not have a valid mask : mask $upd_mask_len object type $object_type_id . Dumping portion of the packet (-5 bytes  / +5 bytes) bytes");
		for($i=-6;$i<6;$i++)
			fputs($out_unk,$line[$line_index+$i]." ");
		fputs($out_unk,"\r\n");
	}
	if($do_a9_human_form)
	{
		$a9_humanly_readable=fopen("a9_human_form.txt","a");
		fputs($a9_humanly_readable,"start createing or updating a new object of type_id=$object_type_id and 8 bytes for guid (guid is copressed fo there might be more bytes here):");
		for($i=$guid_start_pos;$i<=$guid_start_pos+8;$i++)
			fputs($a9_humanly_readable,$line[$i]." ");
		fputs($a9_humanly_readable,"\r\n");
	}
	for($i=0;$i<$upd_mask_len*4;$i++)
		$upd_mask[$i]=$line[$line_index++];
//	echo "updt mask len $upd_mask_len from index $line_index<br>";
	//now we read all the values that are sent to us
	$data="";
	$pmindex = 1;
	for($i=0;$i<$upd_mask_len*32;$i++)
	{
		$mindex = (int)($i/8);
		if($mindex!=$pmindex)
		{
			$pmindex=$mindex;
			$cur_mask=(int)($upd_mask[$mindex]);
//			echo "cur mask index = $mindex and mask $cur_mask <br>";
		}
		$last_bit=(int)((int)($cur_mask) % 2);
		$cur_mask = (int)($cur_mask / 2);
//		echo "$last_bit";
		if($last_bit==1)
		{
			$data[$i]=convert_from_line_to_int($line);
			if($do_a9_human_form)
				fputs($a9_humanly_readable,$Player_updatefields_lookup[$i]."=".$data[$i]."\r\n");//we requested to write to file this A9
		}
		else $data[$i]=0;
	}
	if($do_a9_human_form)
		fclose($a9_humanly_readable);
	return $data;
}

function is_valid_update_mask($mask,$object_type_id)
{
	if(	($mask==8 && $object_type_id==3) || //not unit updatemask
		($mask==2 && $object_type_id==1) || //not item updatemask
		($mask==4 && $object_type_id==3) || //not container updatemask
		($mask==5 && $object_type_id==3) || //not container2 updatemask
		($mask==46 && $object_type_id==4) || //not player updatemask
		($mask==1 && $object_type_id==5) || //not gamobject updatemask
		($mask==1 && $object_type_id==6) || //not dinamicobject updatemask
		($mask==2 && $object_type_id==7)) //not corpse updatemask
		return 1;
	else return 0;
}

function get_config_var($var_name)
{
	global $dbi;
	$get_query="select value from parse_vars where name like '$var_name'";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200708031723 ".mysql_error($dbi).$get_query);
	list($var_value)=mysql_fetch_row($get_res);
	return $var_value;
}

function set_config_var($var_name,$value)
{
	global $dbi;
	$updt_query="update parse_vars set value='".mysql_escape_string($value)."' where name like '$var_name'";
	$updt_res=mysql_query($updt_query,$dbi) or die(" Q200708031723 ".mysql_error($dbi).$updt_query);
}

function get_next_file_to_process()
{
	$srcdir=get_config_var("src_dir");
	if($curdir = opendir($srcdir))
		while($file = readdir($curdir))
			if($file != '.' && $file != '..')
			{
				$srcfile = $srcdir . '\\' . $file;
//				$dstfile = $dstdir . '\\dump.txt';
				if(is_file($srcfile))
				{
					closedir($curdir);
					return $srcfile;
				}
				closedir($curdir);
				return "";
			}
	closedir($curdir);
	return "";
}

function proceedtonextstep($p_nextfilename="")
{
	global $processing_file;
	global $next_file_name;
	//delete the file that we processed already
//	@unlink($processing_file);
	if(strlen($p_nextfilename))
	{
		?>
		<script language="JavaScript" type="text/javascript">
			document.location="<?=$p_nextfilename;?>";
		</script>
		<?php
	}
	else
	{
		?>
		<script language="JavaScript" type="text/javascript">
			document.location="<?=$next_file_name;?>";
		</script>
		<?php
	}
	die();//do not let this run in a separate thread
}

function empty_step2dir()
{
	$srcdir="./step2";
	if($curdir = opendir($srcdir))
		while($file = readdir($curdir))
			if($file != '.' && $file != '..')
			{
				$srcfile = $srcdir . '\\' . $file;
				if(is_file($srcfile))
					unlink($srcfile);
			}
	closedir($curdir);
}

function empty_step3dir()
{
	$srcfile="./step3/character_blocks.txt";
	if(is_file($srcfile))
		unlink($srcfile);
	$srcfile="./step3/creature_blocks.txt";
	if(is_file($srcfile))
		unlink($srcfile);
	$srcfile="./step3/gameobject_blocks.txt";
	if(is_file($srcfile))
		unlink($srcfile);
	$srcfile="./step3/item_blocks.txt";
	if(is_file($srcfile))
		unlink($srcfile);
	$srcfile="./step3/mov_blocks.txt";
	if(is_file($srcfile))
		unlink($srcfile);
	$srcfile="./step3/uncompressed_lines.txt";
	if(is_file($srcfile))
		unlink($srcfile);
	$srcfile="./step3/update_blocks.txt";
	if(is_file($srcfile))
		unlink($srcfile);
	$srcfile="./step3/compressed_line_timestamps.txt";
	if(is_file($srcfile))
		unlink($srcfile);
}


function cleanup_before_map()
{
	global $dbi;
	//clear go and creature relation tables
	$clr_query="DELETE FROM go_lookup";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
}

function cleanup_after_step()
{
	global $dbi;
	//clear go and creature relation tables
//	$clr_query="DELETE FROM go_lookup"; //maybe there are some static spawns that do not require cleaning up
//	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$clr_query="DELETE FROM creature_lookup";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$clr_query="DELETE FROM creature_lookup_summoner";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$clr_query="DELETE FROM death_lookup";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$clr_query="DELETE FROM creature_movement";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$clr_query="DELETE FROM creature_movement_spline";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$clr_query="DELETE FROM item_lookup";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	$clr_query="DELETE FROM creature_emotes_timestamped";
	$get_res=mysql_query($clr_query,$dbi) or die(" Q200611231501 ".mysql_error($dbi));
	empty_step2dir();
	empty_step3dir();
}

function parse_error_report($text)
{
	$ouf_item = fopen("error_reports.txt","a");
	fputs($ouf_item,$text."\n");
	fclose($ouf_item);
}

function parse_debug_report($text)
{
	$ouf_item = fopen("debug_reports.txt","a");
	fputs($ouf_item,$text."\n");
	fclose($ouf_item);
}

function get_prefered_spell_target($spell_id)
{
	global $dbi;
    $EFF_TARGET_NONE                        = 0;
    $EFF_TARGET_SELF                        = 1;
    $EFF_TARGET_PET                         = 5;
    $EFF_TARGET_SINGLE_ENEMY                = 6;
    $EFF_TARGET_ALL_ENEMY_IN_AREA			= 15;
    $EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT   = 16;
    $EFF_TARGET_ALL_PARTY_AROUND_CASTER		= 20;
    $EFF_TARGET_SINGLE_FRIEND               = 21;
    $EFF_TARGET_ALL_ENEMIES_AROUND_CASTER	= 22;
    $EFF_TARGET_GAMEOBJECT					= 23;
    $EFF_TARGET_IN_FRONT_OF_CASTER			= 24;
    $EFF_TARGET_DUEL                        = 25;//Dont know the real name!!!
    $EFF_TARGET_GAMEOBJECT_ITEM				= 26;
    $EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED = 28;
    $EFF_TARGET_MINION						= 32;
    $EFF_TARGET_SINGLE_PARTY				= 35;
    $EFF_TARGET_ALL_PARTY                   = 37;
    $EFF_TARGET_SELF_FISHING				= 39;
    $EFF_TARGET_TOTEM_EARTH					= 41;
    $EFF_TARGET_TOTEM_WATER					= 42;
    $EFF_TARGET_TOTEM_AIR                   = 43;
    $EFF_TARGET_TOTEM_FIRE					= 44;
    $EFF_TARGET_CHAIN                       = 45;
    $EFF_TARGET_DYNAMIC_OBJECT				= 47;//not sure exactly where is used
    $EFF_TARGET_CURRENT_SELECTION			= 53;
    $EFF_TARGET_PARTY_MEMBER                = 57;
    $EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS  = 61;

	$TTYPE_NULL = 0;
	$TTYPE_SINGLETARGET = 1;
	$TTYPE_DESTINATION = 2;
	$TTYPE_SOURCE = 3;
	$TTYPE_CASTER = 4;

	$get_query = "select effect_targeta_1,effect_targeta_2,effect_targeta_3,effect_targetb_1,effect_targetb_2,effect_targetb_3 from dbc_spell where id='$spell_id'";
	$get_res=mysql_query($get_query,$dbi) or die(" Q20061123150141 ".mysql_error($dbi));
	$row=mysql_fetch_row($get_res);
	for($i=0;$i<6;$i++)
		switch($row[$i])
		{
			case $EFF_TARGET_SELF:
			case 38://Dummy Target
			case 32:
			case 73:
				return $TTYPE_CASTER;

			case 4: // dono related to "Wandering Plague", "Spirit Steal", "Contagion of Rot", "Retching Plague" and "Copy of Wandering Plague"
			case $EFF_TARGET_PET:
			case $EFF_TARGET_SINGLE_ENEMY:// Single Target Enemy
			case 77:					// grep: i think this fits
			case 8: // related to Chess Move (DND), Firecrackers, Spotlight, aedm, Spice Mortar
			case $EFF_TARGET_IN_FRONT_OF_CASTER:
			case 31:// related to scripted effects
			case 53:// Target Area by Players CurrentSelection()
			case 54:// Targets in Front of the Caster
			case $EFF_TARGET_ALL_PARTY_AROUND_CASTER:
			case $EFF_TARGET_SINGLE_FRIEND:
			case 45:// Chain,!!only for healing!! for chain lightning =6 
			case 57:// Targeted Party Member
			case $EFF_TARGET_DUEL: 
			case 27: // target is owner of pet
			case $EFF_TARGET_MINION:// Minion Target
			case 33://Party members of totem, inside given range
			case $EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
			case $EFF_TARGET_ALL_PARTY: // all Members of the targets party
			case $EFF_TARGET_TOTEM_EARTH:
			case $EFF_TARGET_TOTEM_WATER:
			case $EFF_TARGET_TOTEM_AIR:
			case $EFF_TARGET_TOTEM_FIRE:// Totem
			case 61: // targets with the same group/raid and the same class
				return $TTYPE_SINGLETARGET;

			case $EFF_TARGET_NONE:
			case $EFF_TARGET_GAMEOBJECT:
			case $EFF_TARGET_GAMEOBJECT_ITEM:// Gameobject/Item Target
			case $EFF_TARGET_SELF_FISHING://Fishing
			case 47:// Portal
			case 52:	// Lightwells, etc
			case 40://Activate Object target(probably based on focus)
				return $TTYPE_NULL;

			case $EFF_TARGET_ALL_ENEMY_IN_AREA: // All Enemies in Area of Effect (TEST)
			case $EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT: // All Enemies in Area of Effect instant (e.g. Flamestrike)
			case $EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED:// All Enemies in Area of Effect(Blizzard/Rain of Fire/volley) channeled
				return $TTYPE_DESTINATION;

				// spells like 17278:Cannon Fire and 21117:Summon Son of Flame A
			case 17: // A single target at a xyz location or the target is a possition xyz
			case 18:// Land under caster.Maybe not correct
			case 46://Unknown Summon Atal'ai Skeleton
			case $EFF_TARGET_ALL_ENEMIES_AROUND_CASTER:
				return $TTYPE_SOURCE;

		}//end switch
	return $TTYPE_NULL;// this means a new spell :P
}

function IsBeneficSpell($spell_id)
{
	global $dbi;
    $EFF_TARGET_NONE                        = 0;
    $EFF_TARGET_SELF                        = 1;
    $EFF_TARGET_PET                         = 5;
    $EFF_TARGET_SINGLE_ENEMY                = 6;
    $EFF_TARGET_ALL_ENEMY_IN_AREA			= 15;
    $EFF_TARGET_ALL_ENEMY_IN_AREA_INSTANT   = 16;
    $EFF_TARGET_ALL_PARTY_AROUND_CASTER		= 20;
    $EFF_TARGET_SINGLE_FRIEND               = 21;
    $EFF_TARGET_ALL_ENEMIES_AROUND_CASTER	= 22;
    $EFF_TARGET_GAMEOBJECT					= 23;
    $EFF_TARGET_IN_FRONT_OF_CASTER			= 24;
    $EFF_TARGET_DUEL                        = 25;//Dont know the real name!!!
    $EFF_TARGET_GAMEOBJECT_ITEM				= 26;
    $EFF_TARGET_ALL_ENEMY_IN_AREA_CHANNELED = 28;
    $EFF_TARGET_MINION						= 32;
    $EFF_TARGET_SINGLE_PARTY				= 35;
    $EFF_TARGET_ALL_PARTY                   = 37;
    $EFF_TARGET_SELF_FISHING				= 39;
    $EFF_TARGET_TOTEM_EARTH					= 41;
    $EFF_TARGET_TOTEM_WATER					= 42;
    $EFF_TARGET_TOTEM_AIR                   = 43;
    $EFF_TARGET_TOTEM_FIRE					= 44;
    $EFF_TARGET_CHAIN                       = 45;
    $EFF_TARGET_DYNAMIC_OBJECT				= 47;//not sure exactly where is used
    $EFF_TARGET_CURRENT_SELECTION			= 53;
    $EFF_TARGET_PARTY_MEMBER                = 57;
    $EFF_TARGET_AREAEFFECT_PARTY_AND_CLASS  = 61;

	$get_query = "select effect_targeta_1,effect_targeta_2,effect_targeta_3,effect_targetb_1,effect_targetb_2,effect_targetb_3 from dbc_spell where id='$spell_id'";
	$get_res=mysql_query($get_query,$dbi) or die(" Q20061123150141 ".mysql_error($dbi));
	$row=mysql_fetch_row($get_res);
	for($i=0;$i<6;$i++)
		switch($row[$i])
		{
				case $EFF_TARGET_SELF:
				case $EFF_TARGET_PET:
				case $EFF_TARGET_ALL_PARTY_AROUND_CASTER:
				case $EFF_TARGET_SINGLE_FRIEND:
				case 45:// Chain,!!only for healing!! for chain lightning =6 
				case 57:// Targeted Party Member
				case 27: // target is owner of pet
				case $EFF_TARGET_MINION:// Minion Target
				case 33://Party members of totem, inside given range
				case $EFF_TARGET_SINGLE_PARTY:// Single Target Party Member
				case $EFF_TARGET_ALL_PARTY: // all Members of the targets party
				case $EFF_TARGET_SELF_FISHING://Fishing
				case 46://Unknown Summon Atal'ai Skeleton
				case 47:// Portal
				case 52:	// Lightwells, etc
				case 40://Activate Object target(probably based on focus)
				case $EFF_TARGET_TOTEM_EARTH:
				case $EFF_TARGET_TOTEM_WATER:
				case $EFF_TARGET_TOTEM_AIR:
				case $EFF_TARGET_TOTEM_FIRE:// Totem
				case 61: // targets with the same group/raid and the same class
				case 32:
				case 73:
					return 1;
		}//end switch
	return 0;
}

function save_a9_sample($block,$vect,$start,$end)
{
	global $dbi;
	$get_query = "select count(id) from a9_samples where flags1='".$block->moveinfo->flags."' and flags2='".$block->moveinfo->flags2."' and spliceflags='".$block->moveinfo->splineFlags."'";
	$get_res=mysql_query($get_query,$dbi) or die(" Q20061123150141 ".mysql_error($dbi));
	list($have_sample)=mysql_fetch_row($get_res);
	if($have_sample==0)
	{
		$ins_query="insert into a9_samples (flags1,flags2,spliceflags,packet_block) values ('".$block->moveinfo->flags."','".$block->moveinfo->flags2."','".$block->moveinfo->splineFlags."','";
		for($i=$start;$i<=$end+2000;$i++)
			$ins_query.=$vect[$i]." ";
		$ins_query.="')";
		$get_res=mysql_query($ins_query,$dbi) or die(" Q200712062143 ".mysql_error($dbi));
	}
}
?>