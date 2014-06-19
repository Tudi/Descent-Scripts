<?php
echo "!!!!FLOATS ARE NOT SAFE TO BE USED!!!";
/*
struct DBC_header_struct
{
	int Signiture;
	int RecordsCount;
	int FieldsCount;
	int RecordSize;
	int StringTableSize; 
};
/**/
$f = fopen("t.dbc","wb");
//header
WriteVect( "WDBC", $f ); //int Signiture;
WriteUint32( 1, $f ); //int RecordsCount;
WriteUint32( 2, $f ); //int FieldsCount;
WriteUint32( 8, $f ); //int RecordSize;
WriteUint32( 1, $f ); //int StringTableSize;
//data as data
WriteUint32( 10, $f ); //int val 1;
WriteFloat( 0.9, $f ); //int val 1;
//data as list of strings
WriteStr( "", $f );
/*
WriteUint8( 0x11223344, $f );
WriteUint32( 0x11223344, $f );
WriteStr( "test", $f );
WriteFloat( (float)0.5, $f );
*/
fclose( $f );

function convert_from_float($value)
{
	$a = unpack("L",pack( "f",$value ));
	return $a[1];
}
function WriteUint8( $val, $File )
{
	WriteStrAsBin( $val, $File, 1 );
}
function WriteUint32( $val, $File )
{
	WriteStrAsBin( $val, $File, 4 );
}
function WriteFloat( $val, $File )
{
	$val = convert_from_float( $val );
echo "-$val-";
	WriteStrAsBin( $val, $File, 4 );
}
function WriteStr( $val, $File )
{
	fwrite( $File, $val, strlen( $val ) );
	WriteUint8( 0, $File );
}
function WriteVect( $val, $File )
{
	fwrite( $File, $val, strlen( $val ) );
}
function WriteStrAsBin( $val, $File, $count )
{
	$val = (int)$val;
	for($i=0;$i<$count;$i++)
	{
		$FileByte = chr( (int)$val & 0xFF );
		$val = $val >> 8;
		fwrite( $File, $FileByte, 1 );
	}
}
?>