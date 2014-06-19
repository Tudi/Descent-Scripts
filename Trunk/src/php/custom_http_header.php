<html>
<head>
	<title>PHP HeaderSpoof</title>
	<style type="text/css">
	pre{
		font-family: courier,sans-serif;
		font-size: 12px;
	}
	td{
		vertical-align: top;
		border-bottom: 1px solid gray;
	}
	body,td,b,u{
		font-family: verdana,arial;
		font-size: 12px;
	}
	u{
		font-weight: bold;
		text-decoration: none;
	}
	hr{
		width: 100%;
		height: 1px;
		color: black;
	}
	.text{
		width: 500px;
	}
	select.small{
		width: 120px;
	}
	</style>
</head>
<body>
<form action="<?=$_SERVER['PHP_SELF']?>" method="POST">
<?

// initialise some variables
$headersEnded = 0;
$nohtml_data = "";
$process_data = "";

if(!empty($_POST["address"]) && !empty($_POST["port"]) && !empty($_POST["file"]))
{
	// because some fools cannot follow
	// the mind numbingly simple examples i've been kind enough to include.. ;-)
	$_POST["address"] = str_replace("http://","",$_POST["address"]);

	// open connection
	if($_POST["protocol"]=="HTTPS")
	{
		/*

		$context = stream_context_create();
		$result = stream_context_set_option($context, 'ssl', 'local_cert', '/path/to/keys.pem');
		$result = stream_context_set_option($context, 'ssl', 'passphrase', 'pass_to_access_keys');

		$fsock = fsockopen('ssl://'.$host, 443, $errno, $errstr, 10, $context);

		*/

		// in dev
		echo "<b>Sorry, SSL support still in dev...</b><br />";
		$fsock = @fsockopen("ssl://".$_POST["address"], 443,$errno, $errstr);
	} else {
		// HTTP
		// works fine
		// timeout set at 10 seconds.
		$fsock = @fsockopen($_POST["address"], $_POST["port"],$errno, $errstr,10);
	}
	if(!$fsock)
	{
		echo "<b>ERROR</b><br />Could not open connection, are you sure a service is running on that port on that host? Is the host down?<br /><br />";
		//echo "$errstr ($errno)<br />";
	} else {

		// prepare headers to send

		// replace spaces with url encoded equivalent in filename (otherwise it screws the GET request)
		$out = $_POST['method']." ".stripslashes(str_replace(" ","%20",$_POST["file"]))." HTTP/".$_POST['version']."\r\n";

		// ask server to return data in plain ascii
		if($_POST['version']=="1.0")
		{
			$out .= "Accept-Encoding: \r\n";
		} else {
			$out .= "Accept: */*\r\n";
			$out .= "Accept-Encoding: \r\n";
		}

		if(!empty($_POST["hostname"]))
		{
			$out .= "Host: ".stripslashes($_POST["hostname"])."\r\n";
		}
		if(!empty($_POST["referer"]))
		{
			$out .= "Referer: ".stripslashes($_POST["referer"])."\r\n";
		}
		if(!empty($_POST["cookie"]))
		{
			$out .= "Cookie: ".stripslashes($_POST['cookie'])."\r\n";
		}
		if(!empty($_POST["user_agent"]))
		{
			$out .= "User-Agent: ".stripslashes($_POST['user_agent'])."\r\n";
		}
		if(!empty($_POST["auth_string"]))
		{
			$out .= "Authorization: Basic ".base64_encode($_POST['auth_string'])."\r\n";
		}
		if(!empty($_POST["range"]))
		{
			$out .= "Range: bytes=".stripslashes($_POST['range'])."\r\n";
		}
		if(!empty($_POST["other_headers"]))
		{
			// anything I forgot?
			$out .= stripslashes($_POST['other_headers'])."\r\n";
		}

		$out .= "Connection: close\r\n";

		if($_POST['method']=="POST" && !empty($_POST["post_data"]))
		{
			if(!empty($_POST['content_type']))
			{
				// add content type header
				$out .= "Content-Type: ".stripslashes($_POST['content_type'])."\r\n";
			} else {
				// the moronic user doesn't understand that you need a content-type.
				$out .= "Content-Type: application/x-www-form-urlencoded\r\n";
			}
			// add length header
			$out .= "Content-Length: ".strlen(stripslashes(str_replace("\r\n","",$_POST['post_data'])))."\r\n\r\n";
			// add post data
			$out .= stripslashes($_POST['post_data'])."\r\n";
		}

		// terminate headers
		$out .= "\r\n";

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
}
?>
<hr />
<br /><br />
<table cellpadding="2" cellspacing="0">
<tr>
	<td colspan="2">
		<center><b>HeaderSpoof, by User24.</b></center><br /><br />
		Required fields are address, file and port.
	</td>
</tr>
<tr>
	<td><b>Address:</b><br /><i>eg; www.google.com or 216.239.39.99</i></td>
	<td><input class="text" type="text" name="address" value="<?=htmlspecialchars($_POST['address'])?>"></td>
</tr>
<tr>
	<td><b>Protocol:</b></td>
	<td>
		<select name="protocol" class="small">
		<option value="HTTP">HTTP</option>
		<option value="HTTPS" <?=($_POST['protocol']=='HTTPS') ? ' selected' : ''; ?>>HTTPS (beta)</option>
		</select>
	</td>
</tr>
<tr>
	<td><b>Version:</b></td>
	<td>
		<select name="version" class="small">
		<option value="1.0">1.0</option>
		<option value="1.1" <?=($_POST['version']=='1.1') ? ' selected' : ''; ?>>1.1</option>
		</select>
	</td>
</tr>
<tr>
	<td><b>Return:</b></td>
	<td>
		<select name="return" class="small">
		<option value="headers">only headers</option>
		<option value="all" <?=($_POST['return']=='all') ? ' selected' : ''; ?>>everything</option>
		<option value="nohtml" <?=($_POST['return']=='nohtml') ? ' selected' : ''; ?>>no HTML</option>
		</select>
	</td>
</tr>
<tr>
	<td><b>Method:</b></td>
	<td>
		<select name="method" class="small">
			<option value="GET">GET</option>
			<option value="POST" <?=($_POST['method']=='POST') ? ' selected' : ''; ?>>POST</option>
		</select>
	</td>
</tr>
<tr>
	<td><b>Hostname:</b><br /><i>eg; www.google.com</i></td>
	<td><input class="text" type="text" name="hostname" value="<?=htmlspecialchars($_POST['hostname'])?>"></td>
</tr>
<tr>
	<td><b>Port #:</b><br /><i>eg; 80</i></td>
	<td><input class="text" type="text" name="port" value="<?=htmlspecialchars($_POST['port'])?>"></td>
</tr>
<tr>
	<td><b>File to request:</b><br /><i>eg; /index.html</i></td>
	<td><input class="text" type="text" name="file" value="<?=htmlspecialchars($_POST['file'])?>"></td>
</tr>
<tr>
	<td><b>Referer:</b><br /><i>eg; http://www.google.com/</i></td>
	<td><input class="text" type="text" name="referer" value="<?=htmlspecialchars($_POST['referer'])?>"></td>
</tr>
<tr>
	<td><b>Cookie(s):</b><br /><i>eg; name=value; name2=val2</i></td>
	<td><input class="text" type="text" name="cookie" value="<?=htmlspecialchars($_POST['cookie'])?>"></td>
</tr>
<tr>
	<td><b>User Agent:</b><br /><i>eg; Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)</i></td>
	<td><input class="text" type="text" name="user_agent" value="<?=htmlspecialchars($_POST['user_agent'])?>"></td>
