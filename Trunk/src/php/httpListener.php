<?php
$f = fopen( "ParamsReceived.txt", "at" );
if( $f )
{
	foreach($_REQUEST as $request_name=>$request_value)
		fwrite( $f, "$request_name = $request_value\n");
	fwrite( $f, "\n\n" );
	fclose( $f );
}
echo "Thanks, drop by later";

?>