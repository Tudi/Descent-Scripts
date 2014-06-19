<?php
$f = fopen( "t.txt", "wt" );
foreach($_REQUEST as $request_name=>$request_value)
{
	$$request_name=mysql_escape_string($request_value);
	fwrite( $f, "$request_name = ".$$request_name );
}
//fwrite( $f, "test" );
fclose( $f );
?>