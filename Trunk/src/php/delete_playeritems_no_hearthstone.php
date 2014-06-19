<?php
	$dbhost = "localhost";
	$dbname = "characters";
	$dbuname = "root";
	$dbupass = "lofaszka";
	$dbtype = "MySQL";
	//connect to mole's settings database
	$dbim =mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
	mysql_select_db($dbname, $dbim) or die("Q 200503271826");

	$query = "select ownerguid where ownerguid not in ( select ownerguid from playeritems where entry in ( 6948 ) )";
	$result=mysql_query($query,$dbim) or die($query);
	$counter = "";
	$chars = "";
	while($row=mysql_fetch_row($result) )
	{
		$counter[$row[0]] = $counter[$row[0]] + 1;
	}
		
	asort( $counter );
	
	foreach( $counter as $key => $value )
		echo "$key = $value <br>";
?>