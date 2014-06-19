<?php
$table="";
//open mobinfo file 
$inp_h = @fopen("stats.txt", "r");
$line=0;
$qp=0;
$frames_found=0;
while($inp_h && !feof($inp_h)) 
{
	$line=fgets($inp_h);
	if(strpos($line,"a new frame"))
	{
		$parts = explode(" = ",$line);
//		echo $parts[2]."and".((int)($parts[2]))."<br>";
		//if it is a new qp then we can generate table for previous one
		if($qp<(int)($parts[2]) && $qp!=0)
		{
			echo "{ // size for qp $qp <br>"; //a new matrix of values
			for($i=0;$i<9;$i++)
			{
				if($size_table_values[$qp][$i])
					$size_table[$qp][$i] = $size_table[$qp][$i] / $size_table_values[$qp][$i];
				if($cost_table_values[$qp][$i])
					$cost_table[$qp][$i] = $cost_table[$qp][$i] / $cost_table_values[$qp][$i];
			}
			for($i=0;$i<9;$i++)
			{
				echo "&nbsp&nbsp&nbsp&nbsp{"; //a new row of values
				for($j=0;$j<9;$j++)
				{
					if($size_table[$qp][$j]!=0)
						$trans_mat[$qp][$i][$j] = $size_table[$qp][$i] / $size_table[$qp][$j];
					else $trans_mat[$qp][$i][$j] = 0;
					if($cost_table[$qp][$j]!=0)
						$trans_mat2[$qp][$i][$j] = $cost_table[$qp][$i] / $cost_table[$qp][$j];
					else $trans_mat2[$qp][$i][$j] = 0;
//					$floatinvalue=$trans_mat[$qp][$i][$j];
					$floatinvalue=$trans_mat2[$qp][$i][$j];
					$endvalue=substr($floatinvalue,0,7);
					if($endvalue==(int)$endvalue)
						$endvalue=$endvalue.".";
					for($len=strlen($endvalue);$len<7;$len++)
						$endvalue.=0;
					if($j<8)
						echo ($endvalue).", ";
					else echo ($endvalue)." ";
				}
				echo "},<br>"; //end row
			}
			echo "},<br>"; //end matrix
		}
		$qp=(int)($parts[2]);
		$frames_found++;
		$size_table[$qp] = "";
		$cost_table[$qp] = "";
	}
	else
	{
		$parts = explode("\t",$line);
		foreach($parts as $key => $value)
		{
			if($key % 3 == 0)
				$size = (int)$value;
			else if($key % 3 == 1)
				$cost = (int)$value;
			else if($key % 3 == 2)
			{
				$typ = (int)$value;
				$size_table[$qp][$typ] += $size;
				$size_table_values[$qp][$typ]++;
				$cost_table[$qp][$typ] += $cost;
				$cost_table_values[$qp][$typ]++;
			}
		}
	}
}
/*
echo "{ // for min qp<br>"; //a new matrix of values
for($i=0;$i<9;$i++)
{
	echo "&nbsp&nbsp&nbsp&nbsp{"; //a new row of values
	for($j=0;$j<9;$j++)
	{
		$size_table_min[$i][$j] = 99999;
		for($k=0;$k<51;$k++)
			if($trans_mat[$k][$i][$j]<$size_table_min[$i][$j] && $trans_mat[$k][$i][$j]!=0)
				$size_table_min[$i][$j] = $trans_mat[$k][$i][$j];
		if($size_table_min[$i][$j]==99999)
			$size_table_min[$i][$j]=0;
		$floatinvalue = $size_table_min[$i][$j];
		$endvalue=substr($floatinvalue,0,7);
		if($endvalue==(int)$endvalue)
			$endvalue=$endvalue.".";
		for($len=strlen($endvalue);$len<7;$len++)
			$endvalue.=0;
		if($j<8)
			echo ($endvalue).", ";
		else echo ($endvalue)." ";
	}
	echo "},<br>"; //end row
}
echo "},<br>"; //end matrix

echo "{ // for max qp<br>"; //a new matrix of values
for($i=0;$i<9;$i++)
{
	echo "&nbsp&nbsp&nbsp&nbsp{"; //a new row of values
	for($j=0;$j<9;$j++)
	{
		$size_table_max[$i][$j] = -1;
		for($k=0;$k<51;$k++)
			if($trans_mat[$k][$i][$j]>$size_table_max[$i][$j])
				$size_table_max[$i][$j] = $trans_mat[$k][$i][$j];
		$floatinvalue = $size_table_max[$i][$j];
		$endvalue=substr($floatinvalue,0,7);
		if($endvalue==(int)$endvalue)
			$endvalue=$endvalue.".";
		for($len=strlen($endvalue);$len<7;$len++)
			$endvalue.=0;
		if($j<8)
			echo ($endvalue).", ";
		else echo ($endvalue)." ";
	}
	echo "},<br>"; //end row
}
echo "},<br>"; //end matrix

echo "{ // for avg qp<br>"; //a new matrix of values
for($i=0;$i<9;$i++)
{
	echo "&nbsp&nbsp&nbsp&nbsp{"; //a new row of values
	for($j=0;$j<9;$j++)
	{
		$total=0;
		$values=0;
		for($k=0;$k<51;$k++)
			if($trans_mat[$k][$i][$j])
			{
				$total += $trans_mat[$k][$i][$j];
				$values++;
			}
		if($values!=0)
			$floatinvalue = $total / $values;
		else $floatinvalue = 0;
		$endvalue=substr($floatinvalue,0,7);
		if($endvalue==(int)$endvalue)
			$endvalue=$endvalue.".";
		for($len=strlen($endvalue);$len<7;$len++)
			$endvalue.=0;
		if($j<8)
			echo ($endvalue).", ";
		else echo ($endvalue)." ";
	}
	echo "},<br>"; //end row
}
echo "},<br>"; //end matrix


echo "{ // for avg cost<br>"; //a new matrix of values
for($i=0;$i<9;$i++)
{
	echo "&nbsp&nbsp&nbsp&nbsp{"; //a new row of values
	for($j=0;$j<9;$j++)
	{
		$total=0;
		$values=0;
		for($k=0;$k<51;$k++)
			if($trans_mat2[$k][$i][$j])
			{
				$total += $trans_mat2[$k][$i][$j];
				$values++;
			}
		if($values!=0)
			$floatinvalue = $total / $values;
		else $floatinvalue = 0;
		$endvalue=substr($floatinvalue,0,7);
		if($endvalue==(int)$endvalue)
			$endvalue=$endvalue.".";
		for($len=strlen($endvalue);$len<7;$len++)
			$endvalue.=0;
		if($j<8)
			echo ($endvalue).", ";
		else echo ($endvalue)." ";
	}
	echo "},<br>"; //end row
}
echo "},<br>"; //end matrix

echo "{ // for min cost<br>"; //a new matrix of values
for($i=0;$i<9;$i++)
{
	echo "&nbsp&nbsp&nbsp&nbsp{"; //a new row of values
	for($j=0;$j<9;$j++)
	{
		$cost_table_min[$i][$j] = 99999;
		for($k=0;$k<51;$k++)
			if($trans_mat2[$k][$i][$j]<$cost_table_min[$i][$j] && $trans_mat2[$k][$i][$j]!=0)
				$cost_table_min[$i][$j] = $trans_mat2[$k][$i][$j];
		if($cost_table_min[$i][$j]==99999)
			$cost_table_min[$i][$j]=0;
		$floatinvalue = $cost_table_min[$i][$j];
		$endvalue=substr($floatinvalue,0,7);
		if($endvalue==(int)$endvalue)
			$endvalue=$endvalue.".";
		for($len=strlen($endvalue);$len<7;$len++)
			$endvalue.=0;
		if($j<8)
			echo ($endvalue).", ";
		else echo ($endvalue)." ";
	}
	echo "},<br>"; //end row
}
echo "},<br>"; //end matrix

echo "{ // for max cost<br>"; //a new matrix of values
for($i=0;$i<9;$i++)
{
	echo "&nbsp&nbsp&nbsp&nbsp{"; //a new row of values
	for($j=0;$j<9;$j++)
	{
		$cost_table_max[$i][$j] = -1;
		for($k=0;$k<51;$k++)
			if($trans_mat2[$k][$i][$j]>$cost_table_max[$i][$j])
				$cost_table_max[$i][$j] = $trans_mat2[$k][$i][$j];
		$floatinvalue = $cost_table_max[$i][$j];
		$endvalue=substr($floatinvalue,0,7);
		if($endvalue==(int)$endvalue)
			$endvalue=$endvalue.".";
		for($len=strlen($endvalue);$len<7;$len++)
			$endvalue.=0;
		if($j<8)
			echo ($endvalue).", ";
		else echo ($endvalue)." ";
	}
	echo "},<br>"; //end row
}
echo "},<br>"; //end matrix*/
?>
