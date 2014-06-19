<?php
$file=@fopen("http://wowbeez.com/index.html","r");
if($file)
{
	while(!feof($file)) 
	{
		$line = fgets($file);

		if( strpos("#".$line,"<title>") == 0 )
			continue; 

		$line= str_replace(array(" ","/t"),"",$line);

		$pieces=explode("<title>",$line);
		$pieces=explode("</title>",$pieces[1]);
		$title=$pieces[0];
		echo "title is '$title' <br>";
		break;
	}
	fclose($file);
}
?>
