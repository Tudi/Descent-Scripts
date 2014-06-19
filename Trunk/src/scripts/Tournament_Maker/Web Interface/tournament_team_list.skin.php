<?php
//direct acces of a skin is not allowed
if( !$dbi )
	die("Can't acces page directly");

//we can only pend for joins if we do not have a team already
$query = "select team_leader_guid from tournament_team_settings where team_leader_guid='$s_user_guid' and tournament_id='$s_user_tournament'";
//echo $query."<br>";
$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
list($has_team)=mysql_fetch_row($result);
if( $has_team )
	$error_msg = "You already have a team. Can't have team and join other team at the same time";

$query = "select team_id from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament'";
//echo $query."<br>";
$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
list($in_team_id)=mysql_fetch_row($result);

if($error_msg)
{
	?>
	<table width="970"  border="0" align="center" cellpadding="0" cellspacing="0">
	   <tr>
			<td>
				<?=$error_msg;?>
		</td>
	</tr>
</table>
	<?php
	die();
}

if( $SubscribeFor!="" )
{
	//we will try subscribe for this team. They will need to accept us to be able to actually join it
	$list_sql="select is_pending_to_join_team from tournament_players where team_id='$SubscribeFor' and player_guid='$s_user_guid' and tournament_id='$s_user_tournament'";
	$result1=mysql_query($list_sql,$dbi) or die("200901261641");
	list($is_pending_to_join_team )=mysql_fetch_row($result1);
	if( $is_pending_to_join_team == "" )
	{
		$result=mysql_query("Insert into tournament_players (team_id,player_guid,tournament_id,is_pending_to_join_team,is_pending_to_accept_team_invite,strength_points) values('$SubscribeFor','$s_user_guid','$s_user_tournament','1','0','".Get_player_strength($s_user_guid)."')",$dbi) or die("Q 200501180949".mysql_error($dbi));
	}
}
if( $UnSubscribeFor!="" )
{
	$result=mysql_query("delete from tournament_players where player_guid='$s_user_guid' and team_id='$UnSubscribeFor' and tournament_id='$s_user_tournament'",$dbi) or die("Q 200501180949".mysql_error($dbi));
}
if( $FullJoin!="" )
{
	//check if this team can have more players
	$list_sql="select count(*) from tournament_players where team_id='$SubscribeFor' and tournament_id='$s_user_tournament' and is_pending_to_join_team=0 and is_pending_to_join_team=0";
	$result1=mysql_query($list_sql,$dbi) or die("200901261641");
	list($team_member_count )=mysql_fetch_row($result1);

	//see the limit for this tournament
	if( $s_user_max_team_members<=$team_member_count)
		Alert("This team is full atm. Try to negotaite ingame with them for an open spot");
	else
	{
		//leave other joined teams if there is any
		$query="delete from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament' and is_pending_to_accept_team_invite=0 and is_pending_to_join_team=0";
		$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
		//try to full join this team
		$query="update tournament_players set is_pending_to_join_team=0,is_pending_to_accept_team_invite=0 where team_id='$FullJoin' and player_guid='$s_user_guid' and tournament_id='$s_user_tournament' and is_pending_to_accept_team_invite=1";
		$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi).$query);
	}
}
if( $NoPendings )
{
	$result=mysql_query("delete from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament' and is_pending_to_join_team=1",$dbi) or die("Q 200501180949".mysql_error($dbi));
}
$file_name="tournament_team_list.php";
$list_name_text="tournament_team_settings";
$new_entry_text=" New tournament team";
$edit_file_name="tournament_team_join.php";
$field_prefix="tournament_team_list_";
$nr_columns_listed="1";

$initStartLimit = 0;
$limitPerPage = 10;

$startLimit = $_REQUEST['startLimit'];
$numberOfRows = $_REQUEST['rows'];
$sortBy = $_REQUEST['sortBy'];
$sortOrder = $_REQUEST['sortOrder'];

if ($startLimit=="")
{
		$startLimit = $initStartLimit;
}

if ($numberOfRows=="")
{
		$numberOfRows = $limitPerPage;
}

if ($sortOrder=="")
{
		$sortOrder  = "DESC";
}
if ($sortOrder == "DESC") { $newSortOrder = "ASC"; } else  { $newSortOrder = "DESC"; }
$limitQuery = " LIMIT ".$startLimit.",".$numberOfRows;
$nextStartLimit = $startLimit + $limitPerPage;
$previousStartLimit = $startLimit - $limitPerPage;

