<?php
//direct acces of a skin is not allowed
if( !$dbi )
	die("Can't acces page directly");

//we can only pend for joins if we do not have a team already
$query = "select id from tournament_team_settings where team_leader_guid='$s_user_guid' and tournament_id='$s_user_tournament'";
//echo $query."<br>";
$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
list($team_id)=mysql_fetch_row($result);
if( $team_id=="" )
	$error_msg = "You do not have a team to accept new members.";

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

if( $AcceptPlayer != "")
{
	//we will try subscribe for this team. They will need to accept us to be able to actually join it
	$query="update tournament_players set is_pending_to_accept_team_invite=1 where team_id='$team_id' and player_guid='$AcceptPlayer' and tournament_id='$s_user_tournament'";
	$result1=mysql_query($query,$dbi) or die("200901261641");
}
if( $KickPlayer != "" )
{
	//we will try subscribe for this team. They will need to accept us to be able to actually join it
	$query="update tournament_players set is_pending_to_accept_team_invite=0 where team_id='$team_id' and player_guid='$KickPlayer' and tournament_id='$s_user_tournament'";
	$result1=mysql_query($query,$dbi) or die("200901261641");
}
if( $NoPendings != "" )
{
	//we will try subscribe for this team. They will need to accept us to be able to actually join it
	$query="update tournament_players set is_pending_to_accept_team_invite=0 where team_id='$team_id' and tournament_id='$s_user_tournament'";
	$result1=mysql_query($query,$dbi) or die("200901261641");
}
$file_name="accept_team_pend_join.php";
$list_name_text="tournament_team_settings";
$new_entry_text=" New tournament team";
$edit_file_name="tournament_team_join.php";
$field_prefix="accept_team_pend_join_";
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


<table width="970"  border="0" align="center" cellpadding="0" cellspacing="0">
   <tr>
    <td>
	<table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr bgcolor="#66990" class="clsListheaderText01">	
          
          <td height="20" class="clsListheaderText01">Player name</td>
          <td height="20" class="clsListheaderText01">Matches played</td>
          <td height="20" class="clsListheaderText01">Matches won</td>
          <td height="20" class="clsListheaderText01">Strength points</td>
          <td height="20" class="clsListheaderText01">Invited to team</td>
          <td>Actions</td>
        </tr>
        <?php
$where_query=" where tournament_id='$s_user_tournament' and team_id='$team_id' and player_guid!='$s_user_guid' and is_pending_to_join_team=1";
$list_sql="select player_guid,strength_points,is_pending_to_accept_team_invite from tournament_players ".$where_query;
$list_sql_error="Q 200901260338_1";
$count_list_sql="select count(*) from tournament_players".$where_query;
$count_list_sql_error="Q 200901260338_2";


$result=mysql_query($count_list_sql,$dbi) or die($count_list_sql_error.$count_list_sql);
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
?>
        <tr>
          
          <td height="30" class="clsListText01">
			   <?php
				//check if we are waiting team to accept us
				$list_sql="select name from characters where guid='$accept_team_pend_join_player_guid' ";
				$result1=mysql_query($list_sql,$dbi) or die("200901261641");
				list($name)=mysql_fetch_row($result1);
				?>
				<?=$name?>
		  </td>
          <td height="30" class="clsListText01">
			   <?php
				//check if we are waiting team to accept us
				$list_sql="select matches_played,matches_won from tournament_player_info where player_guid='$accept_team_pend_join_guid' ";
				$result1=mysql_query($list_sql,$dbi) or die("200901261641");
				list($matches_played,$matches_won )=mysql_fetch_row($result1);
				if( $matches_played=="")
					$matches_played=$matches_won = 0;
				?>
				<?=$matches_played?>
		  </td>
          <td height="30" class="clsListText01">
			  <?=$matches_won?>
		  </td>
          <td height="30" class="clsListText01">
			  <?=$accept_team_pend_join_strength_points?>
		  </td>
		   <td height="30" class="clsListText01">
				<?=$accept_team_pend_join_is_pending_to_accept_team_invite?>
		   </td>         
          <td align="right">
			<?php if( $accept_team_pend_join_is_pending_to_accept_team_invite == 0 ) { ?>
			<input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=accept_team_pend_join.skin.php&AcceptPlayer=<?=$accept_team_pend_join_player_guid?>'" value="Accept member" />
			<?php } if( $accept_team_pend_join_is_pending_to_accept_team_invite == 1 ) { ?>
	        <input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=accept_team_pend_join.skin.php&KickPlayer=<?=$accept_team_pend_join_player_guid?>'" value="Kick member" />
			<?php } ?>
          </td>
        </tr>
        <tr>
          <td colspan="<?=$nr_columns_listed+5?>" height="1" bgcolor="#CCCCCC"><img src="/images/blank.gif" width="1" height="1" /></td>
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
<input name="button" type="button" class="clsButton01" onclick="document.location='index.php?skin=accept_team_pend_join.skin.php&NoPendings=1'" value="Clear all Joins" />
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
