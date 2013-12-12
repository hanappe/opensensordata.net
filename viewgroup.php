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
require_once "group.inc.php";
require_once "selector.model.php";

db_connect();

$path = $_GET['path'];

$selector = new Selector();
if (!$selector->parse($path))
        badRequest($selector->err);

$group = new Group();
if (!$group->load($selector->id, true)) 
        badRequest("Invalid group ID: " . $selector->id);

?><html>
  <head>
    <!-- meta http-equiv="refresh" content="60" -->
    <script type="text/javascript" src="<?php echo $base_url ?>/dygraph-combined.js"></script>
    <script type="text/javascript" src="<?php echo $base_url ?>/json2.js"></script>
    <script type="text/javascript" src="<?php echo $base_url ?>/opensensordata.js"></script>
    <script type="text/javascript" src="<?php echo $base_url ?>/PhotostreamViewer.js"></script>
    <style>

div.PhotostreamViewer {
  width: 240px;
  border: solid 1px #a0a0a0;
  margin: 10px;
}

div.PhotostreamViewerImageDiv {}
img.PhotostreamViewerImage {}
div.PhotostreamViewerCaption {
  font-size: 8px;
  border-bottom: solid 1px #a0a0a0;
}
div.PhotostreamViewerControl {
  height: 32px;
  text-align: center;
  vertical-align: middle;
}
img.PhotostreamViewerIcon {}

span.PhotostreamViewerArrow {
  height: 32px;
  width: 24px;
  padding: 4px;
  vertical-align: bottom;
}

a.PhotostreamViewerArrow:link { color: #000000; text-decoration: none; font-weight: bold; }
a.PhotostreamViewerArrow:visited { color: #000000; text-decoration: none; font-weight: bold; }
a.PhotostreamViewerArrow:active { color: #000000; text-decoration: none; font-weight: bold; }
a.PhotostreamViewerArrow:hover { color: #afafaf; text-decoration: none; font-weight: bold; }

div.DatastreamViewer {
  width: 320px;
  height: 160px;
  border: solid 1px #a0a0a0;
  margin: 10px;
}

    </style>
  </head>
  <body>
<?php
        for ($i = 0; $i < $group->count_photostreams(); $i++):
                $photostream = $group->get_photostream($i);
                $id = 'photostream_' . $photostream->id;
                $photos_id = "photos_" . $photostream->id;
                $path = $selector->construct_path($photostream->id, ".json");
?>
    <div id="<?php echo $id ?>" class="PhotostreamViewer"></div>

    <script type="text/javascript"> 
      var <?php echo $id ?> = new OpenSensorData("<?php echo $base_url ?>");
      var <?php echo $photos_id ?> = <?php echo $id ?>.get("photos/<?php echo $path ?>");
      new PhotostreamViewer(<?php echo $photos_id ?>, "<?php echo $id ?>");
    </script>
<?php endfor; ?>
<?php
        for ($i = 0; $i < $group->count_datastreams(); $i++):
                $datastream = $group->get_datastream($i);
                $id = 'datastream_' . $datastream->id;
                $path = $selector->construct_path($datastream->id, ".csv");
?>
    <div id="<?php echo $id ?>" class="DatastreamViewer"></div>
    <script type="text/javascript">
      g = new Dygraph(
          document.getElementById("<?php echo $id ?>"),
          "<?php echo $base_url ?>/datastreams/<?php echo $path ?>",
          { labels: [ "Date", "<?php echo $datastream->name ?>" ] }
          
      );
    </script>
<?php endfor; ?>
  </body>
</html>
<?php db_close(); ?>