if ($sortBy!="")
{
		$orderByQuery = " ORDER BY ".$sortBy." ".$sortOrder;
}
?>
<link href="css/style.css" rel="stylesheet" type="text/css">


<table width="870"  border="0" align="center" cellpadding="0" cellspacing="0">
   <tr>
    <td>
	<table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr bgcolor="#66990" class="clsListheaderText01">	
          
          <td height="20" class="clsListheaderText01">
				<a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Team name&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
					Team name
	            </a>
			</td>
          <td height="20" class="clsListheaderText01">
				<a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy= matches_won_total&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
					Matches won
	            </a>
			</td>
          <td height="20" class="clsListheaderText01">Team Info </td>
          <td height="20" class="clsListheaderText01">Waiting on team accept </td>
          <td height="20" class="clsListheaderText01">Waiting to approve team invite </td>
          <td>Actions</td>
        </tr>
        <?php
$where_query=" where tournament_id='$s_user_tournament' and is_open_team=1 ";
$list_sql="select id,team_name,team_desc_public,matches_won_total from tournament_team_settings ".$where_query;
$list_sql_error="Q 200901260338_1";
$count_list_sql="select count(id) from tournament_team_settings".$where_query;
$count_list_sql_error="Q 200901260338_2";


$result=mysql_query($count_list_sql,$dbi) or die($count_list_sql_error);
list($max_result) = mysql_fetch_row($result);

#$list_sql.=" where ";
$list_sql.= $orderByQuery.$limitQuery;


$result=mysql_query($list_sql,$dbi) or die($list_sql_error.$list_sql);
while($list_row=mysql_fetch_array($result, MYSQL_ASSOC))
{
	foreach($list_row as $list_row_name=>$list_row_value)
	{
		$temp=$field_prefix.$list_row_name;
		$$temp=$list_row_value;
	}//end of foreach
	
	//check if we are waiting team to accept us
	$list_sql="select is_pending_to_join_team,is_pending_to_accept_team_invite from tournament_players where team_id='$tournament_team_list_id' and player_guid='$s_user_guid' ";
//echo $list_sql."<br>";
	$result1=mysql_query($list_sql,$dbi) or die("200901261641");
	list($is_pending_to_join_team,$is_pending_to_accept_team_invitem )=mysql_fetch_row($result1);
//echo $is_pending_to_join_team." $is_pending_to_accept_team_invitem<br>";
	//do not list team members to be able to join again
	if(  $is_pending_to_join_team!="" && $is_pending_to_join_team == 0 && $is_pending_to_accept_team_invite == 0 )
	{
//echo "iszero<br>";
		continue;
	}
	if( $is_pending_to_join_team == "" )
		$is_pending_to_join_team=$is_pending_to_accept_team_invite=0;
?>
        <tr>
          
          <td height="30" class="clsListText01"><?=$tournament_team_list_team_name?></td>
          <td height="30" class="clsListText01"><?=$tournament_team_list_matches_won_total?></td>
		   <td height="30" class="clsListText01">
				<?=$tournament_team_list_team_desc_public?>
		   </td>
		   <td height="30" class="clsListText01"><?=$is_pending_to_accept_team_invite?></td>
		   <td height="30" class="clsListText01"><?=$is_pending_to_join_team?></td>
          
          <td align="right">
			<?php if( $is_pending_to_join_team==0 ) { ?>
			<input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=tournament_team_list.skin.php&SubscribeFor=<?=$tournament_team_list_id?>'" value="Pending Join" />
			<?php } if( $is_pending_to_join_team==1 ) { ?>
	        <input name="button2" type="button" class="clsButton01" 
			onclick="document.location='index.php?skin=tournament_team_list.skin.php&UnSubscribeFor=<?=$tournament_team_list_id?>'" value="Stop Pending" />
			<?php } ?>
		    <input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=my_team_normal.skin.php&team_id=<?=$tournament_team_list_id?>'" value="View Team" />
			<?php if( $is_pending_to_accept_team_invite ) { ?>
		    <input name="button2" type="button" class="clsButton01" onclick="<?=$field_prefix?>FullJoin(<?=$tournament_team_list_id?>)" value="Join Team" />
			<?php } ?>
          </td>
        </tr>
        <tr>
          <td colspan="<?=$nr_columns_listed+4?>" height="1" bgcolor="#CCCCCC"><img src="/images/blank.gif" width="1" height="1" /></td>
        </tr>
        <?php
}//end of while
?>
      </table></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
	  <table width="100%"  border="0" cellspacing="0" cellpadding="0">
