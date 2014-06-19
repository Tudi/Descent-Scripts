<?php

	set_time_limit (5*60); //5 minutes timeout ?

	$srcdir="pics";
	if($curdir = opendir($srcdir))
		while($file = readdir($curdir))
			if($file != '.' && $file != '..')
			{
				$srcfile = $srcdir . '\\' . $file;
				if(is_file($srcfile))
				{
					$cmd = "BLPConverter.exe $srcfile";
					$ret = exec($cmd);
				}
			}
	closedir($curdir);
?>