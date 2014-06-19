<?
$main_dir ="D:/temp/M4A-trunk/";
$main_dir ="c:/Perforce/branches/Durian/CMSEngine";
//$main_dir ="D:/temp/M4A-trunk/m4/";
process_dir($main_dir);

function process_dir($dir)
{
   $srcdir=$dir;
   echo "processing dir : $srcdir <br>";
   if($curdir = opendir($srcdir))
   {
      while($file = readdir($curdir))
         if($file != '.' && $file != '..')
         {
            $srcfile = $srcdir . '\\' . $file;
            if(is_file($srcfile))
				convert_file( $srcfile );
			else if( is_dir($srcfile) )
				process_dir( $srcfile );
         }
   }
   closedir($curdir);
}

function convert_file($full_path)
{
	//extension of the file
/*	$pos = strpos( $full_path, '.', strlen( $full_path ) - 4 );
	$ext = substr ( $full_path, $pos + 1 );
//echo "extension is $ext <br>";
	if( $ext != "sh" && $ext != "SH" && $ext != "bat" && $ext != "BAT" && $ext != "txt" )
	{
		echo "Skip processing file : $full_path <br>";
		return;
	}
	else */
		echo "processing file : $full_path <br>";

	$lines = "";
	$linec=0;
	$file = fopen($full_path,"r");
	if($file)
	{
		while(!feof($file)) 
		{
			$lines[$linec++] = fgets($file);
		}
		fclose($file);
	}
	$file = fopen($full_path,"w");
	if($file)
	{
		for($i=0;$i<$linec;$i++)
		{
			$content = str_replace("\r\n","\n",$lines[$i]);
//			$content = str_replace("\n","\r\n",$lines[$i]);
			fputs($file,$content);
		}
		fclose($file);
	}/**/
}
?>