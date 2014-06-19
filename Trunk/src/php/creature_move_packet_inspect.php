<?php
include("include/functions.php");
$processing_file="SMSG_MONSTER_MOVE.txt";
$inp_h = @fopen($processing_file, "r");

$MONSTER_MOVE_FLAG_WALK			= 0x0;
$MONSTER_MOVE_FLAG_ARC_JUMP		= 0x00000100;	//removed value until i find them to not crash the client
//	$MONSTER_MOVE_FLAG_RUN			= 0x00001000;
	$MONSTER_MOVE_FLAG_RUN			= 0x00000000;	//removed value until i find them to not crash the client
	$MONSTER_MOVE_FLAG_DEATHGRIP		= 0x00000300;	//this is supposed to be arc jump
	$MONSTER_MOVE_FLAG_HEROIC_LEAP	= 0x00000300;	//this is supposed to be arc jump
	$MONSTER_MOVE_FLAG_FLY			= 0x04000400;	
	$MONSTER_MOVE_FLAG_TAXI_FLY_420	= 0x0C008400;	//need to figure out sub flags; right now it is a hackfix
    $Forward							= 0x00000001;
    $Backward						= 0x00000002;
    $StrafeLeft						= 0x00000004;
    $StrafeRight						= 0x00000008;
    $TurnLeft						= 0x00000010;
    $TurnRight						= 0x00000020;
    $Falling							= 0x00000040;
    $NoSpline						= 0x00000080;
    $Parabolic						= 0x00000100;
    $WalkMode						= 0x00000200;
    $Flying							= 0x00000400;
    $RotationFixed					= 0x00000800;
    $FinalPoint						= 0x00001000;
    $FinalTarget						= 0x00002000;
    $FinalOrient						= 0x00004000;
    $CatMullRom						= 0x00008000;
    $Cyclic							= 0x00010000;
    $EnterCycle						= 0x00020000;
    $Animation						= 0x00040000;
    $Unk4							= 0x00080000;
    $Unk5							= 0x00100000;
    $Unk6							= 0x00200000;
    $Unk7							= 0x00400000;
    $Unk8							= 0x00800000;
    $InvertOrient					= 0x01000000;
    $SmoothPathing					= 0x02000000;
    $Unk11							= 0x04000000;
	
$SearchForFlag1 = 0xFF;
$SearchForFlag2 = 0x01000000;		// 0xFFFFFFFF to skip using
$SearchForFlagNOT2 = 0x00000000;	// 0 to skip using
$debugging_rows = 11111150;

//echo ToHex( 0x0F,4 )."-".ToHex( 0xFF,4 )."-".ToHex( 0xABC,4 ); 
while($inp_h && !feof($inp_h)) 
{
	$debugging_rows--;
	if( $debugging_rows <= 0 ) break;
	//take 1 line
   	$line = fgets($inp_h);
	$vect=convert_line_to_elements($line);
	$line_size = count( $vect );
	$line_index=0;
	$guid1=convert_from_line_to_guid($vect);
//echo "$line_index<br>";
	$guid2=convert_from_line_to_guid($vect);
//echo "$line_index<br>";
	$x = convert_to_float(convert_from_line_to_int($vect));
//echo "$line_index<br>";
	$y = convert_to_float(convert_from_line_to_int($vect));
//echo "$line_index<br>";
	$z = convert_to_float(convert_from_line_to_int($vect));
//echo "$line_index<br>";
	$sync = convert_from_line_to_int($vect);
//echo "$line_index<br>";
	$f1 = $vect[ $line_index++ ];
	if(( $f1 & $SearchForFlag1 ) == 0 )
		continue;
//echo "$line_index<br>";
	if( $f1 == 3 )
	{
		$target_low = convert_from_line_to_int($vect);
		$target_high = convert_from_line_to_int($vect);
	}
	else if( ( $f1 & 4 ) || ( $f1 & 2 ) )
	{
//echo "$x - $y - $z - $sync - $f1 <br>";	
		$o = convert_to_float(convert_from_line_to_int($vect));
//echo "Has o<br>";		
	}
	$f2 = convert_from_line_to_int($vect);
	if(( $f2 & $SearchForFlag2 ) == 0 )
		continue;
	if(( $f2 & $SearchForFlagNOT2 ) != 0 )
		continue;
//echo "$line_index<br>";
	$time = convert_from_line_to_int($vect);
//echo "$line_index<br>";
	if( $f2 & $Parabolic )
	{
//echo "Has parabolic<br>";		
		$zSpeed = convert_to_float(convert_from_line_to_int($vect));
		$unk = convert_from_line_to_int($vect);
	}
	$count = convert_from_line_to_int($vect);
//echo "$line_index - $count <br>";
	if( $count > 100 )
	{
		echo "Strangely large dest count $count. Skipping row $line<br>";
		continue;
	}
	$dx = convert_to_float(convert_from_line_to_int($vect));
	$dy = convert_to_float(convert_from_line_to_int($vect));
	$dz = convert_to_float(convert_from_line_to_int($vect));
	echo "$x-$y-$z => $dx-$dy-$dz ( $time ) - $f1 - $f2(".ToHex($f2, 4).") $count = $line <br>";
	continue;
	//this is twilight zone
	for( $i=0; $i<$count; $i++ )
	{
		if( $line_index > $line_size )
		{
			echo "Not enough bytes in packet - $count - $line_index - $line_size. Skipping row $line<br>";
			break;
		}
	}
	/*
        uint32 last_idx = spline.getPointCount() - 3;
        const Vector3 * real_path = &spline.getPoint(1);

        data << last_idx;
        data << real_path[last_idx];   // destination
        if (last_idx > 1)
        {
            Vector3 middle = (real_path[0] + real_path[last_idx]) / 2.f;
            Vector3 offset;
            // first and last points already appended
            for (uint32 i = 1; i < last_idx; ++i)
            {
                offset = middle - real_path[i];
                data.appendPackXYZ(offset.x, offset.y, offset.z);
            }
        }
		*/
}
@fclose($inp_h);

