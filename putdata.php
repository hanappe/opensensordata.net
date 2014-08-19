<?php
/*  
    OpenSensorData.net 

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
require_once "account.inc.php";
require_once "datastream.inc.php";
require_once "account_stats.inc.php";

//downForMaintenanceUntil("Fri, 6 Dec 2013 12:00:00 GMT");

db_connect();

$key = input_get_key();
if (!is_valid_key($key)) 
        badRequest("Invalid key");

//echo "Key: " . $key . "\n";

$account = new Account();
if (!$account->load("key", $key)) 
        badRequest("Invalid key");
if (!$account->validated) 
        unauthorized("Invalid account");

$idmap = array();


// Check upload limits

$stats = new AccountStats();
if (!$stats->load($account->id) && !$stats->create($account->id)) {
        internalServerError("The access to the account statistics failed: "
                            . $stats->err);
}

$curweek = (int) date("YW");
if ($stats->data_curweek != $curweek) {
        // Reset the counter every week.
        $stats->data_curweek = $curweek;
        $stats->data_uploads = 0;
 }
if (($stats->data_maxuploads > 0) 
    && ($stats->data_uploads > $max_weekly_data_uploads)) { 
        badRequest("Too many uploads.");
 }


function get_number($s, $min, $max, $name)
{
        if (!is_numeric($s)) badRequest("Expected a number for $name");
        $n = (int) $s;
        if (($n < $min) || ($n > $max)) badRequest("Invalid $name: " . $s);
        return $n;
}

/*

  Accepted IDs for datastreams:
  - stream-id
  - user-id/group-id/stream-index
  - user-id/group-id/stream-name
  - user-id/group-name/stream-index
  - user-id/group-name/stream-name
  - user-name/group-id/stream-index
  - user-name/group-id/stream-name
  - user-name/group-name/stream-index
  - user-name/group-name/stream-name

*/
function get_id($s, $linenum)
{
        global $idmap, $mysqli;

        if (isset($idmap[$s]))
            return $idmap[$s];

        $a = preg_split('|/|', $s);

        if (count($a) == 1) {
                if (!is_numeric($a[0])) 
                        badRequest("Line $linenum: Invalid datastream ID: " . $s);
                $n = (int) $a[0];
                if ($n < 0) 
                        badRequest("Line $linenum: Invalid datastream ID: " . $s);
                $idmap[$s] = $n;
                return $n;

        } else if (count($a) == 3) {
                $n = get_datastream_id($a[0], $a[1], $a[2]);
                $idmap[$s] = $n;
                return $n;
        }
}

function get_value($s, $linenum)
{
        $t = trim($s);
        if ($t == '')
                return false;
        else if (is_numeric($t)) 
                return (float) $t;
        else if ($t == "null") 
                return "NULL";
        else return false;
}

function get_longitude($s, $linenum)
{
        $t = trim($s);
        if ($t == '')
                return "NULL";
        if (!is_numeric($t)) 
                badRequest("Line $linenum: Expected a numerical value for the longitude: $s");
        $n = (float) $s;
        if (($n < -180) || ($n > 180)) 
                badRequest("Line $linenum: Invalid the longitude: -180 <= longitude <= 180");
        return $n;
}

function get_latitude($s, $linenum)
{
        $t = trim($s);
        if ($t == '')
                return "NULL";
        if (!is_numeric($t)) 
                badRequest("Line $linenum: Expected a numerical value for the latitude: $s");
        $n = (float) $s;
        if (($n < -90) || ($n > 90)) 
                badRequest("Line $linenum: Invalid the latitude: -90 <= latitude <= 90");
        return $n;
}

function insert_unique(array &$a, $id, $linenum)
{
        for ($k = 0; $k < count($a); $k++) {
                $datastream = $a[$k];
                if ($datastream->id == $id) 
                        return;
        }
        $datastream = new Datastream();
        if (!$datastream->load($id)) 
                badRequest("Line $linenum: Invalid datastream ID: $id");
        $a[] = $datastream;
}

$timestamp_now = NULL;

