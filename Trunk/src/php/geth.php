<?php
for($i=1;$i<52;$i++)
{
	$inf=@fopen("temp.sql","r");

	if(!$inf)
		continue;

	while(!feof($inf)) 
	{
		//take 1 line
   		$line = fgets($inf);
		if(strpos($line,"6948"))
		{
			$parts = explode("(\"",$line);
			$parts2 = explode("\"",$parts[1]);
			echo "INSERT INTO `playercreateinfo_items` (`indexid`, `protoid`, `slotid`, `amount`) VALUES('".$parts2[0]."','1111','28','1');<br>";
			echo "INSERT INTO `playercreateinfo_items` (`indexid`, `protoid`, `slotid`, `amount`) VALUES('".$parts2[0]."','2222','29','1');<br>";
			echo "INSERT INTO `playercreateinfo_items` (`indexid`, `protoid`, `slotid`, `amount`) VALUES('".$parts2[0]."','3333','30','1');<br>";
		}
	}
	fclose($inf);
}

?>