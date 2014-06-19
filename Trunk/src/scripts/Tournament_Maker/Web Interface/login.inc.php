<?php
function shitCheckerNum($str)
{
  $var = preg_match('/[^a-zA-Z0-9]/', $str);
  return $var;
}

if(isset($login))
{
	if( $selected_tournament == "" )
	Alert("There are no ongoing tournaments atm");

	//Check captcha - may be good against bots
	include("securimage.php");
	$img = new Securimage();
	$valid = $img->check($_POST['code']);

	if($valid == false) { 
	Die("<center>The code you entered was invalid.  <a href=\"javascript:history.go(-1)\">Go back</a> to try again.</center>"); 
	}

	//Remove bullshit characters from user entered data
	$username = mysql_real_escape_string(html_entity_decode(htmlentities($username)));
	$pswd = mysql_real_escape_string(html_entity_decode(htmlentities($pswd)));
	$charname = mysql_real_escape_string(html_entity_decode(htmlentities($charname)));

	//Check for invalid symbols in input
	    if(shitCheckerNum($username) == 1) 
Die("<center>Error: Account contains invalid characters! <a href=\"javascript:history.go(-1)\">Go back</a></center>");
	    if(shitCheckerNum($pswd) == 1) 
Die("<center>Error: Password contains invalid characters! <a href=\"javascript:history.go(-1)\">Go back</a></center>");
	    if(shitCheckerNum($charname) == 1) 
Die("<center>Error: Invalit character! <a href=\"javascript:history.go(-1)\">Go back</a></center>");

	//check if acct is ok
	$result=mysql_query("select acct,gm from ACCOUNTS where login='$username' and password='$pswd'",$dbim) or die("Q 200501180949".mysql_error($dbim));
	if(mysql_num_rows($result))
	{
		list($acct_id,$gm_rights)=mysql_fetch_row($result);
		session_register("s_acct_id");
		$s_acct_id=$acct_id;
		session_register("s_gm_rights");
		$s_gm_rights=$gm_rights;
		//get the guid for the char
		$result=mysql_query("select guid,name,class,level from characters where name='$charname' and acct='$acct_id'",$dbi) or die("Q 200501180949".mysql_error($dbi));
		if(mysql_num_rows($result))
		{
			list($guid,$name,$class,$level )=mysql_fetch_row($result);
			session_register("s_user_guid");		
			$s_user_guid=$guid;
			session_register("s_user_name");		
			$s_user_name=$name;
			session_register("s_user_class");		
			$s_user_class=$class;
			session_register("s_user_level");		
			$s_user_level=$level;
			//check if selected player can join this tournament
			$query="select id,teams_per_account,max_chars_per_team,max_teams from tournament_settings where min_player_level_to_join<='$level' and max_player_level_to_join>='$level' and id='$selected_tournament'";
			$result=mysql_query($query,$dbi) or die("Q	200501180949".mysql_error($dbi).$query);
			if(mysql_num_rows($result))
			{
				list( $id,$teams_per_account,$max_chars_per_team,$max_teams )=mysql_fetch_row($result);
				session_register("s_user_tournament");		
				$s_user_tournament=$id;
				session_register("s_user_max_teams");		
				$s_user_max_teams=$teams_per_account;
				session_register("s_user_max_team_members");		
				$s_user_max_team_members=$max_chars_per_team;
				session_register("s_max_teams");		
				$s_max_teams=$max_teams;
				jumpto("index.php");
			}
			else Alert("Character does not meet the level requirements to join this tournament");
		}
		else Alert("Character does not exist");
	}
	else Alert("Account does not exist");
}
?>
<link rel="stylesheet" type="text/css" media="screen" href="../../screen.css">
<style type="text/css">
<!--
.style1 {
	color: #FFFFFF;
	font-weight: bold;
}
.style5 {color: #FFFFFF}
-->
</style>
</head>



<body Text="#FFFFFF" bgColor="#1A120B">

<div id="container">


<div><TABLE class="maint">
<TR valign=top>
<TD height="161" colspan=3 background="../../images/logo.jpg">
<div>
<div align="right"><a href="../../"><img src="../../images/en.jpg" alt="en" width="24" height="16" border="2" class="style1"></a> <a href="../../ro/"><img src="../../images/ro.jpg" alt="ro" width="24" height="16" border="2" class="style1"></a> <a href="../../de/"><img src="../../images/de.jpg" alt="de" width="24" height="16" border="2" class="style1"></a></div>
</div></TD>
</TR>
<TR valign=top>
<TD rowspan=4 width=172 bgcolor="#1A120B">

<div id="wowservermenu">
<div id="vertmenu"> 
<!---Wow Server Menu-->
<h1>Menu </h1>
<ul>
<li><a href="/" tabindex="1">Home</a></li>
<li><a href="../../uterms.php" tabindex="1">Terms of use</a></li>
<li><a href="../../rules.php" tabindex="1">Servers Rules</a></li>
</ul>
<!---Accounts-->
<h1>Account</h1>
<ul>
<li><a href="../../create.php" tabindex="1">Create Account</a></li>
<li><a href="../../cpass.php" tabindex="1">Change Password</a></li>
<li><a href="../../rpass.php" tabindex="1">Password Retrieval</a></li>
</ul>
<!---Donations-->
<h1>Donations</h1>
<ul>
<li><a href="../../donate.php" tabindex="1">Donate</a></li>
<li><a href="../../dterms.php" tabindex="1">Donation Terms</a></li>
</ul>
<!---Vote Panel-->
<h1>Vote Panel</h1>
<ul>
<li><a href="../../vote/?act=vote" tabindex="1">Vote</a></li>
<li><a href="../../vote/?act=rewards" tabindex="1">Vote Panel</a></li>
</ul>

<!---Player Tools-->
<h1>Players Tools</h1>
<ul>
<li><a href="../../unstuck.php" tabindex="1">Unstuck & Revive</a></li>
</ul>
<!---Forum Support and FAQ-->
<h1>Support</h1>
<ul>
<li><a href="../../contact.php" tabindex="1">Contact Us</a></li>
<li><a href="http://forum.wowbeez.com/" tabindex="1">Forum</a></li>
<li><a href="../../faq.php" tabindex="1">FAQ</a></li>
</ul>
</div></div>
        <tr>
<TD width=813 height=20 background="../../images/nav.gif">
<div align=center><font color="#FFFFFF"><B>TOURNAMENT ORGANIZER</B></font></div></TD>

        </tr>
        <tr>

<TR valign=top>
<TD width=813 height=600 bgcolor="#DBC78A">
<center>
<br /><br />
			 <table>
                          <form name="loginform" action="index.php?login" method="post">
                            <tr>
                              <td width="150" height="30" align="right" class="clsLoginText01">Account:&nbsp;</td>
                              <td width="240"><input type="text" name="username" style="width:180" class="clsTextField01"></td>
                            </tr>
                            <tr>
                              <td height="30" align="right" class="clsLoginText01">Password:&nbsp;</td>
                              <td><input type="password" name="pswd" style="width:180" class="clsTextField01"></td>
                            </tr>
                            <tr>
                              <td height="30" align="right" class="clsLoginText01">Charname:&nbsp;</td>
                              <td><input type="text" name="charname" style="width:180" class="clsTextField01"></td>
                            </tr>
                            <tr>
                              <td height="30" align="right" class="clsLoginText01">Tournament:&nbsp;</td>
                              <td>
								<select class="clsDropdown" name="selected_tournament">
								<?php
									$query = "select id,tournament_name from tournament_settings order by id asc";
									$result=mysql_query($query,$dbi) or die("Q 200702280857_4");
									while(list($id,$tournament_name)=mysql_fetch_row($result))
									{
										?>
										<option value="<?=$id?>" ><?=$tournament_name?></option>
										<?php
									}//end of while
								?>
								</select>
  			     </td>
                            </tr>

			<tr>
				<td height="30" align="right" class="clsLoginText01"><img src="securimage_show.php?sid=<?php echo md5(uniqid(time())); ?>"></td>
				<td><input type="text" name="code" /></td>
			</tr>

                          
</table>
<input type="submit" value="Login" style="width:90px" class="clsButton01">
</center>
</form>

</TD>
</TR>

<TR valign=top bgcolor="#101C23">
<TD colspan=2>
<div align=center>
<div align=center><a href="index.php" title="" class="style2">Home</a><font color="#969696">&nbsp; |&nbsp;&nbsp; </font><a href="rules.php" title="" class="style2">Wow Rules</a><font color="#969696">&nbsp;&nbsp; |&nbsp;&nbsp; </font><a href="dterms.php" title="" class="style2">Donation Terms</a><font color="#969696">&nbsp;&nbsp; |&nbsp;&nbsp; </font><a href="uterms.php" title="" class="style2">Terms of Use</a><font color="#969696">&nbsp;&nbsp; |&nbsp;&nbsp; </font><a href="faq.php" title="" class="style2">FAQ</a><font color="#969696">&nbsp;&nbsp;&nbsp;&nbsp; |&nbsp;&nbsp; </font><a href="contact.php" title="" class="style2">Contact</a></div>
<div align=center><font color="#969696"><BR></font></div>
<div align=center><font color="#969696"><BR></font></div>

<div align=center><font color="#969696"> Copyright © 2008 WoWBeez</font></div>


</TD>
</TR>
</TABLE>
</div>
</div>
