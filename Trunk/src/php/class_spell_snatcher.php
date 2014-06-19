<?php
//new Listview({template: 'spell', id: 'spells', visibleCols: ['level', 'school', 'source'], sort: ['level', 'name'], nItemsPerPage: 100, data: [{id:50096,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 1'},{id:61274,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 1'},{id:50108,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 2'},{id:61275,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 2'},{id:50109,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 3'},{id:61276,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 3'},{id:50110,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 4'},{id:61277,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 4'},{id:50111,name:'@Blood Gorged',level:0,school:0,skill:[770],cat:7,chrclass:6,rank:'Rank 5'
	$file=@fopen("http://www.wowhead.com/?spells=7.6.776","r");
	if($file)
	{
		while(!feof($file)) 
		{
			$line = fgets($file);
			if( strpos("#".$line,"new Listview({template: 'spell', id: 'spells', visibleCols:") != 1 )
				continue;
			$parts = explode("data: [{",$line);
			$idmixed=$parts[1];
			$parts = explode("id:",$idmixed);
			$count=count($parts);
			for($i=1;$i<$count;$i+=1)
			{
				$pp = explode( ",",$parts[$i]);
				echo $pp[0].",";
			}
		}
	}
	echo "<br>bla ?";
?>