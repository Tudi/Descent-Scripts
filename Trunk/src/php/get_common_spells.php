<?php
$unique_spells="";
for($i=0;$i<65535;$i++)
	$unique_spells[$i] = 0;


   $srcdir="./spell_lists";
   if($curdir = opendir($srcdir))
      while($file = readdir($curdir))
         if($file != '.' && $file != '..')
         {
            $srcfile = $srcdir . '\\' . $file;
            if(is_file($srcfile))
				process_file($srcfile);
         }
   closedir($curdir);


//now get max values
$max=0;
for($i=0;$i<65535;$i++)
	if( $unique_spells[$i] > $max )
		$max = $unique_spells[$i];


//print out common spells
if( $max > 0 )
	for($i=0;$i<65535;$i++)
		if( $unique_spells[$i] == $max )
			echo "$i<br>";

 function process_file($filename)
 {
	 global $unique_spells;
	 $inf = fopen($filename,"r");
	while($inf && !feof($inf)) 
	{
   		$line = fgets($inf);
//echo $line."<br>";
		$line = (int) $line;
//echo $line."<br>";
		$unique_spells[$line]++;
	}
	 fclose($inf);
 }
?>