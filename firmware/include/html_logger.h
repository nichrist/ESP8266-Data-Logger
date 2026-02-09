const char* page_logger PROGMEM = R"==( 
<!DOCTYPE html>
<html>
<head>
  <title>Data Logger</title>
<head>
<body>
  <h1 style="text-align:center; color:red;">EPAL ARGYROUPOLIS IoT Projects</h1>
  <h3 style="text-align:center;">ESP8266 Temperature Data Logger</h3>
  <style>
  canvas{
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
  }
  /* Data Table Styling*/ 
  #dataTable {
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    border-collapse: collapse;
    width: 100%;
    text-align: center;
  }
  #dataTable td, #dataTable th {
    border: 1px solid #ddd;
    padding: 8px;
  }
  #dataTable tr:nth-child(even){background-color: #f2f2f2;}
  #dataTable tr:hover {background-color: #ddd;}
  #dataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: center;
    background-color: #050505;
    color: white;
  }
  </style>
</head>
<body>
<div>
  <table id="dataTable">
    <tr><th>Time</th><th>Temperature (&degC)</th><th>Humidity (%)</th><th>Pressure (mBar)</th></tr>
  </table>
</div>
<br>
<br>  
<script>
//  Store some values for future use
var Tvalues = [];
var Hvalues = [];
var Bvalues = [];
var timeStamp = [];

setInterval(function() {
  // Call a function repetatively with 2.5 Second interval
  getData();
}, 2500); //2500mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
  var time = new Date().toLocaleTimeString();
  var txt = this.responseText;
  var obj = JSON.parse(txt);

  // Store 50 values for future use
  const MAX_POINTS = 100;
  function pushLimited(arr, value)
  {
  arr.push(value);
  if (arr.length > MAX_POINTS) arr.shift();
  }
  pushLimited(Tvalues, obj.Temperature);
  pushLimited(Hvalues, obj.Humidity);
  pushLimited(Bvalues, obj.Pressure);
  pushLimited(timeStamp, time);

  //Update Data Table
    var table = document.getElementById("dataTable");
    var row = table.insertRow(1); //Add after headings
    if (table.rows.length > 50) 
    {
      table.deleteRow(table.rows.length - 1);
    }
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);
    var cell4 = row.insertCell(3);
    cell1.innerHTML = time;
    cell2.innerHTML = obj.Temperature;
    cell3.innerHTML = obj.Humidity;
    cell4.innerHTML = obj.Pressure;
    }
  };
  // END of function getData()

  xhttp.open("GET", "readData", true); //Handle readData server on ESP8266
  xhttp.send();
}
</script>
</body>
</html>
)==";
