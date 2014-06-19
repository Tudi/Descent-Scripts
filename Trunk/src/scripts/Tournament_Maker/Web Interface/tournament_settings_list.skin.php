<?php
	//direct acces of a skin is not allowed
	if( !$dbi )
		die("Can't acces page directly");
$file_name="tournament_settings_list.skin.php";
$list_name_text="tournament_settings";
$new_entry_text=" New tournament_settings";
$edit_file_name="tournament_settings.skin.php";
$field_prefix="tournament_settings_list_";
$nr_columns_listed="7";
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

$delete=mysql_escape_string($delete);
if($delete>0)
{
	$delete_sql="delete from tournament_settings where id=$delete";
	$delete_sql_error="Q 200902030503_0";
	$result=mysql_query($delete_sql,$dbi) or die($delete_sql_error);
	$delete_sql="delete from tournament_team_settings where tournament_id=$delete";
	$delete_sql_error="Q 200902030503_0";
	$result=mysql_query($delete_sql,$dbi) or die($delete_sql_error);
	$delete_sql="delete from tournament_players where tournament_id=$delete";
	$delete_sql_error="Q 200902030503_0";
	$result=mysql_query($delete_sql,$dbi) or die($delete_sql_error);
}//end of delete
?>
<link href="css/style.css" rel="stylesheet" type="text/css">


<table width="100%" border="0" cellspacing="0" cellpadding="0">
   <tr>
    <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
        <tr bgcolor="#66990" class="clsListheaderText01">
          
          <td height="20" class="clsListheaderText01"><a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Id&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
            Id
            </a> </td>
          
          <td height="20" class="clsListheaderText01"><a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Tournament name&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
            Tournament name
            </a> </td>
          
          <td height="20" class="clsListheaderText01"><a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Min teams to start&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
            Min teams to start
            </a> </td>
          
          <td height="20" class="clsListheaderText01"><a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Max chars per team&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
            Max chars per team
            </a> </td>
          
          <td height="20" class="clsListheaderText01"><a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Inscription starts&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
            Inscription starts
            </a> </td>
          
          <td height="20" class="clsListheaderText01"><a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Inscription ends&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
            Inscription ends
            </a> </td>
          
          <td height="20" class="clsListheaderText01"><a class="clsListheaderText01" href="<?=$PHP_SELF?>?sortBy=Tournament starts&sortOrder=<?=$newSortOrder?>&startLimit=<?=$startLimit?>&rows=<?=$limitPerPage; ?>">
            Tournament starts
            </a> </td>
          
          <td>&nbsp;</td>
        </tr>
        <?php
  $list_sql="select id,tournament_name,min_teams_to_start,max_chars_per_team,inscription_starts,inscription_ends,tournament_starts from tournament_settings";
$list_sql_error="Q 200902030503_1";
$count_list_sql="select count(id) from tournament_settings";
$count_list_sql_error="Q 200902030503_2";


$result=mysql_query($count_list_sql,$dbi) or die($count_list_sql_error);
list($max_result) = mysql_fetch_row($result);

#$list_sql.=" where ";
$list_sql.= $orderByQuery.$limitQuery;


$result=mysql_query($list_sql,$dbi) or die($list_sql_error);
while($list_row=mysql_fetch_array($result, MYSQL_ASSOC))
{
	foreach($list_row as $list_row_name=>$list_row_value)
	{
		$temp=$field_prefix.$list_row_name;
		$$temp=$list_row_value;
	}//end of foreach
	
	$tournament_settings_list_inscription_starts = date("Y-M-d h:i",$tournament_settings_list_inscription_starts);
	$tournament_settings_list_inscription_ends = date("Y-M-d h:i",$tournament_settings_list_inscription_ends);
	$tournament_settings_list_tournament_starts = date("Y-M-d h:i",$tournament_settings_list_tournament_starts);

?>
        <tr>
          
          <td height="30" class="clsListText01"><?=$tournament_settings_list_id?></td>
          
          <td height="30" class="clsListText01"><?=$tournament_settings_list_tournament_name?></td>
          
          <td height="30" class="clsListText01"><?=$tournament_settings_list_min_teams_to_start?></td>
          
          <td height="30" class="clsListText01"><?=$tournament_settings_list_max_chars_per_team?></td>
          
          <td height="30" class="clsListText01"><?=$tournament_settings_list_inscription_starts?></td>
          
          <td height="30" class="clsListText01"><?=$tournament_settings_list_inscription_ends?></td>
          
          <td height="30" class="clsListText01"><?=$tournament_settings_list_tournament_starts?></td>
          
          <td align="right">
		  <input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=sendrewards.skin.php&id=<?=$tournament_settings_list_id?>'" value="Send Rewards" />
		  <input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=player_rankings.skin.php&id=<?=$tournament_settings_list_id?>'" value="Gen Score" />
		  <input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=list_team_members.skin.php&id=<?=$tournament_settings_list_id?>'" value="View teams" />
		  <input name="button2" type="button" class="clsButton01" onclick="document.location='index.php?skin=recycle_tournament.skin.php&id=<?=$tournament_settings_list_id?>'" value="Recycle" />
          <input name="button2" type="button" class="clsButton01" onclick="<?=$field_prefix?>editfunc(<?=$tournament_settings_list_id?>)" value="Edit" />
            <input name="button2" type="button" class="clsButton02" onclick="<?=$field_prefix?>deletefunc(<?=$tournament_settings_list_id?>)" value="Delete" />
          </td>
        </tr>
        <tr>
          <td colspan="<?=$nr_columns_listed+2?>" height="1" bgcolor="#CCCCCC"><img src="/images/blank.gif" width="1" height="1" /></td>
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
<input name="button" type="button" class="clsButton01" onclick="<?=$field_prefix?>editfunc(0)" value="New" />
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
function <?=$field_prefix?>deletefunc(id)
{
 if(confirm('Are you sure you want to delete this entry?')) document.location="index.php?skin=<?=$file_name?>&delete="+id
 else return;
}
function <?=$field_prefix?>editfunc(id)
{
 document.location="index.php?skin=<?=$edit_file_name?>&id="+id
}
</script>