function ToHex( $num, $OutputByteLen )
{
	$hexstr="0123456789ABCDEF";
	$res = "";
	do{
		$mod = $num % 16;
		$res = $hexstr[$mod].$res;
		$num = (int)((int)$num / (int)16);
	}while( $num > 0 );
	for( $i=strlen( $res );$i<$OutputByteLen*2;$i++)
		$res = "0".$res;	
	return "0x".$res;
}
die();
?>
	f2 & 100
CF A6 99 D9 04 80 04 
00 
F6 54 AA 44 
84 A4 B0 44 
CA 37 A2 43 
5E 04 00 00 
03 - guid
52 96 23 06 00 00 00 03 
00 03 00 14 - f2
55 02 00 00 - 597 
2E 54 9A 41 - 19.291103363037 
00 00 00 00 
01 00 00 00 
D4 30 AD 44 
51 8F AE 44 
CA F0 A6 43 
355069 
 
		F2 & 0x02000000
F7 7F B1 01 FA 07 30 F7 
00 
D1 1E 1B 46 
32 8B 88 44 
66 7B A4 44 
87 6F F2 01 
04 
	D9 6D B7 40 
00 00 00 06 
A7 00 00 00 
01 00 00 00 
0B 1D 1B 46 
23 58 88 44 
F2 7E A4 44

F1 13 0B D5 30 F1 
00 
51 36 A4 44 
D9 8C A3 44 
94 27 10 C1 
18 00 00 00 
	03 6F C8 64 
06 00 00 00 03 00 00 00 06 8D 00 00 00 01 00 00 00 47 17 A4 44 46 87 A3 44 95 28 10 C1

F7 7F B1 01 FA 07 30 F7 
00 
0B 1D 1B 46 
23 58 88 44 
F2 7E A4 44 
E5 6D F2 01 
03 
F5 92 04 00 40 C2 30 F7 
00 00 00 06 
D0 00 00 00 01 00 00 00 D1 1E 1B 46 32 8B 88 44 66 7B A4 44

F3 AD D8 40 C2 30 F1 
00 
AA 85 03 46 
48 D6 7D 44 
51 8C 07 42 
4B 4A EF 01 
00 
00 00 00 02 
6A 0D 00 00 
04 00 00 00 c 4 => 12
31 93 03 46 
D6 B3 7F 44 
2A B1 07 42 
02 38 80 FF 01 18 C0 FE FD CF BF FF 
 

F3 2D DC 72 C2 30 F1 
00 
AA 9A 01 46 
68 FB 70 44 
01 9F 73 41 
49 4A EF 01 
00 
00 00 00 02 
7B 1B 00 00 
03 00 00 00 c 3 => 8
C9 B4 01 46 
05 0D 75 44 
68 C4 5E 41 
02 30 C0 - 3 bytes 
FF F7 4F 3F - 0.81237787008286 
00  

F3 A1 D5 72 C2 30 F1
00 
18 B4 02 46
E0 87 80 44 
6C 5D B0 41 
47 4A EF 01 
00 
00 00 00 02 f2
79 23 00 00 t
08 00 00 00 c 8 => 28
05 63 02 46 8344.7548828125 
F8 E4 7E 44 1019.5776367188 
8D FA 84 41 16.622339248657 
DF 97 FF FD E6 AF BF - 7 bytes ?
FE F1 D7 3F 1.6870725154877 
FF FC F7 3F 1.9374083280563 
00 06 10 80
00 15 40 40 
01 20 68 C0 01

F3 7A 83 72 C2 30 F1 
00 
19 88 01 46 
44 0A 75 44 
D4 72 58 41 
46 4A EF 01 
00 
00 00 00 02 
D5 17 00 00 
01 00 00 00 
6E BA 01 46 
8A 31 77 44 
E8 04 55 41 241332 

F3 7A 83 72 C2 30 F1 
00 
19 88 01 46 x
44 0A 75 44 y
D4 72 58 41 z
46 4A EF 01 - sync
00 flags 1
00 00 00 02 flags 2
D5 17 00 00 time
01 00 00 00 count
6E BA 01 46 8A 31 77 44 E8 04 55 41 241332