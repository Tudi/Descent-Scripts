<?php

$BACK_BUTTON_ICON	= 0;
$MAX_ALLOWED_MENUS_PER_PAGE	= 10;
$TEXT_ID_START_INDEX	= 20000;

/*
CREATE TABLE IF NOT EXISTS gossip_menu (
  id int(10) unsigned NOT NULL auto_increment,
  page_text varchar(1500) NOT NULL ,
  page_text_id int(10) unsigned NOT NULL DEFAULT '0' ,
  menu_name varchar(1500) NOT NULL ,
  prev_page_id int(10) unsigned NOT NULL DEFAULT '0' ,
  menu_icon mediumint(10) unsigned NOT NULL DEFAULT '0' ,
  popup_message varchar(1500) NOT NULL DEFAULT '0' ,
  popup_required_gold int(10) unsigned NOT NULL DEFAULT '0' ,
  PRIMARY KEY (id),
  UNIQUE KEY id (id),
   KEY id_2 (id)
);
*/
foreach($_REQUEST as $foreachname=>$foreachvalue)
	$$foreachname=$foreachvalue;

//ah crap man, darn comfigs
if( $PHP_SELF == "" )
	$PHP_SELF = "menu_generator.php";
	
//conn settings
$dbhost="localhost";
$dbuname="root";
//$dbupass="lofaszka";
$dbupass="";
$dbname="temp";

//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));
 
if( $prev_page_id == "" )
{
	$get_query3 = "select prev_page_id from gossip_menu order by prev_page_id asc limit 0,1"; 
	$get_res = mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	list($prev_page_id)=mysql_fetch_row($get_res);
}

///////////////////////// add ///////////////////////////
if( $menu_name_new != "" )
{
	$get_query3 = "insert into gossip_menu (menu_name,prev_page_id,menu_icon,popup_message,popup_required_gold) values 
	('".mysql_escape_string($menu_name_new)."'
	,'".mysql_escape_string($prev_page_id)."'
	,'".mysql_escape_string($menu_icon_new)."'
	,'".mysql_escape_string($menu_popup_new)."'
	,'".mysql_escape_string($menu_gold_new)."'
	)"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
}
///////////////////////// del ///////////////////////////
if( $action == "del" )
{
	$get_query3 = "delete from gossip_menu where id=$id"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
}
///////////////////////// update ///////////////////////////
if( $action == "update" )
{
	$get_query3 = "replace into gossip_menu (id,menu_name,prev_page_id,menu_icon,popup_message,popup_required_gold) values 
	(
	'".mysql_escape_string($id)."'
	,'".mysql_escape_string($menu_name_up)."'
	,'".mysql_escape_string($prev_page_id)."'
	,'".mysql_escape_string($menu_icon_up)."'
	,'".mysql_escape_string($menu_popup_up)."'
	,'".mysql_escape_string($menu_gold_up)."'
	)"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
}
///////////////////////// update page text///////////////////////////
if( $action == "update_page_text" )
{
	$get_query3 = "update gossip_menu set page_text='".mysql_escape_string($page_text)."' where prev_page_id='$prev_page_id'"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
}
///////////////////////// link new page ///////////////////////////
if( $action == "attach" )
	$prev_page_id = $id;
///////////////////////// prev page ///////////////////////////
if( $action == "prev" )
{
	$get_query3 = "select prev_page_id from gossip_menu where id='".mysql_escape_string($prev_page_id)."' order by id limit 0,1"; 
	$get_res = mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	list($prev_page_id)=mysql_fetch_row($get_res);
	if( $prev_page_id == "" )
	{
		$get_query3 = "select prev_page_id from gossip_menu order by prev_page_id asc limit 0,1"; 
		$get_res = mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
		list($prev_page_id)=mysql_fetch_row($get_res);
	}
}

