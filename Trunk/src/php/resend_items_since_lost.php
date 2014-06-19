<?php

$dbhost="localhost";
$dbuname="stefanzaza112233";
$dbupass="stefan18051805";
$dbname="donate";

$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239"); 

//fetch the donations that we need to resend
$get_query = "select char_guid,info from logs where `date` >= '2009-08-04' and char_guid!=0";
$get_res=mysql_query($get_query,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
while($row=mysql_fetch_row($get_res) )
{
	//get the reward items for this reward name
	$pguid = $row[0];
	$get_query2 = "select gold,item1,item2,item3,item4,item5,item6,item7,item8 from rewards where name='".mysql_escape_string($row[1])."'";
	$get_res2=mysql_query($get_query2,$dbi) or die(" Q200611231501411 ".mysql_error($dbi));
	$row2=mysql_fetch_row($get_res2);
	//generate mail for each dontation
	$send_count=0;
	for($i=1;$i<=8;$i++)
	if( $row2[$i] != 0 )
	{
		echo "INSERT INTO mailbox_insert_queue (sender_guid,receiver_guid,subject,body,stationary,item_id,item_stack) VALUES ('".$pguid."','".$pguid."','Donation resend','Due to some technical difficulties there was a rollback on player items database. We are resending your donations. Please create a contact-us ticket in case we should do more to fix this issue','61','".$row2[$i]."','1');<br>";
		$send_count++;
	}
	//send gold
	if( $row2[0] )
	{
		echo "INSERT INTO mailbox_insert_queue (sender_guid,receiver_guid,subject,body,stationary,money) VALUES ('".$pguid."','".$pguid."','Donation resend','Due to some technical difficulties there was a rollback on player items database. We are resending your donations. Please create a contact-us ticket in case we should do more to fix this issue','61','".$row2[0]."');<br>";
		$send_count++;
	}
	if( $send_count == 0 )
		echo "!!!!!!! there was an error and nothing was sent !<br>";
}


?>