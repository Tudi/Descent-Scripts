<?php
set_time_limit (55*60); //5 minutes timeout ?

   $srcdir="d://Mystuff//myserver//ncwow//MPQ editors//16160 - with editor//world//Expansion04//Doodads//";

search_dir( $srcdir );

function search_dir( $srcdir )
{
   if($curdir = opendir($srcdir))
   {
      while($file = readdir($curdir))
	   {
         if($file != '.' && $file != '..')
         {
            $srcfile = $srcdir . '\\' . $file;
            if(is_file($srcfile) )
				handle_new_file( $srcfile );
			else
				search_dir( $srcfile );
         }
	   }
   }
   closedir($curdir);
}

function handle_new_file( $srcfile )
{
	//extension of the file
	$ext = substr ( $srcfile, -4 );
	$f = fopen( $srcfile, "rb" );
	$str = fread( $f, 4 );
	fclose( $f );
	if( $ext == "M2" || $ext == "m2" )
	{
		if( $str != "MD20" )
			echo "found one ! '$str' : ";
	}
	else if( $ext == "BLP" || $ext == "blp" )
	{
		if( $str != "BLP2" )
			echo "found one ! '$str' : ";
	}
	else if( $ext == "skin" || $ext == "SKIN" )
	{
		if( $str != "SKIN" )
			echo "found one ! '$str' : ";
	}
//die( "$ext - $srcfile" );
//	echo "$srcfile<br>";
}

echo "All Done. If you see no other text then don't panic. It means no files got deleted<br>";
?>