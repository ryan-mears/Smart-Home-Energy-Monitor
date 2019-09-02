<?php
$con = mysqli_connect("54.164.68.96","CSUN","smarthome","smarthome");

// Check connection
if (mysqli_connect_errno())
  {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
  }

mysqli_query($con,"SELECT * FROM devicedata ");
mysqli_query($con,"DELETE FROM devicedata WHERE device = '".$_GET["device"]."'");
mysqli_query($con,"INSERT INTO devicedata (device,power,powerfactor) VALUES ('".$_GET["device"]."','".$_GET["power"]."','".$_GET["powerfactor"]."')"); 
mysqli_query($con,"UPDATE devicedata SET ID = 1 WHERE ID > 2");
?>