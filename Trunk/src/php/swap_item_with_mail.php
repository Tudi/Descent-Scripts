<?php
set_time_limit (3000);

$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="acemu_chars";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); /**/

$i=0;
$from[$i]=17;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=2248;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=138;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=89;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=93;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=133;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=5090;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=101;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=105;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=123;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=134;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=18438;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=12751;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=28648;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=12750;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=5222;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=5226;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=4033;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=3002;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=1350;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=1544;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=1403;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=1622;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=996;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=5828;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=6673;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=6698;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=2588;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=5378;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=16141;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=16159;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=876;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=1957;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=1961;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=1984;	$to[$i]=151;	$tocount[$i]=1;		$togold[$i]=0;	$i++;
$from[$i]=22805;	$to[$i]=0;	$tocount[$i]=1;		$togold[$i]=100000000;	$i++;
$from[$i]=23741;	$to[$i]=0;	$tocount[$i]=1;		$togold[$i]=100000000;	$i++;
$from[$i]=23743;	$to[$i]=0;	$tocount[$i]=1;		$togold[$i]=100000000;	$i++;
$from[$i]=13338;	$to[$i]=0;	$tocount[$i]=1;		$togold[$i]=100000000;	$i++;
$from[$i]=12750;	$to[$i]=0;	$tocount[$i]=1;		$togold[$i]=100000000;	$i++;

for($c=0;$c<$i;$c++)
{
	$get_query = "select ownerguid from playeritems where entry=".$from[$c]." order by ownerguid";
	$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi)." $get_query ");
	$prev_guid = 0;
	while($row=mysql_fetch_row($get_res))
	{
		$get_res2=mysql_query("insert into mailbox_insert_queue values (".$row[0].",".$row[0].",'Vote rewards have been swapped','Removed one of your vote items and here is the price.',61,".$togold[$c].",".$to[$c].",".$tocount[$c].")",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		if($prev_guid!=$row[0])
		{
			$prev_guid=$row[0];
			$get_res2=mysql_query("delete from playeritems where ownerguid=${row[0]} and entry=".$from[$c]."",$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
		}
	}
}
?>