<tr>
	  	  <td width="169"> 
<input name="button" type="button" class="clsButton01" onclick="document.location='index.php?skin=new_team.skin.php'" value="New team" />
<input name="button" type="button" class="clsButton01" onclick="document.location='index.php?skin=tournament_team_list.skin.php&NoPendings=1'" value="Clear all Joins" />
		</td>
	
          <td width="182" class="clsMainLink01"> 
            <?php
	if($startLimit>0)
	{
?>
            <a href="<?=$PHP_SELF?>?sortBy=<?=$sortBy?>&sortOrder=<?=$sortOrder?>&startLimit=<?=$previousStartLimit?>&rows=<?=$limitPerPage; ?>" class="clsMainLink01" style="font-weight: bold">&laquo; 
            Previous </a> 
            <?php
	}//end of previous
?>
          </td>
          
        <td width="572" align="center"  class="clsMainLink01">
          <?php
				
				$max_page_count = (int)($max_result/$limitPerPage)+1;
				if ($max_result % $limitPerPage==0) $max_page_count--;
				
				if ($max_page_count>1)  //display only if more pages than one
				{
				
				$pages_to_display=15;

				$start_page =  ((int)($startLimit/($pages_to_display*$limitPerPage)) ) *$pages_to_display +1;
				$end_page = $start_page+$pages_to_display-1;
				if ($end_page>$max_page_count) $end_page=$max_page_count;
				
				if ($start_page>1)
				{
					?>
          <a href="<?=$PHP_SELF?>?sortBy=<?=$sortBy?>&sortOrder=<?=$sortOrder?>&startLimit=<?php echo ($start_page-$pages_to_display-1)*$limitPerPage?>&rows=<?=$limitPerPage; ?>" class="clsMainLink01" style="font-weight: bold">&laquo;</a> 
          <?php
				}
				
				for($i=$start_page;$i<=$end_page;$i++)
				{
					if ( ($i-1)*$limitPerPage == $startLimit )
					{
						?>
          <a href="<?=$PHP_SELF?>?sortBy=<?=$sortBy?>&sortOrder=<?=$sortOrder?>&startLimit=<?php echo ($i-1)*$limitPerPage;?>&rows=<?=$limitPerPage; ?>" class="clsMainLink01" style="font-weight: bold"> 
          <?=$i?>
          </a> 
          <?php
					}
					else
					{
						?>
          <a href="<?=$PHP_SELF?>?sortBy=<?=$sortBy?>&sortOrder=<?=$sortOrder?>&startLimit=<?php echo ($i-1)*$limitPerPage;?>&rows=<?=$limitPerPage; ?>" class="clsPageNumberLink01" style="font-weight: bold"> 
          <?=$i?>
          </a> 
          <?php
					}
				}
				
				if ($end_page<$max_page_count)
				{
					?>
          <a href="<?=$PHP_SELF?>?sortBy=<?=$sortBy?>&sortOrder=<?=$sortOrder?>&startLimit=<?php echo ($end_page)*$limitPerPage?>&rows=<?=$limitPerPage; ?>" class="clsMainLink01" style="font-weight: bold"> 
          &raquo; </a> 
          <?php
					
				}
				
				} //max page count > 1, if not do not display page selector
				?>
        </td>
          
        <td width="119" align="right" class="clsMainLink01">
          <?php
	if($nextStartLimit<$max_result)
	{
?>
          <a href="<?=$PHP_SELF?>?sortBy=<?=$sortBy?>&sortOrder=<?=$sortOrder?>&startLimit=<?=$nextStartLimit?>&rows=<?=$limitPerPage; ?>" class="clsMainLink01" style="font-weight: bold">
          Next
          &raquo;</a> 
          <?php
	}//end of previous
?>
        </td>
	
  </tr>
</table>
</td>
</tr>
</table>
<script language="JavaScript">
function <?=$field_prefix?>FullJoin(id)
{
	if(confirm('You can only join one team. Join this one ?')) 
		document.location="index.php?skin=tournament_team_list.skin.php&FullJoin="+id;
	else 
		return;
}
 
</script>
