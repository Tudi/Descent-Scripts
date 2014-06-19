<?php
	//direct acces of a skin is not allowed
	if( !$dbi )
		die("Can't acces page directly");
$file_name="tournament_settings.skin.php";
$list_name_text="tournament_settings";
$new_entry_text=" New tournament_settings";
$save_jumpto="index.php?skin=tournament_settings_list.skin.php";
$field_prefix="tournament_settings_";
$table_name="tournament_settings";

if(!isset($id)) $id=0;
if(isset($delete))
{
	if($id!=0)
	{
	$delete_entry_query="delete from tournament_settings where id=$delete";
	$mysql_delete_error_code="Q 200902030504_0";

		$result=mysql_query($delete_entry_query,$dbi) or die($mysql_delete_error_code);
	jumpto($save_jumpto);
	die();
	}
}//end of delete
if((isset($save)) && ($save==1))
{
$save_insert_query="insert into tournament_settings (tournament_name,min_teams_to_start,max_chars_per_team,min_player_level_to_join,max_player_level_to_join,inscription_starts,inscription_ends,tournament_starts,teams_per_account,cur_match_round,max_teams) values ('$tournament_settings_tournament_name','$tournament_settings_min_teams_to_start','$tournament_settings_max_chars_per_team','$tournament_settings_min_player_level_to_join','$tournament_settings_max_player_level_to_join','$tournament_settings_inscription_starts','$tournament_settings_inscription_ends','$tournament_settings_tournament_starts','$tournament_settings_teams_per_account','$tournament_settings_cur_match_round','$tournament_settings_max_teams')";
$save_update_query="update tournament_settings set tournament_name='$tournament_settings_tournament_name',min_teams_to_start='$tournament_settings_min_teams_to_start',max_chars_per_team='$tournament_settings_max_chars_per_team',min_player_level_to_join='$tournament_settings_min_player_level_to_join',max_player_level_to_join='$tournament_settings_max_player_level_to_join',inscription_starts='$tournament_settings_inscription_starts',inscription_ends='$tournament_settings_inscription_ends',tournament_starts='$tournament_settings_tournament_starts',teams_per_account='$tournament_settings_teams_per_account',cur_match_round='$tournament_settings_cur_match_round',max_teams='$tournament_settings_max_teams' where id=$id";

$mysql_save_insert_error_code="Q 200902030504_1";
$mysql_save_update_error_code="Q 200902030504_2";

	if($id==0)
		{
			$result=mysql_query($save_insert_query,$dbi) or die($mysql_save_insert_error_code);
		}//end of save
	else
		{
			$result=mysql_query($save_update_query,$dbi) or die($mysql_save_update_error_code);
		}//end of update
		jumpto($save_jumpto);
		die();
}//end of save/update

if (!isset($save))
{
$tournament_settings_tournament_name="1 vs 1";
$tournament_settings_min_teams_to_start="2";
$tournament_settings_max_chars_per_team="1";
$tournament_settings_min_player_level_to_join="70";
$tournament_settings_max_player_level_to_join="80";
$tournament_settings_inscription_starts=time();
$tournament_settings_inscription_ends=time()+60*60*24;
$tournament_settings_tournament_starts=time()+60*60*24;
$tournament_settings_teams_per_account="1";
$tournament_settings_cur_match_round="0";
$tournament_settings_max_teams="1000";
$load_values_query="select tournament_name,min_teams_to_start,max_chars_per_team,min_player_level_to_join,max_player_level_to_join,inscription_starts,inscription_ends,tournament_starts,teams_per_account,cur_match_round,max_teams from tournament_settings where id=$id";
$mysql_load_values_error_code="Q 200902030504_3";

if($id!=0)
{
	$result=mysql_query($load_values_query,$dbi) or die($mysql_load_values_error_code);
	$list_row=mysql_fetch_array($result, MYSQL_ASSOC);
	foreach($list_row as $list_row_name=>$list_row_value)
	{
		$temp=$field_prefix.$list_row_name;
		$$temp=$list_row_value;
	}//end of foreach
}

} //save not set
?>

