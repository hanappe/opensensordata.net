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
require_once "selector.model.php";


function printCSVDatapoint($row, $d, $geolocated)
{
        $value = $row['value'];
        if ($value == null)
                $value = "null";

        if ($geolocated) {
                $longitude = $row['longitude'];
                if (!$longitude)
                        $longitude = $datastream->longitude;
                if (!$longitude)
                        $longitude = "0";
                
                $latitude = $row['latitude'];
                if (!$latitude)
                        $latitude = $datastream->latitude;
                if (!$latitude)
                        $latitude = "0";
                
                echo sprintf("%04d/%02d/%02d %02d:%02d:%02d,%s,%f,%f\n",
                             $d->year, $d->month, $d->day, 
                             $d->hour, $d->minute, $d->second,
                             $value, 
                             $longitude, $latitude);

        } else {
                echo sprintf("%04d/%02d/%02d %02d:%02d:%02d,%s\n",
                             $d->year, $d->month, $d->day, 
                             $d->hour, $d->minute, $d->second,
                             $value);

        }
}

function printJSONDatapoint($row, $d, $geolocated, $last)
{
        $value = $row['avg'];
        if ($value == null)
                $value = "null";

        if ($geolocated) {
                $longitude = $row['longitude'];
                if (!$longitude)
                        $longitude = $datastream->longitude;
                if (!$longitude)
                        $longitude = "0";
                
                $latitude = $row['latitude'];
                if (!$latitude)
                        $latitude = $datastream->latitude;
                if (!$latitude)
                        $latitude = "0";
                
                echo sprintf("[\"%04d/%02d/%02d %02d:%02d:%02d\",%s,%f,%f]",
                             $d->year, $d->month, $d->day, 
                             $d->hour, $d->minute, $d->second,
                             $value, 
                             $longitude, $latitude);

        } else {
                echo sprintf("[\"%04d/%02d/%02d %02d:%02d:%02d\",%s]",
                             $d->year, $d->month, $d->day, 
                             $d->hour, $d->minute, $d->second,
                             $value);
                /* echo sprintf("[\"%04d/%02d/%02d %02d:%02d:%02d\",[%s,%s,%s]]", */
                /*              $d->year, $d->month, $d->day,  */
                /*              $d->hour, $d->minute, $d->second, */
                /*              $row['min'], $row['avg'], $row['max']); */
        }

        if (!$last) echo ",\n"; 
        else echo "\n"; 
}

function printDatapoints($results, $timeformat, $default_timezone, 
                         $geolocated, $content_disposition,
                         $format, $delta)
{
        header('Content-Disposition: attachment; filename=' . $content_disposition); 

        $osddatetime = new OSDDateTime();

        if ($format == "csv") {
                header('Content-type: text/csv');
                while ($row = $results->fetch_assoc()) {
                        $osddatetime->import($row);
                        printCSVDatapoint($row, $osddatetime, $geolocated);
                }

        } else if ($format == "json") {
                header("Content-type: application/json");
                echo "[\n";
                $last_stamp = 0;
                for ($i = 0; $i < $results->num_rows; $i++) {
                        $row = $results->fetch_assoc();
                        $osddatetime->import($row);
                        $last = ($i == $results->num_rows - 1);
                        if (($i > 0) && ($row['timestamp'] - $last_stamp > 3600)) {
                                echo sprintf("[\"%04d/%02d/%02d %02d:%02d:%02d\",null],",
                                             $osddatetime->year, $osddatetime->month, $osddatetime->day, 
                                             $osddatetime->hour, $osddatetime->minute, $osddatetime->second);
                        } 
                        printJSONDatapoint($row, $osddatetime, $geolocated, $last);
                        $last_stamp = $row['timestamp'];
                }
                echo "]\n";
        }
}


db_connect();

$timeformat = input_get_timeformat();
if (!is_valid_timeformat($timeformat))
        badRequest("Invalid time format");

$geolocated = input_get_geolocated();
$geolocated = ($geolocated == "y")? true : false;

$path = $_GET['path'];

$selector = new Selector();
if (!$selector->parse($path))
        badRequest($selector->err);

if (($selector->format != "csv") && ($selector->format != "json"))
        badRequest("Unsupported output format");

$datastream = new Datastream();
if (!$datastream->load($selector->id)) 
        badRequest("Invalid datastream ID: " . var_export($selector));

header('Access-Control-Allow-Origin: *');

$delta = 5;

$query = Datastream::to_filter_query($selector, $delta);
$filename = $selector->to_filename("datastream");
        
$results = $mysqli->query($query);
if (!$results) {
        internalServerError("Failed to read the datapoints, Query: " . $query);
 }

//echo $query . "\n";
printDatapoints($results, $timeformat, 
                $datastream->timezone, $geolocated, 
                $filename, $selector->format, 60);

db_close();

?>
