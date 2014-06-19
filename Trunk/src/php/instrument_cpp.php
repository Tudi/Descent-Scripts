<?php
global 	$unique_trace_id;
global	$just_remove_instrumentation;

$unique_trace_id = 0;
$just_remove_instrumentation = 0;

$srcdir="D:/MyStuff/myserver/ncwow/wotlk/1173_trunk/src/arcemu-world";
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*   
   if($curdir = opendir($srcdir))
      while($file = readdir($curdir))
         if($file != '.' && $file != '..')
         {
            $srcfile = $srcdir . '/' . $file;
            if(is_file($srcfile) && ( 
//				strpos("#".$file,".h")!=0 || 
				strpos("#".$file,".cpp")!=0) )
			 {
				//instrument file
				$new_file_content = instrument_file($srcfile);
				//now overwrite old file
				$outf=fopen($srcfile,"w");
//echo $new_file_content;
//				$outf=fopen("t.cpp","w");
				fputs($outf,$new_file_content);
				fclose($outf);
//die("for testing we will use only 1 file !");
			 }
         }
   closedir($curdir);/**/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

$file_list=array("AIInterface.cpp","ArenaTeam.cpp","AuctionHouse.cpp","AuctionMgr.cpp","BattlegroundHandler.cpp","BattlegroundMgr.cpp","ChannelHandler.cpp","CharacterHandler.cpp","ChatHandler.cpp","CombatHandler.cpp","Container.cpp","Corpse.cpp","Creature.cpp","DuelHandler.cpp","DynamicObject.cpp","EventableObject.cpp","GameObject.cpp","Group.cpp","GroupHandler.cpp","Guild.cpp","GuildHandler.cpp","HonorHandler.cpp","Item.cpp","ItemHandler.cpp","ItemInterface.cpp","LfgHandler.cpp","LfgMgr.cpp","LootMgr.cpp","MailSystem.cpp","Map.cpp","MapCell.cpp","MapMgr.cpp","MiscHandler.cpp","MovementHandler.cpp","NPCHandler.cpp","Object.cpp","ObjectMgr.cpp","ObjectStorage.cpp","Pet.cpp","PetHandler.cpp","Player.cpp","Quest.cpp","QuestMgr.cpp","RaidHandler.cpp","ReputationHandler.cpp","ScriptMgr.cpp","SkillHandler.cpp","Spell.cpp","SpellAuras.cpp","SpellEffects.cpp","SpellHandler.cpp","TaxiHandler.cpp","TaxiMgr.cpp","TradeHandler.cpp","Unit.cpp","WarsongGulch.cpp");
foreach($file_list as $key => $value )
{
	$srcfile = $srcdir . '/' . $value;
	if(is_file($srcfile) )
	 {
		//instrument file
		$new_file_content = instrument_file($srcfile);
		//now overwrite old file
		$outf=fopen($srcfile,"w");
//echo $new_file_content;
//$outf=fopen("t.cpp","w");
		fputs($outf,$new_file_content);
		fclose($outf);
	//die("for testing we will use only 1 file !");
	 }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
echo "Maximum nbumber of Unique IDs used : $unique_trace_id <br>";

//search in the file for function starts and try to instrument them
//we define function start as a level 0 "{" braket
global $html_output,$newline,$braket_level,$file_trace_id,$function_trace_id,$file_thread_ID,$skip_instrument,$big_comment_detected;
function instrument_file($filename)
{
//echo "Instrumenting file : $filename";
	global 	$unique_trace_id,$just_remove_instrumentation;
	global	$html_output,$newline,$braket_level,$file_trace_id,$function_trace_id,$file_thread_ID,$skip_instrument;
	$infile = fopen($filename,"r");
	//no way
	if( !$infile )
		return;

//	$html_output="<br>";
	$html_output="";
	$newline="\r\n";

	$result_file="";
	$braket_level=0;
	$file_trace_id=0;						//some way to subidentify this trace call
	$function_trace_id=0;					//some way to subidentify this trace call
	$file_thread_ID="GetCurrentThreadId()";	//some way to identify in which thread this callstack is
	$skip_instrument=0;						//for enum,struct,class
	$big_comment_detected=0;				// if we are inside /**/ then we do not process data
	$prev_line="";
	while(!feof($infile)) 
	{
		$line = fgets($infile);

		//skip already inserted instrumentation 
		if(strpos("#".$line,"INSTRUMENTATION_CALLSTACK_MONITOR")!=0)
			continue;

		if( $just_remove_instrumentation != 1 )
		{
/*			//in case line containes more then 1 { we will reformat it into multiple lines each containing only 1
			$line2=str_replace(array("\n","\r"),"",$line);
			$line2=str_replace("{","NO_FRIKKIN_WAY_TO_HAVETHIS_TWICE{NO_FRIKKIN_WAY_TO_HAVETHIS_TWICE",$line2);
			$line2=str_replace("}","NO_FRIKKIN_WAY_TO_HAVETHIS_TWICE}NO_FRIKKIN_WAY_TO_HAVETHIS_TWICE",$line2);
			$sublines = explode("NO_FRIKKIN_WAY_TO_HAVETHIS_TWICE",$line2);
//echo $line2."<br>";
			if( count($sublines ) > 3 ) //1 braket = 3 parts
			{
				$new_line="";
				foreach($sublines as $key => $line3 )
					if( strlen(trim($line)) > 0 )
					{
						$t = instrument_1_line($line3);
						if(strpos("#".$t,"\n")!=0)
							$new_line.=$t;
						else
							$new_line.=$t."{$newline}".$html_output;
					}
				$line = $new_line;
			}
			else if( count($sublines ) > 1 )
				$line = instrument_1_line($line);
			else
				$line = $line;*/

			$line = instrument_1_line($line,$prev_line);
		}
		$prev_line=$line;
		$result_file.=$line."$html_output";
	}
	$file_trace_id++;
	fclose($infile);
	return $result_file;
}

function count_opem_brakets($line)
{
	$ret=0;
	$len=strlen($line);
	for($i=0;$i<$len;$i++)
		if($line[$i]=='{')
			$ret++;
	return $ret;
}

function count_close_brakets($line)
{
	$ret=0;
	$len=strlen($line);
	for($i=0;$i<$len;$i++)
		if($line[$i]=='}')
			$ret++;
	return $ret;
}

function instrument_1_line($line,$prev_line)
{
	global $html_output,$newline,$braket_level,$file_trace_id,$function_trace_id,$file_thread_ID,$skip_instrument;
	global $unique_trace_id,$just_remove_instrumentation,$big_comment_detected;

	$openbrakets = count_opem_brakets($line);
	$closebrakets = count_close_brakets($line);
	if( $braket_level==0 && (strpos("#".$line,"class")!=0 || strpos("#".$line,"enum")!=0 || strpos("#".$line,"struct")!=0 || ( strpos("#".$line,"]")!=0 && strpos("#".$line,"=")!=0)) || $openbrakets>1 || ($openbrakets!=0 && $closebrakets!=0))
		$skip_instrument=1;
	if( strpos("#".$line,"*/")!=0 )
	{
		$big_comment_detected = 0;
		return $line;
	}
	if( $big_comment_detected == 1 || strpos("#".$line,"/*")!=0 )
	{
		$big_comment_detected = 1;
		return $line;
	}
	//probably the start of a new function
	if(strpos("#".$line,"{")!=0)
	{
		if($braket_level==0 && $skip_instrument==0)
		{
			//insert instrumentation here if there is none yet
			$in_function_trace_id=0;
//			$line=str_replace("\r","",$line);
			//$line=str_replace("\n","{$newline}INSTRUMENTATION_CALLSTACK_MONITOR_ENTER(__FILE__,__FUNCTION__,__LINE__,$unique_trace_id,$file_trace_id,$function_trace_id,$in_function_trace_id,$file_thread_ID){$newline}".$html_output,$line);
			$unique_trace_id++;
		}
		$braket_level+=$openbrakets;
	}
	//probably the end of a functions
	if(strpos("#".$line,"}")!=0 || strpos("#".$line,"return ")!=0 || strpos("#".$line,"return;")!=0)
	{
		if( strpos("#".$line,"}")!=0 )
		{
			if($braket_level==1 && $skip_instrument==0 )
			{
				//insert instrumentation here if there is none yet
				$in_function_trace_id++;
				$line=str_replace(array("\n","\r"),"",$line);
				//$n_line="INSTRUMENTATION_CALLSTACK_MONITOR_EXIT(__FILE__,__FUNCTION__,__LINE__,$unique_trace_id,$file_trace_id,$function_trace_id,$in_function_trace_id,$file_thread_ID){$newline}".$html_output;
				$line=$n_line.$line."{$newline}".$html_output;
			}
			if($braket_level>=$closebrakets)
				$braket_level-=$closebrakets;
		}
		else if( (strpos("#".$line,"return ")!=0 || strpos("#".$line,"return;")!=0)&& $skip_instrument==0 )
		{
			//check if line is commented out or has an "if" before it
			$stripped_line=str_replace(array("\n","\r"," ","\t"),"",$line);
			$stripped_line2=substr($stripped_line,0,strpos($stripped_line,"return"));
			//do not format commented out lines
			if( strpos("#".$stripped_line,"//") == 0 )
			{
				$in_function_trace_id++;
				$line=str_replace(array("\n","\r"),"",$line);

				if( strpos($stripped_line,"return") != 0 )
				{
					$tabbing = substr($line,0,strpos($line,"if"));
					$line=str_replace("return",$newline.$tabbing."\t"."return",$line);
				}

				//very common problem when "return" is not enclosed in {}
				$need_brakets = 0;
				$tabbing="";
				if( (strpos("#".$prev_line,"if(") != 0 || strpos("#".$prev_line,"if (") != 0 )&& strpos("#".$prev_line,"(") != 0 )
				{
					$need_brakets = 1;
					$tabbing = substr($prev_line,0,strpos($prev_line,"if"));
				}

				if( $need_brakets != 0 )
					$n_line ="$tabbing{ {$newline}";
				else
					$n_line="";

				//$n_line.="INSTRUMENTATION_CALLSTACK_MONITOR_EXIT(__FILE__,__FUNCTION__,__LINE__,$unique_trace_id,$file_trace_id,$function_trace_id,$in_function_trace_id,$file_thread_ID){$newline}".$html_output;
				$line=$n_line.$line."{$newline}".$html_output;
				if( $need_brakets == 1 )
					$line = $line.$tabbing."}{$newline}";
			}
		}
		if($braket_level==0)
		{
			$function_trace_id++;
			$skip_instrument=0;
		}
		$unique_trace_id++;
	}
	return $line;
}
?>