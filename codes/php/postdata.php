<?php

//Creates new record as per request
//Connect to database
$servername = "localhost";
$username = "calparlamp";
$password = "Calparlamp31*";
$dbname = "calparlamp";

// Create connection
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT id, btn_state, led_state, rgb_val, led_brt timestamp FROM jus ORDER BY ID DESC LIMIT 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
  // output data of each row
  while ($row = $result->fetch_assoc()) {
    echo
      // $row["id"]. "///"
      $row["btn_state"] . "/"
      . $row["led_state"] . "/"
      . $row["rgb_val"] . "/"
      . $row["led_brt"];
    //.$row["timestamp"];
  }
} else {
  echo "0 results";
}
$conn->close();
?>