function get_timestamp($s, $linenum)
{
        global $timestamp_now;

        // NOW, now, -
        if (($s == 'NOW') || ($s == 'now') || ($s == '-')) {
                if (!$timestamp_now) {
                        $t = time();
                        $year = (int) date("Y", $t);
                        $month = (int) date("m", $t);
                        $day = (int) date("d", $t); 
                        $hour = (int) date("H", $t);
                        $minute = (int) date("i", $t);
                        $second = (int) date("s", $t); 
                        $timezone = (int) date("O", $t);
                        $timezone = (int) (60 * ($timezone / 100.0));
                        $timestamp_now = array($year, $month, $day, 
                                               $hour, $minute, $second, 
                                               $timezone);
                }
                return $timestamp_now;
        }

        // See ISO 8601
        //
        // YYYY-MM-DDThh:mm:ss
        // YYYY-MM-DDThh:mm:ssZ
        // YYYY-MM-DDThh:mm:ss+01:00
        // YYYY-MM-DDThh:mm:ss-01:00
        // timestamp <date> "T" <time> <timezone>?
        // date: YYYY-MM-DD
        // time: hh:mm:ss
        // timezone: Z | <sign> hh:mm
        $year = get_number(substr($s, 0, 4), 1900, 2100, "year @ line $linenum");
        $month = get_number(substr($s, 5, 2), 1, 12, "month @ line $linenum");
        $day = get_number(substr($s, 8, 2), 1, 31, "day @ line $linenum");
        $hour = get_number(substr($s, 11, 2), 0, 23, "hour @ line $linenum");
        $minute = get_number(substr($s, 14, 2), 0, 59, "minute @ line $linenum");
        $second = get_number(substr($s, 17, 2), 0, 59, "second @ line $linenum");
        $timezone = "NULL";
        if (strlen($s) > 19) {
                if ($s[19] == "Z") {
                        $timezone = 0;
                } else if ($s[19] == "+") {
                        $timezone = (60 * get_number(substr($s, 20, 2), 0, 12, 
                                                    "timezone @ line $linenum")
                                     + get_number(substr($s, 23, 2), 0, 59, 
                                                  "timezone @ line $linenum"));
                } else if ($s[19] == "-") {
                        $timezone = -(60 * get_number(substr($s, 20, 2), 0, 12, 
                                                     "timezone @ line $linenum")
                                      + get_number(substr($s, 23, 2), 0, 59, 
                                                   "timezone @ line $linenum"));
                }
        }
        return array($year, $month, $day, 
                     $hour, $minute, $second, 
                     $timezone);
}

$data = file_get_contents('php://input');
$lines = preg_split('/\v+/', $data);
$data = NULL;

$datastreams = array();

$query = "REPLACE INTO datapoints(datastream,`datetime`,timezone,longitude,latitude,value) VALUES ";

$count_datapoints = 0;
$count_emptyvalues = 0;
$count_tags = 0;

for ($i = 0; $i < count($lines); $i++) {

        //echo "Line " . $i . ": " . $lines[$i] . "\n";

        $currentLine = trim($lines[$i]); 
        if ($currentLine != '') {
                $currentLine = preg_split('/,/', $currentLine);
                if (count($currentLine) < 3) 
                        badRequest("Line $i: Bad number of data fields.");
                
                $datastream = get_id($currentLine[0], $i);
                $timestamp = get_timestamp($currentLine[1], $i);

                $value = get_value($currentLine[2], $i);
                if ($value === false) {
                        $count_emptyvalues++;
                        continue;
                }

                if (count($currentLine) >= 4) 
                        $longitude = get_longitude($currentLine[3], $i);
                else $longitude = "NULL";

                if (count($currentLine) >= 4) 
                        $latitude = get_latitude($currentLine[3], $i);
                else $latitude = "NULL";

                if ($count_datapoints > 0) $query .= ",";

                $query .= ("(" . $datastream . "," 
                           . "'" . (sprintf("%04d", $timestamp[0]) 
                                    . "-" . sprintf("%02d", $timestamp[1]) 
                                    . "-" . sprintf("%02d", $timestamp[2]) 
                                    . " " . sprintf("%02d", $timestamp[3])
                                    . ":" . sprintf("%02d", $timestamp[4])
                                    . ":" . sprintf("%02d", $timestamp[5])) 
                           . "'," 
                           . $timestamp[6] . "," 
                           . $longitude . "," 
                           . $latitude . "," 
                           . $value . ")" );

                $count_datapoints++;
                
                insert_unique($datastreams, $datastream, $i);
        }
}

//$query .= ";";
$lines = NULL;

for ($i = 0; $i < count($datastreams); $i++) {
        $datastream = $datastreams[$i];
        if ($datastream->owner != $account->id)
                unauthorized("It seems that you don't have the access rights for the datastream with " . $datastream->id);
}

//echo $query . "\n";

$r = $mysqli->query($query);
if ($mysqli->errno) {
        internalServerError("Failed to store the datapoints: " . $mysqli->error 
                            . ", Query: " . $query);
}

$stats->data_uploads += $count_datapoints;
$stats->update();

db_close();

echo "OK. $count_datapoints datapoints have been stored. $count_emptyvalues lines with empty values were found.\n";

?>