$get_query3 = "SELECT page_text FROM gossip_menu where prev_page_id='$prev_page_id'"; 
$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
list($page_text)=mysql_fetch_row($get_res);
 ?>
 Tips : <br>
 - You need to manually replace "\n"(new line) with $B <br>
 - Design on paper manu order, you cannot insert a menu between existing menus(comming soon) <br>
 - you will need to run sql queries also in case you are using page texts. Using page text reduces network trafic a lot <br>
 - there will be only 1 "back" button / page. In case you wish to have page text only then create a back button first <br>
 - page cannot contain only 1 button. Client will auto click it. An empty button is added in case page contains only 1 button <br>
 - you probably want to generate C code and copy paste it into a gossip template script. Also attach the script to some NPC with gossip flag<br>
 - using button text instead page text avoids client caching the text. This means text is downloaded every time a client requests it<br>
 <table border="1">
	<tr>
		<form name="get_data" action="<?=$PHP_SELF?>" method="post">
			<input type="hidden" name="prev_page_id" value="<?=$prev_page_id?>">
			<input type="hidden" name="action" value="update_page_text">
			<td colspan=6 >page text:<input type="text" name="page_text" style="width:800" value="<?=$page_text?>"></td>
			<td><input type="submit" value="update">
		</form>
	</tr>
	<?php
	$get_query3 = "SELECT id,menu_name,menu_icon,popup_message,popup_required_gold FROM gossip_menu where prev_page_id='$prev_page_id'"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	$row_counter = 0;
	while( list($id,$menu_name,$menu_icon,$popup_message,$popup_required_gold)=mysql_fetch_row($get_res) )
	{
	?>
	<form name="get_data" action="<?=$PHP_SELF?>" method="post">
	<input type="hidden" name="prev_page_id" value="<?=$prev_page_id?>">
	<input type="hidden" name="action" value="update">
	<input type="hidden" name="id" value="<?=$id?>">
	<tr>
		<td>menu:<input type="text" name="menu_name_up" style="width:180" value="<?=$menu_name?>"></td>
		<td>icon:<input type="text" name="menu_icon_up" style="width:30" value="<?=$menu_icon?>"></td>
		<td>popup:<input type="text" name="menu_popup_up" style="width:180" value="<?=$popup_message?>"></td>
		<td>popup gold:<input type="text" name="menu_gold_up" style="width:60" value="<?=$popup_required_gold?>"></td>
		<td><input type="submit" value="update"></td>
		<td><a href="<?=$PHP_SELF?>?prev_page_id=<?=$prev_page_id?>&action=del&id=<?=$id?>">del</a></td>
		<td><a href="<?=$PHP_SELF?>?prev_page_id=<?=$prev_page_id?>&action=attach&id=<?=$id?>">link new page</a></td>
	</tr>
	</form>
	<?php 
	$row_counter++;
	} 
	if( $row_counter < $MAX_ALLOWED_MENUS_PER_PAGE )
	{
	?>
	<form name="get_data" action="<?=$PHP_SELF?>" method="post">
	<input type="hidden" name="prev_page_id" value="<?=$prev_page_id;?>">
	<tr>
		<td>menu:<input type="text" name="menu_name_new" style="width:180" value=""></td>
		<td>icon:<input type="text" name="menu_icon_new" style="width:30" value=""></td>
		<td>popup:<input type="text" name="menu_popup_new" style="width:180" value=""></td>
		<td>popup gold:<input type="text" name="menu_gold_new" style="width:60" value=""></td>
		<td><input type="submit" value="add">
	</tr>
	</form>
	<?php } ?>
	<tr>
		<td><a href="<?=$PHP_SELF?>?prev_page_id=<?=$prev_page_id;?>&action=prev">prev page</a></td>
	</tr>
</table>

<br><a href="<?=$PHP_SELF?>?prev_page_id=<?=$prev_page_id;?>&action=genC">Generate C code</a><br>

