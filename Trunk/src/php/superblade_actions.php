<?php
include("include/header.inc.php");
include("include/functions.php");
set_time_limit (3000);

$inp_h = @fopen("gmcommand.log", "r");
while($inp_h && !feof($inp_h)) 
{
	$line=fgets($inp_h);
	if(strpos("#".$line,"SUPERBLADE123")!=0 
		and strpos("#".$line,"used kill command on")==0 
		and strpos("#".$line,"Kicked player")==0 
		and strpos("#".$line,"rooted player")==0 
		and strpos("#".$line,":: summoned")==0 
		and strpos("#".$line,":: revived")==0 
		and strpos("#".$line,"remove item id")==0 
		and strpos("#".$line,"used modify")==0 
		and strpos("#".$line,"godmode ")==0 
		and strpos("#".$line,"modified speed")==0 
		and strpos("#".$line,":: banned ")==0 
		and strpos("#".$line,":: removed spell")==0 
		and strpos("#".$line,"used add item")==0 
		and strpos("#".$line,": taught")==0 
		)
	{
		echo $line."<br>";
		if( strpos("#".$line,"used add item")!=0  )
		{
			$pieces = explode("item id ",$line);
			$pieces2 = explode(" ",$pieces[1]);
			$itemid = $pieces2[0];
			if( $itemid )
			{
				$pieces = explode(" to ",$line);
				$name = $pieces[1];
				$name = str_replace(" ","",$name);
				$name = str_replace("\n","",$name);
				$name = str_replace("\r","",$name);
//				echo "name = $name and id = $itemid <br>";
				$query="select guid,online from characters where name like '$name'";
//echo $query."<br>";
				$res=mysql_query($query,$dbi) or die(" Q200611231503 ".mysql_error($dbi).$query);
				list($guid,$online)=mysql_fetch_row($res);
//				if($online == 0 )
				{
//					$query="select count(*) from playeritems where ownerguid='$guid' and entry='$itemid'";
//					$res=mysql_query($query,$dbi) or die(" Q200611231503 ".mysql_error($dbi).$query);
					echo "delete from playeritems where ownerguid='$guid' and entry='$itemid'; <br>";
				}
			}
		}
		if( strpos("#".$line,":: taught")!=0  )
		{
			$pieces = explode(":: taught ",$line);
			$pieces2 = explode(" ",$pieces[1]);
			$name = $pieces2[0];
			$pieces = explode("spell ",$line);
			$spellid = $pieces[1];
			$spellid = str_replace(" ","",$spellid);
			$spellid = str_replace("\n","",$spellid);
			$spellid = str_replace("\r","",$spellid);
			if( $spellid )
			{
				$query="select name from dbc_spell where id='$spellid'";
				$res=mysql_query($query,$dbi) or die(" Q200611231503 ".mysql_error($dbi).$query);
				list($spellname)=mysql_fetch_row($res);
				echo "name = $name and id = $spellid and spellname=$spellname<br>";
				$query="select guid,online from characters where name like '$name'";
//echo $query."<br>";
				$res=mysql_query($query,$dbi) or die(" Q200611231503 ".mysql_error($dbi).$query);
				list($guid,$online)=mysql_fetch_row($res);
//				if($online == 0 )
				{
					echo "update characters set spells=replace(spells,'$spellid,','') where guid='$guid'; <br>";
				}
			}
		}
	}/**/
}
fclose($inp_h);
?>