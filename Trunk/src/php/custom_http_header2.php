<?

$_POST["address"] = "www.bebemagia.ro";
$_POST["port"] = 80;
$_POST["file"] = "concursDoVote.php";
$_POST["protocol"] = "HTTP";
$_POST['version'] = "";

$out = "POST /concursDoVote.php HTTP/1.1
Host: www.bebemagia.ro
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.1.4) Gecko/20091016 Firefox/3.5.4 (.NET CLR 4.0.20506)
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
Keep-Alive: 300
Connection: close
Referer: http://www.bebemagia.ro/concurs-bebelusul-lunii.php?copil=all&codC=1&p=2
Cookie: phpbb2mysql_bebedata=a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bs%3A0%3A%22%22%3Bs%3A6%3A%22userid%22%3Bi%3A-1%3B%7D; phpbb2mysql_bebesid=1793b0036bffe2e549275327ba9e01cf; trafic_h=bd7c6886bcal45dd0b4282becd613747*1257251301*bebemagia.ro*1257251301*1257253268*2; __utma=149043237.1645586005.1257251301.1257251301.1257253268.2; __utmc=149043237; __utmz=149043237.1257251301.1.1.utmccn=(direct)|utmcsr=(direct)|utmcmd=(none); trafic_v=1; __utmb=149043237
Content-Type: application/x-www-form-urlencoded
Content-Length: 25

nota=1&codP=107689&codC=1

";

//daNotaNota=1&daNotaPoza=107689&daNotaConcurs=1&nota=1&codP=107689&codC=1
// initialise some variables
$headersEnded = 0;
$nohtml_data = "";
$process_data = "";

// because some fools cannot follow
// the mind numbingly simple examples i've been kind enough to include.. ;-)
$_POST["address"] = str_replace("http://","",$_POST["address"]);

// HTTP
// works fine
// timeout set at 10 seconds.
$fsock = @fsockopen($_POST["address"], $_POST["port"],$errno, $errstr,10);
if(!$fsock)
{
	echo "<b>ERROR</b><br />Could not open connection, are you sure a service is running on that port on that host? Is the host down?<br /><br >";
		//echo "$errstr ($errno)<br />";
}
else 
{
		// send headers to server
		fputs($fsock, $out);

		// find hostname
		$hostname = @getHostByAddr($_POST["address"]);
		if(!$hostname)
		{
			// we were given the hostname
			$hostname = htmlspecialchars($_POST["address"]);
		}

		// find IP
		$hostip = @getHostByName($_POST["address"]);
		if(!$hostip)
		{
			// we were given the IP
			$hostip = htmlspecialchars($_POST["address"]);
		}

		// print to screen
		echo "<u>Sent headers to ".$hostname." (".$hostip."):</u><br />";
		echo "<pre>";
		echo htmlspecialchars($out)."<hr />";
		echo "</pre>";

		// display output
		echo "<u>Received headers:</u><br />";
		echo "<pre>";
		while(!feof($fsock))
		{
			// grab data
			$returned_data = fgets($fsock, 128);

			// is it the first blank line we've received?
			// (if so, that's the end of the returned headers)
			if($headersEnded == 0 && $returned_data=="\r\n")
			{
				if($_POST['return']=="headers")
				{
					// user only wanted headers, so end here
					break;
				} else {
					// tell user that's the end of headers
					echo "<hr /><u>Received Output:</u><br />";
					// remember that the headers have ended so we don't do this more than once
					$headersEnded = 1;
				}
			} else {
				// display output
				if($_POST['process']!="none")
				{
					$process_data .= $returned_data;
				}

				if($_POST['return']=="nohtml")
				{
					if($headersEnded)
					{
						// store returned data as one string, for later output
						$nohtml_data .= $returned_data;
					} else {
						echo htmlspecialchars($returned_data);
					}
				} else {
					echo htmlspecialchars($returned_data);
				}
			}
		}
		if($_POST['return']=="nohtml")
		{
			echo strip_tags($nohtml_data);
		}
		echo "</pre>";

		// close connection
		fclose($fsock);

		if($_POST['process']!="none")
		{
			echo "<hr />Post-Processing still in development - should be usable, though.<br />";
		}

		// search for form fields
		if($_POST['process']=="form")
		{
			echo "<hr /><b>Lines containing form data:</b><br />";
			echo "<pre>";

			// make array of lines
			$process_data = explode("\r\n",$process_data);

			foreach($process_data as $data)
			{
				$input_pos = strpos($data,"<input");
				$select_pos = strpos($data,"<select");
				$textarea_pos = strpos($data,"<textarea");
				$form_pos = strpos($data,"<form");

				if($input_pos!==false || $select_pos!==false || $textarea_pos!==false || $form_pos!==false)
				{
					echo htmlspecialchars($data)."<br />";
				}
			}
			echo "</pre>";
		}
		// search for linked files
		if($_POST['process']=="file")
		{
			echo "<hr /><b>Lines containing filenames:</b><br />";
			echo "<pre>";

			// make array of lines
			$process_data = explode("\r\n",$process_data);

			foreach($process_data as $data)
			{
				$href_pos = strpos($data," href");
				$src_pos = strpos($data," src");
				$action_pos = strpos($data," action");

				if($href_pos!==false || $src_pos!==false || $action_pos!==false)
				{
					echo htmlspecialchars($data)."<br />";
				}
			}
			echo "</pre>";
		}
	}
?>
