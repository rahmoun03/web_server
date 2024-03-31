<?php
// Access cookies sent by the client
$name = $_COOKIE['name']; // Retrieve the value of the 'username' cookie
 // Retrieve the value of the 'language' cookie

// Print HTML content
echo "<html>";
echo "<head><title>Received Cookies</title></head>";
echo "<body>";
echo "<h1>Received Cookies</h1>";
echo "<p>Username: $name</p>";
echo "</body>";
echo "</html>";
?>