</tr>
<tr>
	<td><b>Auth-String (basic):</b><br /><i>eg; username:password</i> - base64 encoded automatically</td>
	<td><input class="text" type="text" name="auth_string" value="<?=htmlspecialchars($_POST['auth_string'])?>"></td>
</tr>
<tr>
	<td><b>Byte Range:</b><br /><i>eg; 1-200</i></td>
	<td><input class="text" type="text" name="range" value="<?=htmlspecialchars($_POST['range'])?>"></td>
</tr>
<tr>
	<td><b>Additional Headers:</b></td>
	<td><textarea name="other_headers" class="text" rows="5">
<?=htmlspecialchars($_POST['other_headers'])?></textarea></td>
</tr>
<tr>
	<td><b>Content-Type:</b><br /><i>eg; application/x-www-form-urlencoded</i><br />Required only when POST data below is not empty</td>
	<td><input class="text" type="text" name="content_type" value="<?=htmlspecialchars($_POST['content_type'])?>"></td>
</tr>
<tr>
	<td><b>POST Data:</b><br /><i>eg; field1=val1&field2=val2</i><br />Content Length automatically calculated.<br />Only sent if Request Method is POST</td>
	<td><textarea name="post_data" class="text" rows="5">
<?=htmlspecialchars($_POST['post_data'])?></textarea></td>
</tr>
<tr>
	<td><b>Post-Processing?</td>
	<td>
		<select name="process" class="small">
			<option value="none">none</option>
			<option value="form" <?=($_POST['process']=='form') ? ' selected' : ''; ?>>form data</option>
			<option value="file" <?=($_POST['process']=='file') ? ' selected' : ''; ?>>filenames</option>
		</select>
	</td>
</tr>
<tr>
	<td colspan="2" align="center"><input type="submit" value="Send Request"></td>
</tr>
<tr>
	<td colspan="2">
		This script allows easy manipulation of header information.
		<br /><br />
		If any non-required fields are left empty, those headers will not be sent.
		<br /><br />
		See <a href="http://www.w3.org/Protocols/HTTP/HTRQ_Headers.html" target="_blank">http://www.w3.org/Protocols/HTTP/HTRQ_Headers.html</a> for more info regarding HTTP headers.
	</td>
</tr>
</table>
</form>
</body>
</html>
<?php
?>
POST /concursDoVote.php HTTP/1.1
Host: www.bebemagia.ro
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.1.4) Gecko/20091016 Firefox/3.5.4 (.NET CLR 4.0.20506)
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
Keep-Alive: 300
Connection: keep-alive
Referer: http://www.bebemagia.ro/concurs-bebelusul-lunii.php?copil=all&codC=1&p=2
Cookie: phpbb2mysql_bebedata=a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bs%3A0%3A%22%22%3Bs%3A6%3A%22userid%22%3Bi%3A-1%3B%7D; phpbb2mysql_bebesid=1793b0036bffe2e549275327ba9e01cf; trafic_h=bd7c6886bcal45dd0b4282becd613747*1257251301*bebemagia.ro*1257251301*1257253268*2; __utma=149043237.1645586005.1257251301.1257251301.1257253268.2; __utmc=149043237; __utmz=149043237.1257251301.1.1.utmccn=(direct)|utmcsr=(direct)|utmcmd=(none); trafic_v=1; __utmb=149043237
Content-Type: application/x-www-form-urlencoded
Content-Length: 25

$_SERVER['HTTP_HOST'] == '10.1.1.6';
$_SERVER['HTTP_X_FORWARDED_HOST'] == 'nick.com';
$_SERVER['SERVER_NAME'] == '10.1.1.6';
HTTP_CLIENT_IP
HTTP_X_FORWARDED_FOR
REMOTE_ADDR