<?php
///////////////////////// gen C code ///////////////////////////
if( $action == "genC" )
{
	//generate NPC text ids
	$get_query3 = "SELECT page_text,prev_page_id,id FROM gossip_menu group by page_text"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	$page_text_id = $TEXT_ID_START_INDEX;
	while( list($page_text,$lprev_page_id,$lid)=mysql_fetch_row($get_res) )
		if($page_text!="")
		{
			echo "insert into npc_text (entry,text0_0,prob0) values (".($page_text_id).",'".mysql_escape_string($page_text)."',1);<br>";
			mysql_query("update gossip_menu set page_text_id='$page_text_id' where id=$lid",$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
			$page_text_id++;
		}
			
	//get the smallest menu block number
	$get_query3 = "select prev_page_id from gossip_menu order by prev_page_id asc limit 0,1"; 
	$get_res = mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	list($smallest_menu_block)=mysql_fetch_row($get_res);
	
	echo "Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;<br>";
	echo "if(pCreature==NULL)<br>";
	echo "return;<br>";
	echo "switch(IntId)<br>";
	echo "{<br>";
	print_menu_block_C( $smallest_menu_block,"Main",$smallest_menu_block );
	$get_query3 = "SELECT id,menu_name,prev_page_id FROM gossip_menu"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	while( list($id,$menu_name,$lprev_page_id)=mysql_fetch_row($get_res) )
	{
//		$id = $id + 1;	//avoid 0 ids
		print_menu_block_C( $id,$menu_name,$lprev_page_id );
	}
	echo "&nbsp;&nbsp;&nbsp;&nbsp; case 666: Plr->CloseGossip(); break <br>";
	echo "};<br>";
}
function print_menu_block_C($id,$menu_name,$lprev_page_id)
{
	global $dbi,$TEXT_ID_START_INDEX,$BACK_BUTTON_ICON;
	//check if have submenus
//	$get_query3 = "SELECT count(*) FROM gossip_menu where prev_page_id='$id' and menu_name != 'back' and menu_name != 'Back'"; 
	$get_query3 = "SELECT count(*) FROM gossip_menu where prev_page_id='$id' "; 
	$get_res2=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	list($has_submenus)=mysql_fetch_row($get_res2);
	$tab_size="&nbsp;&nbsp;&nbsp;&nbsp;";
	//this is default behavior for a menu that has submenus 
	if( $has_submenus )
	{
		echo "$tab_size case $id: //$menu_name<br>";
		echo "$tab_size {<br>";
		echo "$tab_size $tab_size GossipMenu *Menu;<br>";
		echo "$tab_size $tab_size objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);<br>";
		$get_query3 = "SELECT page_text_id FROM gossip_menu where page_text!=\"\" and prev_page_id='$id'"; 
		$get_res3=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
		list($page_text_id)=mysql_fetch_row($get_res3);
		if( $page_text_id > 0 )
			echo "$tab_size $tab_size Menu->SetTextID( ".($page_text_id)." );<br>";
		$get_query3 = "SELECT id,menu_name,menu_icon,popup_message,popup_required_gold FROM gossip_menu where prev_page_id='$id'"; 
		$get_res2=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
		$menu_counter = 0;
		while( list($sub_id,$sub_menu_name,$sub_menu_icon,$popup_message,$popup_required_gold)=mysql_fetch_row($get_res2) )
		{
			if( $sub_menu_name == "Back" || $sub_menu_name == "back")
				continue;
			if( $popup_message != "" )
				echo "$tab_size $tab_size Menu->AddMenuItem($sub_menu_icon, \"$sub_menu_name\",0,0,$popup_message,$popup_required_gold,0,$sub_id);<br>";	
			else
				echo "$tab_size $tab_size Menu->AddItem($sub_menu_icon, \"$sub_menu_name\",$sub_id);<br>";	
			$menu_counter++;
		}
		if( $id != $lprev_page_id )
		{
			//add empty menu in case there is only text on this page to avoid client auto clicking on menu 0 and taking us back to previous page
			//i wonder if there is a better way to do this
			echo "$tab_size $tab_size Menu->AddItem($BACK_BUTTON_ICON, \"Back\",$lprev_page_id);<br>";	
			if( $menu_counter == 0 )
				echo "$tab_size $tab_size Menu->AddItem($BACK_BUTTON_ICON, \"Close\",666);<br>";	
		}
		echo "$tab_size $tab_size Menu->SendTo(Plr);<br>";
		echo "$tab_size }break;<br>";
	}
}
?>

The menu tree : <br>
<?php
//get the smallest menu block number
$get_query3 = "select prev_page_id from gossip_menu order by prev_page_id asc limit 0,1"; 
$get_res = mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
list($smallest_menu_block)=mysql_fetch_row($get_res);
?>

<table border='1'>
<?php print_menu_block_HTML($smallest_menu_block,0); ?>
</table>

<?php
function print_menu_block_HTML($lprev_page_id,$depth)
{
	global $dbi,$prev_page_id;
	$get_query3 = "SELECT id,menu_name FROM gossip_menu where prev_page_id='$lprev_page_id'"; 
	$get_res=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
	$row_counter = 0;
	while( list($id,$menu_name)=mysql_fetch_row($get_res) )
	{
//		if($row_counter!=0)
		{?>
			<tr>
		<?php
		}
		
		for($i=0;$i<$depth;$i++)
		{ ?>
			<td>&nbsp;</td>
		<?php } ?>
			
		<td>
			<?php				
				if($lprev_page_id==$prev_page_id)
					$fname = "<b style=\"background-color: #00ED00;\">$menu_name</b>";
				else
					$fname = "$menu_name";
				
			?>
			<a href="<?=$PHP_SELF?>?prev_page_id=<?=$lprev_page_id;?>"><?=$fname;?></a>
		</td>
		
		<?php
//		if($row_counter!=0)
		{?>
			</tr>
		<?php
		}
		//in case this menu has submenus then print out those too
		$get_query3 = "SELECT count(*) FROM gossip_menu where prev_page_id='$id'"; 
		$get_res2=mysql_query($get_query3,$dbi) or die(" Q2006112315011 ".mysql_error($dbi)." $get_query3 ");
		list($has_submenus)=mysql_fetch_row($get_res2);
		if( $has_submenus )
			print_menu_block_HTML($id,$depth+1);
		$row_counter++;
	}
}
?>