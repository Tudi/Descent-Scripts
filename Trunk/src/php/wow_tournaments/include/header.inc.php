<?php
ini_set('session.bug_compat_warn', 0);
session_start();
header('Content-Type: text/html; charset=utf-8');
include("include/config.inc.php");

foreach($_REQUEST as $foreachname=>$foreachvalue)
	$$foreachname=$foreachvalue;
foreach($_SESSION as $foreachname=>$foreachvalue)
	$$foreachname=$foreachvalue;

//connect to logon database
$dbim =mysql_connect($dbhost_logon, $dbuname_logon, $dbupass_logon,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname_logon, $dbim) or die("Q 200503271826");

//connect to char DB
$dbi = mysql_connect($dbhost_chars, $dbuname_chars, $dbupass_chars,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname_chars, $dbi) or die("Q 200503271827");

if (!session_is_registered("dbim"))
	session_register("dbim");

if (!session_is_registered("dbi"))
	session_register("dbi");
	

foreach($_REQUEST as $request_name=>$request_value)
	$$request_name=mysql_escape_string($request_value);

include("include/functions.inc.php");
?>


<!--

===============================================================

   M O L E    v 2.0	    
   Copyright (c) GNU GPL
   
   DEVELOPED BY:
   Zsolt Gabor - cumxum@yahoo.com
   Zoltan Gaspar - gasparz@gmail.com
   Istvan Jozsa Bodnar - jozsab1@hotmail.com

   DESIGN BY:
   Robert Ronyecz - ronyecz_robert@hotmail.com

===============================================================

-->

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>Tournamet Maker</title>
<link href="css/mole.css" rel="stylesheet" type="text/css">
</head>
<body>
