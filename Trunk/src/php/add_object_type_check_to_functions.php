<?php
global 	$unique_trace_id;
global	$just_remove_instrumentation;

$unique_trace_id = 0;
$just_remove_instrumentation = 0;

$srcdir="D:/MyStuff/myserver/ncwow/wotlk/1173_trunk/src/arcemu-world/";
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

$iter=0;
$file_list[$iter] = "AIInterface.cpp";		$objident[$iter]="AIInterface";		$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_AI_INTERFACE_OBJECT_TYPE";		$iter++;
$file_list[$iter] = "Container.cpp";		$objident[$iter]="Container";		$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_CONTAINER_OBJECT_TYPE";			$iter++;
$file_list[$iter] = "Creature.cpp";			$objident[$iter]="Creature";		$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_CREATURE_OBJECT_TYPE";			$iter++;
$file_list[$iter] = "DynamicObject.cpp";	$objident[$iter]="DynamicObject";	$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_DYNO_OBJECT_TYPE";				$iter++;
$file_list[$iter] = "GameObject.cpp";		$objident[$iter]="GameObject";		$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_GO_OBJECT_TYPE";					$iter++;
$file_list[$iter] = "Item.cpp";				$objident[$iter]="Item";			$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_ITEM_OBJECT_TYPE";				$iter++;
$file_list[$iter] = "Pet.cpp";				$objident[$iter]="Pet";				$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_PET_OBJECT_TYPE";				$iter++;
$file_list[$iter] = "Player.cpp";			$objident[$iter]="Player";			$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_PLAYER_OBJECT_TYPE";				$iter++;
$file_list[$iter] = "Unit.cpp";				$objident[$iter]="Unit";			$macro[$iter] = "INSTRUMENT_TYPECAST_CHECK_UNIT_OBJECT_TYPE";				$iter++;
for($i=0;$i<$iter;$i++)
{
	$srcfile = $srcdir.$file_list[$i];
	$new_file_content = instrument_file($srcfile,$objident[$i],$macro[$i]);
	//now overwrite old file
	$outf=fopen($srcfile,"w");
	fputs($outf,$new_file_content);
	fclose($outf);
//echo $new_file_content;
//$outf=fopen("t.cpp","w");
//	die("for testing we will use only 1 file !");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//search in the file for function starts and try to instrument them
//we define function start as a level 0 "{" braket
global $html_output,$newline,$braket_level,$skip_instrument,$big_comment_detected;
global $found_object_function;
function instrument_file($filename,$ident,$macro)
{
//echo "Instrumenting file : $filename";
	global 	$just_remove_instrumentation;
	global	$html_output,$newline,$braket_level,$file_trace_id,$function_trace_id,$file_thread_ID,$skip_instrument;
	global $found_object_function;
	$infile = fopen($filename,"r");
	//no way
	if( !$infile )
		return;

//	$html_output="<br>";
	$html_output="";
	$newline="\r\n";

	$result_file = "";
	$braket_level = 0;
	$skip_instrument = 0;						//for enum,struct,class
	$big_comment_detected = 0;				// if we are inside /**/ then we do not process data
	$found_object_function = 0;
	$prev_line="";
	while(!feof($infile)) 
	{
		$line = fgets($infile);

		//skip already inserted instrumentation 
		if(strpos("#".$line,$macro)!=0)
			continue;

		if( $just_remove_instrumentation != 1 )
			$line = instrument_1_line($line,$prev_line,$ident,$macro);
		$prev_line=$line;
		$result_file.=$line."$html_output";
	}
	$file_trace_id++;
	fclose($infile);
	return $result_file;
}

function instrument_1_line($line,$prev_line,$ident,$macro)
{
	global $html_output,$newline,$skip_instrument;
	global $just_remove_instrumentation,$big_comment_detected;
	global $found_object_function;
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
	//check if this is an object function we are looking for but skip constructor
	if(strpos("#".$line,$ident."::")!=0 && strpos("#".$line,"::".$ident) == 0 )
	{
		$found_object_function = 1;
	}
	$n_line = $line;
	//probably the start of a new function
	if(strpos("#".$line,"{")!=0 && $found_object_function == 1 )
	{
		$found_object_function = 0;
		$line=str_replace(array("\n","\r","{"),"",$line);
		$tabbing = substr($line,0,strpos($line,"if"));
		$n_line="{\n";
		$n_line.="\t$macro\n";
		if( $line!="" )
			$n_line.="$line\n";
	}
	return $n_line;
}
?>