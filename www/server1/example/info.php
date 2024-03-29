<!DOCTYPE html>
<html>
<head>
    <title>Query String Test</title>
</head>
<body>
    <h2>Query String Test</h2>

    <?php
    // Check if query string parameter exists
    if(isset($_GET['name'])) {
        // Retrieve and sanitize the value
        $name = htmlspecialchars($_GET['name']);
        // Display the value
        echo "<p>Hello, $name!</p>";
    } else {
        echo "<p>No name parameter found in the URL.</p>";
    }
    ?>

</body>
</html>
