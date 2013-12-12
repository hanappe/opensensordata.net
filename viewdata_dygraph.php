<?php
/*  
    OpenSensorData.net 

    OpenSensorData.net is a free (as in freedom) online database and
    API to store sensor data.

    Copyright (C) 2013  Sony Computer Science Laboratory Paris
    Author: Peter Hanappe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
require_once "error.inc.php";
require_once "db.inc.php";
require_once "input.inc.php";
require_once "datastream.inc.php";

db_connect();

$path = $_GET['path'];
$selector = new Selector();
if (!$selector->parse($path))
        badRequest($selector->err);

$datastream = new Datastream();
if (!$datastream->load($selector->id)) 
        badRequest("Invalid datastream ID: " . $sel['datastream']);

db_close();

?><html>
  <head>
    <script type="text/javascript" src="<?php echo $base_url ?>/dygraph-combined.js"></script>
  </head>
  <body>
    <div id="graphdiv"></div>
    <script type="text/javascript">
      g = new Dygraph(
          document.getElementById("graphdiv"),
          "<?php echo $base_url ?>/datastreams/<?php echo $path ?>",
          { labels: [ "Date", "<?php echo $datastream->name ?>" ] }
          
      );
    </script>
  </body>
</html>