<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <form name="<?=$table_name?>_form" action="index.php?skin=<?=$file_name?>&id=<?=$id?>" method="post">
    <input type="hidden" name="save" value="0">
	<tr bgcolor="#66990" class="clsListheaderText01">  
      <td height="20">Column</td>
      <td >Value</td>
    </tr>
	<tr> 
      <td height="10" colspan="4"><img src="../../images/blank.gif" width="1" height="1"></td>
    </tr>
    
    <tr>
      <td class="clsListText01">Tournament name
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_tournament_name" value="<?=$tournament_settings_tournament_name?>">

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Min teams to start
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_min_teams_to_start" value="<?=$tournament_settings_min_teams_to_start?>">

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Max chars per team
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_max_chars_per_team" value="<?=$tournament_settings_max_chars_per_team?>">

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Min player level to join
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_min_player_level_to_join" value="<?=$tournament_settings_min_player_level_to_join?>">

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Max player level to join
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_max_player_level_to_join" value="<?=$tournament_settings_max_player_level_to_join?>">

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Inscription starts
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_inscription_starts" value="<?=$tournament_settings_inscription_starts?>"><?php echo date("Y-M-d h:i",$tournament_settings_inscription_starts); ?>
	
      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Inscription ends
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_inscription_ends" value="<?=$tournament_settings_inscription_ends?>"><?php echo date("Y-M-d h:i",$tournament_settings_inscription_ends); ?>

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Tournament starts
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_tournament_starts" value="<?=$tournament_settings_tournament_starts?>"><?php echo date("Y-M-d h:i",$tournament_settings_tournament_starts); ?>

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Teams per account
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_teams_per_account" value="<?=$tournament_settings_teams_per_account?>">

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Cur match round
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_cur_match_round" value="<?=$tournament_settings_cur_match_round?>">

      </td>
    </tr>
    
    <tr>
      <td class="clsListText01">Max teams
      </td>
      <td class="clsFormText01"><input class="clsTextField01" type="text" name="tournament_settings_max_teams" value="<?=$tournament_settings_max_teams?>">

      </td>
    </tr>
    
    <tr>
      <td>&nbsp;</td>
    </tr>
    <tr>
      <td><?php
if($id==0)
	{
		$save_button_text="Add";
	}
else
	{
		$save_button_text="Update";
	}	
?>
        <input type="button" class="clsButton01" value="<?=$save_button_text?>" onClick="save_form()">
	  <?php if($id>0) { ?>
        <input type="button" class="clsButton02" value="Delete" onClick="delete_entry()" <?php if($id==0) echo "disabled";?>></td>
	  <?php } ?>
	</tr>
  </form>
<form name="h2u">
<table border="1" align="center"><tr><td>
<table border="0">
  <tr>
    <th>Mon:</th>
    <th>&nbsp;</th>
    <th>Day:</th>
    <th>&nbsp;</th>
    <th>Year:</th>

    <th>&nbsp;</th>
    <th>Hr:</th>
    <th>&nbsp;</th>
    <th>Min:</th>
    <th>&nbsp;</th>
    <th>Sec:</th>
  </tr>

  <tr>
    <td><input type="text" size="4" maxlength="2" name="inMon" value="01"></td>
    <th>/</th>
    <td><input type="text" size="4" maxlength="2" name="inDay" value="02"></td>
    <th>/</th>
    <td><input type="text" size="4" maxlength="4" name="inYear" value="2009"></td>
    <th>at</th>

    <td><input type="text" size="4" maxlength="2" name="inHr" value="0"></td>
    <th>:</th>
    <td><input type="text" size="4" maxlength="2" name="inMin" value="0"></td>
    <th>:</th>
    <td><input type="text" size="4" maxlength="2" name="inSec" value="0"></td>
  </tr>
  <tr>
    <td colspan="11" align="center"><input type="button" value="Submit" onClick="humanToTime();">&nbsp;<input type="reset" value="Reset"></td>

  </tr>
  <tr>
    <td colspan="11" align="center"><input name="result" type="text" value="" size="30" READONLY></td>
  </tr>
</table>
</td></tr></table>
</form>
</table>

<script language="JavaScript">
function reload_page()
{
<?=$table_name?>_form.submit();
}//end of function
function save_form()
{
 

<?=$table_name?>_form.save.value="1";
<?=$table_name?>_form.submit()
}//end of saveform
function delete_entry()
{
	if(! confirm("Are you sure you want to delete this?"))
		return;
	else
		document.location="index.php?skin=<?=$file_name?>&id=<?=$id?>&delete=<?=$id?>"
}
  function humanToTime()
  {
    var humDate = new Date(Date.UTC(document.h2u.inYear.value,
          (stripLeadingZeroes(document.h2u.inMon.value)-1),
          stripLeadingZeroes(document.h2u.inDay.value),
          stripLeadingZeroes(document.h2u.inHr.value),
          stripLeadingZeroes(document.h2u.inMin.value),
          stripLeadingZeroes(document.h2u.inSec.value)));
    document.h2u.result.value = (humDate.getTime()/1000.0);
  }
</script>
