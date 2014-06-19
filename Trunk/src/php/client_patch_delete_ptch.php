<?php
set_time_limit (55*60); //5 minutes timeout ?

//   $srcdir="D:\Munka\Myserver\wowbeez_core\MPQ editors\14334-multi\wow-update-base-14333\Cameras\\";
//   $srcdir="D:\\Munka\\Myserver\\wowbeez_core\\MPQ editors\\14334-multi\\wow-update-base-14333\\Cameras\\";
//   $srcdir="D:\\Munka\\Myserver\\wowbeez_core\\MPQ editors\\14334-multi\\wow-update-base-14333\\Character\\BloodElf\\";
//   $srcdir="D:\\Munka\\Myserver\\wowbeez_core\\MPQ editors\\14334-multi - patched by wow then editor\\";
//   $srcdir="d:\\Mystuff\\myserver\\ncwow\\MPQ editors\\14333 - EnGB folder\\";
//   $srcdir="d:\\Mystuff\\myserver\\ncwow\\MPQ editors\\14333 - World\\";
//   $srcdir="c://MOP 16067 extracted//";
   $srcdir="d:\\Mystuff\\myserver\\ncwow\\MPQ editors\\16650";
   $srcdir="d:\\Mystuff\\myserver\\ncwow\\MPQ editors\\16016 - 17345 - what we want to use";

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
	$f = fopen( $srcfile, "rb" );
	$str = fread( $f, 4 );
	fclose( $f );
	if( $str == "PTCH" )
	{
		echo "found one ! '$str' : ";
		unlink( $srcfile );
	}
//	echo "$srcfile<br>";
}

echo "All Done. If you see no other text then don't panic. It means no files got deleted<br>";
?>