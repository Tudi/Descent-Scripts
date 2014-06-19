<?php
session_start();
include("include/config.inc.php");
include("include/mysql.inc.php"); 
foreach($_REQUEST as $key => $value)
	$$key=mysql_escape_string($value);

set_time_limit (5*60*1000);
?>
 
<!--

===============================================================

	               DEVELOPED BY:
	    Jozsa Istvan - jozsab1@yahoo.com

===============================================================

-->

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<title>Dump parser</title>
<link href="../css/pbx.css" rel="stylesheet" type="text/css">
</head>
<body>
<a class="clsListText01" href="index.php">HOME</a>
<br>