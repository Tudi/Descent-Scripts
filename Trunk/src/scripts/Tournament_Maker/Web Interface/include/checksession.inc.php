<?php
if (!session_is_registered('s_user'))
	{
	?>
	<script>
	alert("Session expired!\n\nPease connect again.")
	document.location="index.php"
	</script>
	<?
	exit;
	}
?>
