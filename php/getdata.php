<?php

//Creates new record as per request
    //Connect to database
    $servername = "localhost";
    $username = "USER";
    $password = "DBPWD";
    $dbname = "DBNAME";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Database Connection failed: " . $conn->connect_error);
    }

    //Get current date and time
    date_default_timezone_set('Europe/Istanbul');
    $d = date("Y-m-d");
    //echo " Date:".$d."<BR>";
    $t = date("H:i:s");

    	$btn_state = $_GET['btn_state'];
    	$led_state = $_GET['led_state'];
        $rgb_val = $_GET['rgb_val'];
        $led_brt = $_GET['led_brt'];
        $pattern = $_GET['pattern'];

	    $sql = "INSERT INTO jus (btn_state, led_state, rgb_val, led_brt, pattern, date, time)
		
		VALUES ('$btn_state', '$led_state', '$rgb_val', '".$d."', '".$t."')";

		if ($conn->query($sql) === TRUE) {
		    console.log("OK");
		} else {
		    echo "Error: " . $sql . "<br>" . $conn->error;
		}


	$conn->close();
?>