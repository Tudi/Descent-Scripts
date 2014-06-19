<?php
include("include/header.inc.php");
?>
<?php
if (!session_is_registered("s_acct_id") || !session_is_registered("s_user_guid") || !session_is_registered("s_user_tournament"))
{
	include("login.inc.php");
	die();
}
?>
<?php
//include("include/site_header.inc.php");
include("include/logout.inc.php");
?>

<link rel="stylesheet" type="text/css" media="screen" href="../../screen.css">
</head>

<body Text="#000000" bgColor="#1A120B">

<div id="container">


<div><TABLE>
<TR valign=top>
<TD height="161" colspan=3 background="../../images/logo.jpg">
<div>
</TD>
</TR>
<TR valign=top>
<TD rowspan=4 width=182 bgcolor="#1A120B">
<div id="wowservermenu">
<div id="vertmenu"> 
<!---Wow Server Menu-->
<h1>Menu</h1>
<ul>
<li><a href=http://www.wowbeez.com/" tabindex="1">Wowbeez Page</a></li>
<li><a href="/" tabindex="1">Home Page</a></li>
</ul>
<!---Accounts-->
<h1>Tournament</h1>
<ul>
<?php
if($skin=="")
	$skin = "info.skin.php";
?>
<li><a href="index.php?skin=new_team.skin.php">Create/Edit Team</a></li>

		  <?php
			$query = "select team_name from tournament_team_settings where team_leader_guid='$s_user_guid' and tournament_id='$s_user_tournament'";
			$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
			list($team_has_name)=mysql_fetch_row($result);

			$query = "select team_id from tournament_players where player_guid='$s_user_guid' and tournament_id='$s_user_tournament' and is_pending_to_join_team=0 and is_pending_to_accept_team_invite=0 ";
			$result=mysql_query($query,$dbi) or die("Q 200501180949".mysql_error($dbi));
			list($part_of_team)=mysql_fetch_row($result);
			if( $team_has_name )
			{
		  ?>

<li><a href="index.php?skin=disband_team.skin.php">Disband Team</a></li>

		  <?php } if( $s_user_max_team_members > 1 && ( $team_has_name || $part_of_team )) { ?>


<li><a href="index.php?skin=my_team.skin.php&team_id=<?=$part_of_team;?>">My current Team</a></li>
		<?php }  if( $s_user_max_team_members > 1 && $team_has_name) { ?>
<li><a href="index.php?skin=accept_team_pend_join.skin.php">Accept Pending Joins</a></li>
		  <?php } if( $s_user_max_team_members > 1 && $team_has_name=="" && $part_of_team=="" ) { ?>

<li><a href="index.php?skin=quick_team_join.skin.php">Quick Join Team</a></li>
<li><a href="index.php?skin=tournament_team_list.skin.php">Join Team</a></li>
		  <? } ?>
<!-- <li><a href="index.php?skin=refresh_strength.skin.php">Refresh Player info</a></li>-->

<li><a href="index.php?skin=match_ladder.skin.php">Match ladder</a></li>
<li><a href="index.php?skin=info.skin.php">Info</a></li>
<?php
if( strpos("#".$s_gm_rights,"a") != 0 )
{
	?>
	<li><a href="index.php?skin=tournament_settings_list.skin.php">Tournament Managment </a></li>
  <?php
}
?>
<li><a href="index.php?logout">Logout</a></li>


</div></div>


<TD width=813 height=20 background="../../images/nav.gif">
<div align=center><font color="#FFFFFF"><B>TOURNAMENT</B></font></div>
</TD>
</TR>

<TR valign=top>
<TD width=813 height=600 bgcolor="#DBC78A">

<?php
if( $skin )
	include_once( $skin );
?>


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
