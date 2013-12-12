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
require_once "photostream.inc.php";

db_connect();

$timeformat = input_get_timeformat();
if (!is_valid_timeformat($timeformat))
        badRequest("Invalid time format");

function formatTimeDygraph($year, $month, $day, $hour, $minute, $second, $timezone)
{
        return sprintf("%04d/%02d/%02d %02d:%02d:%02d",
                       $year, $month, $day, 
                       $hour, $minute, $second);
}

function formatTimeIso8601($year, $month, $day, $hour, $minute, $second, $timezone)
{
        return sprintf("%04d-%02d-%02dT%02d:%02d:%02dZ",
                       $year, $month, $day, 
                       $hour, $minute, $second);
}

function formatTime($format, $year, $month, $day, $hour, $minute, $second, $timezone)
{
        if ($format == "iso8601") 
                return formatTimeIso8601($year, $month, $day, 
                                         $hour, $minute, $second,
                                         $timezone);
        if ($format == "dygraph") 
                return formatTimeDygraph($year, $month, $day, 
                                         $hour, $minute, $second,
                                         $timezone);
        return "BADTIMEFORMAT";
}

$path = $_GET['path'];
$selector = new Selector();
if (!$selector->parse($path))
        badRequest($selector->err);

$photostream = new Photostream();
if (!$photostream->load($selector->id)) 
        badRequest("Invalid photostream ID: " . $selector->id);

$query = Photostream::to_query($selector);
//echo $query . "\n";

$results = $mysqli->query($query);
if (!$results) internalServerError();

header('Access-Control-Allow-Origin: *');

if ($selector->format == "txt") {

        for ($i = 0; $i < $results->num_rows; $i++) {
                $row = $results->fetch_assoc();
                $url = sprintf("%s/photo/%d/%04d%02d%02d-%02d%02d%02d.jpg",
                               $base_url,
                               $row['photostream'], 
                               $row['year'], $row['month'], $row['day'],
                               $row['hour'], $row['minute'], $row['second']);
                echo $url . "\n";
        }

 } else {

        header('Content-type: application/json');

        echo "[\n";

        for ($i = 0; $i < $results->num_rows; $i++) {

                $row = $results->fetch_assoc();

                $timezone = $row['timezone'];
                if (!$timezone)
                        $timezone = $photostream->timezone;
                if (!$timezone)
                        $timezone = "null";

                $longitude = $row['longitude'];
                if (!$longitude)
                        $longitude = $photostream->longitude;
                if (!$longitude)
                        $longitude = "null";
        
                $latitude = $row['latitude'];
                if (!$latitude)
                        $latitude = $photostream->latitude;
                if (!$latitude)
                        $latitude = "null";


                $line = "{";

                $path = sprintf("photo/%d/%04d%02d%02d-%02d%02d%02d",
                               $row['photostream'], 
                               $row['year'], $row['month'], $row['day'],
                               $row['hour'], $row['minute'], $row['second']);

                $line .= '"path":"' . $path . '"';

                $url = sprintf("%s/photo/%d/%04d%02d%02d-%02d%02d%02d.jpg",
                               $base_url,
                               $row['photostream'], 
                               $row['year'], $row['month'], $row['day'],
                               $row['hour'], $row['minute'], $row['second']);

                $line .= ',"orig":"' . $url . '"';


                $url = sprintf("%s/photo/%d/%04d%02d%02d-%02d%02d%02d_m.jpg",
                               $base_url,
                               $row['photostream'], 
                               $row['year'], $row['month'], $row['day'],
                               $row['hour'], $row['minute'], $row['second']);

                $line .= ',"medium":"' . $url . '"';

                $url = sprintf("%s/photo/%d/%04d%02d%02d-%02d%02d%02d_s.jpg",
                               $base_url,
                               $row['photostream'], 
                               $row['year'], $row['month'], $row['day'],
                               $row['hour'], $row['minute'], $row['second']);

                $line .= ',"small":"' . $url . '"';


                $url = sprintf("%s/photo/%d/%04d%02d%02d-%02d%02d%02d_t.jpg",
                               $base_url,
                               $row['photostream'], 
                               $row['year'], $row['month'], $row['day'],
                               $row['hour'], $row['minute'], $row['second']);

                $line .= ',"thumb":"' . $url . '"';

                $line .= ',"datetime":"' . formatTime($timeformat, 
                                                     $row['year'], $row['month'], $row['day'], 
                                                     $row['hour'], $row['minute'], $row['second'], 
                                                     $timezone) . '"';

                $line .= ',"timezone":' . $timezone;                
        
                $line .= ',"longitude":' . $longitude . ',"latitude":' . $latitude;

                $line .= "}";
                if ($i < $results->num_rows - 1) $line .= ",";

                echo $line . "\n";
        }

        echo "]\n";
 }

db_close();

?>
