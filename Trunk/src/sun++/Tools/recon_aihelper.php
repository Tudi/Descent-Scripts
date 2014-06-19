<?php

$kapcsolat = @ mysql_connect("localhost","root","");
$kivalaszt = @ mysql_select_db("aiagent");


?>
<html>
<head>
<script src="http://www.wowhead.com/widgets/power.js"></script>
</head>
<title>HDB ai helper, UPDATER by RECON</title>



<?php
for ($i=1;$i<=10;$i++) {
mysql_query("UPDATE `ai_agents` SET `event`='".$_POST["Event".$i]."', `chance`='".$_POST["chance".$i]."', `maxcount`='".$_POST["szor".$i]."', `spelltype`='".$_POST["sT".$i]."', `targettype_overwrite`='".$_POST["to".$i]."' WHERE `spell`='".$_POST["idje".$i]."'");
}

print("<br /><br />

<form action=\"aihelper.php\" method=\"post\">");
$szam = 1;
$sql = mysql_query("SELECT DISTINCT spell FROM ai_agents WHERE spelltype='0' LIMIT 0,10");
while ($row = @ mysql_fetch_array($sql)) {
$random=rand(10,100);
$randomszor=rand(0,10);
print("
<A HREF=\"http://www.wowhead.com/?spell=".$row["spell"]."\">spell</a>
<b>EVENT</b>
<input type=\"hidden\" name=\"idje".$szam."\" value=\"".$row["spell"]."\" />
<select size=\"1\" name=\"Event".$szam."\">
<option value=\"0\">ENTERCOMBAT</option>
<option value=\"7\">TARGETCRITHIT</option>
<option value=\"2\">DAMAGETAKEN</option>
<option value=\"3\">TARGETCASTSPELL</option>
<option value=\"5\">TARGETDODGED</option>
</select>

<b>Chance:</b>
<input type=\"text\" value=\"".$random."\" name=\"chance".$szam."\">
<b>MaxCount:</b>
<input type=\"text\" value=\"".$randomszor."\" name=\"szor".$szam."\"></select>
<b>Type</b>
<select size=\"1\" name=\"sT".$szam."\">
	<option value=\"1\">ROOT</option>
	<option value=\"2\">HEAL</option>
	<option value=\"3\">STUN</option>
	<option value=\"4\">FEAR</option>
	<option value=\"6\">CURSE</option>
	<option value=\"5\">SILENCE</option>
	<option value=\"8\">DAMAGE</option>
	<option value=\"9\">SUMMON</option>
	<option value=\"7\">AOEDAMAGE</option>
	<option value=\"10\">BUFF</option>
	<option value=\"11\">DEBUFF</option>
</select>
<b>TargetType:</b>
<select size=\"1\" name=\"to".$szam."\">
	<option value=\"1\">Véletlen játékos</option>
	<option value=\"2\">Aki üt</option>
	<option value=\"4\">Magára</option>
</select><br /><br />");
$szam++;
}

print("
<input type=\"submit\">
</form>");
?>
</html>