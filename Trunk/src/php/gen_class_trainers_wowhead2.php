<?php
set_time_limit( 15*60 );

$UNIT_NPC_FLAG_GOSSIP = 1;
$UNIT_NPC_FLAG_TRAINER = 16;
$UNIT_NPC_FLAG_TRAINER_CLASS = 32;
$UNIT_NPC_FLAG_TRAINER_PROF = 64;
	
$dbhost="localhost";
$dbuname="root";
$dbupass="";
$dbname="temp";
$dbname_world="wowbeez_db";

//database connection 
 $dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
 mysql_select_db($dbname, $dbi) or die("Q 200603201239".mysql_error($dbi));

//fetch all class spells that are "trainer" source
$class_spells=array("","","","","","","","","","");
$class_spells_counter=array("","","","","","","","","","");
$spell_trainers=array("","","","","","","","","","");
$spell_trainers_counter=array("","","","","","","","","","");
for($class=1;$class<=11;$class++)
{
	//ask for each trainer spell the list of trainers
	$class_spells_counter[$class] = 0;
	$file=@fopen("http://www.wowhead.com/spells=7.$class","r");
//	echo "Getting class $class spells<br> ";
	if($file)
	{
		$spells="";
		while(!feof($file)) 
		{
			$line = fgets($file);
			if( strpos("#".$line,"new Listview({template: 'spell', id: 'spells'") == 0 
				&& strpos("#".$line,"new Listview({template: 'spell', id: 'teaches-ability'") == 0 
				)
				continue;
//new Listview({template: 'spell', id: 'spells', visibleCols: ['level', 'schools', 'source', 'classes'], sort: ['level', 'name'], nItemsPerPage: 100, data: [{"cat":7,"chrclass":1,"id":76268,"level":0,"name":"@Armor Skills","nskillup":0,"rank":"Passive","reqclass":1,"schools":1,"skill":[803]},{"cat":7,"chrclass":1,"id":76290,"level":0,"name":"@Weapon Skills","nskillup":0,"rank":"Passive","reqclass":1,"schools":1,"skill":[803]},{"cat":7,"chrclass":1,"id":88163,"level":1,"name":"@Attack","nskillup":0,"reqclass":1,"schools":1,"skill":[803]},{"cat":7,"chrclass":1,"id":2457,"level":1,"name":"@Battle Stance","nskillup":1,"reqclass":1,"schools":1,"skill":[26],"source":[10]},{"cat":7,"chrclass":1,"id":88161,"level":1,"name":"@Strike","nskillup":0,"reqclass":1,"schools":1,"skill":[26]},{"cat":7,"chrclass":1,"id":100,"level":3,"name":"@Charge","nskillup":1,"rank":"Battle Stance","reqclass":1,"schools":1,"skill":[26],"source":[6],"trainingcost":53},{"cat":7,"chrclass":1,"id":34428,"level":5,"name":"@Victory Rush","nskillup":1,"reqclass":1,"schools":1,"skill":[256],"source":[6],"trainingcost":126},{"cat":7,"chrclass":1,"id":772,"level":7,"name":"@Rend","nskillup":1,"rank":"Battle, Defensive 
//{"cat":7,"chrclass":1,"id":100,"level":3,"name":"@Charge","nskillup":1,"rank":"Battle Stance","reqclass":1,"schools":1,"skill":[26],"source":[6],"trainingcost":53}
			//check char class - really hope 1 trainer teaches only 1 class
			$line_stripped = str_replace( "\"","", $line );
//echo $line_stripped;		
			$pieces=explode("id:",$line_stripped);
			foreach($pieces as $index => $val)
//				if( $index>0 )
				{
					//get train cost
					$pieces2=explode("trainingcost:",$val);
					$pieces3=explode("}",$pieces2[1]);
					$train_cost=$pieces3[0];
					if( $train_cost <= 0 )
						continue;
					//get source. We only care about trainer spells and quest spells
					if( strpos( $val, "source:[6" ) == 0 && strpos( $val, "source:[4" ) == 0 && strpos( $val, "source:[10" ) == 0)
						continue;
					//get required level
					$pieces2=explode("level:",$val);
					$pieces3=explode(",",$pieces2[1]);
					$level=$pieces3[0];
					//get spell id
					$pieces2=explode(",",$val);
					$spell=$pieces2[0];
					$spell=(int)$spell;
					if( $spell <= 0 )
						continue;

					//find the cast spell if possible for this spell
					$get_query2 = "SELECT count(*) FROM temp.SpellEffectdbccsv WHERE col_1=36 and col_24=$spell"; 
					$get_res2 = mysql_query($get_query2 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query2 " );
					list($is_teaching_spell)=mysql_fetch_row($get_res2); //if spell does not have teach effect then we just add it to player spellbook

					$class_spells[$class][$class_spells_counter[$class]][ "id" ] = $spell;
					$class_spells[$class][$class_spells_counter[$class]][ "level" ] = $level;
					$class_spells[$class][$class_spells_counter[$class]][ "train_cost" ] = $train_cost;
					if( $is_teaching_spell > 0 )
					{
						$class_spells[$class][$class_spells_counter[$class]][ "learn_spell" ] = 0;
						$class_spells[$class][$class_spells_counter[$class]][ "cast_spell" ] = $spell;
					}
					else
					{
						$class_spells[$class][$class_spells_counter[$class]][ "learn_spell" ] = $spell;
						$class_spells[$class][$class_spells_counter[$class]][ "cast_spell" ] = 0;
					}
					$class_spells_counter[$class]++;
				}
		}
		fclose( $file );
	}
}
for($class=1;$class<=11;$class++)
	//generate trainer spells 
	if( count( $class_spells[$class] ) )
		foreach( $class_spells[$class] as $key => $class_spell_struct )
		{
			$spell_id = $class_spell_struct[ "id" ];
			$level = $class_spell_struct[ "level" ];
			$train_cost = $class_spell_struct[ "train_cost" ];
			$learn_spell = $class_spell_struct[ "learn_spell" ];
			$cast_spell = $class_spell_struct[ "cast_spell" ];

			//these are the "general trainers
			$sql="INSERT IGNORE into trainer_spells (entry,cast_spell,learn_spell,reqlevel,spellcost) values ";
			$sql.="(".(10000+$class).",$cast_spell,$learn_spell,'$level',$train_cost);";
			echo "$sql<br>";
			
		}
die();	
/**/	
for($class=1;$class<=11;$class++)
{
	//generate trainer defs if it is missing
	$string_npc="";
	foreach( $class_spells[$class] as $key => $class_spell_struct )
	{
//		print_r($val);
		$spell_id = $class_spell_struct[ "id" ];
		$file=@fopen("http://www.wowhead.com/spell=$spell_id","r");
		if($file)
		{
			while(!feof($file)) 
			{
				$line = fgets($file);
				if( strpos("#".$line,"new Listview({template: 'npc', id: 'taught-by-npc'") == 0 )
					continue;
//new Listview({template: 'npc', id: 'taught-by-npc', name: LANG.tab_taughtby, tabs: tabsRelated, parent: 'lkljbjkb574', data: [{"classification":0,"id":43009,"location":[3487],"maxlevel":60,"minlevel":60,"name":"Alsudar the Bastion","react":[undefined,1],"tag":"Warrior Trainer","type":7},{"classification":0,"id":914,"location":[1519],"maxlevel":60,"minlevel":60,"name":"Ander Germaine","react":[1,undefined],"tag":"Warrior Trainer","type":7},{"classification":0,"id":4087,"location":[1657],"maxlevel":60,"minlevel":60,"name":"Arias'ta 
//{"classification":0,"id":43009,"location":[3487],"maxlevel":60,"minlevel":60,"name":"Alsudar the Bastion"
				//check char class - really hope 1 trainer teaches only 1 class
				$line_stripped = str_replace( "\"","", $line );
//echo $line_stripped;		
				$pieces=explode("id:",$line_stripped);
				foreach($pieces as $index => $val)
//					if( $index>0 )
					{
						//get npc id
						$pieces2=explode(",",$val);
						$npc=$pieces2[0];
						$npc=(int)$npc;
						if( $npc <= 0 )
							continue;

						$spell_trainers[$spell_id][$spell_trainers_counter[$spell_id]] = $npc;
						$spell_trainers_counter[$spell_id]++;
						
						if( strpos( "#".$string_npc, "$npc" ) == 0 )
							$string_npc .="$npc,";
					}
			}
			fclose( $file );
		}
	}
	//delete old trainer spells
	echo "delete from trainer_spells where entry in ($string_npc 0);<br>";
	//create new defs if required
	$pieces = explode( ",",$string_npc );
	foreach( $pieces as $key => $npc )
		if( $npc > 0 )
		{
			//check if we have a trainer def for this trainer
			$get_query2 = "SELECT count(*) FROM $dbname_world.trainer_defs WHERE entry=$npc"; 
			$get_res2 = mysql_query($get_query2 ,$dbi) or die(" Q200806091705 ".mysql_error($dbi)." $get_query2 " );
			list($is_defined)=mysql_fetch_row($get_res2); //if spell does not have teach effect then we just add it to player spellbook
			if( $is_defined == 0 )
				echo "INSERT IGNORE INTO trainer_defs ( entry, req_class ) values ( '$npc', '$class' );<br>";
		}
	//update creature_proto flags
	echo "Update creature_proto set NpcFlags= NpcFlags | $UNIT_NPC_FLAG_TRAINER | $UNIT_NPC_FLAG_GOSSIP | $UNIT_NPC_FLAG_TRAINER_CLASS where entry in ($string_npc 0);<br>";
	//generate trainer spells 
	foreach( $class_spells[$class] as $key => $class_spell_struct )
	{
		$spell_id = $class_spell_struct[ "id" ];
		$level = $class_spell_struct[ "level" ];
		$train_cost = $class_spell_struct[ "train_cost" ];
		$learn_spell = $class_spell_struct[ "learn_spell" ];
		$cast_spell = $class_spell_struct[ "cast_spell" ];

		foreach($spell_trainers[$spell_id] as $key => $npc_entry )
		{
			$sql="INSERT IGNORE into trainer_spells (entry,cast_spell,learn_spell,reqlevel,spellcost) values ";
			$sql.="($npc_entry,$cast_spell,$learn_spell,'$level',$train_cost);";
			echo "$sql<br>";
		}
	}
//	die();
}
?>