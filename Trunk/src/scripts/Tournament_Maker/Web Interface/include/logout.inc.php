<?php
if (isset($logout))
{
	session_destroy();
	jumpto("index.php");
	die();
}
?>
