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
require_once "selector.model.php";

class Photostream
{
        public $id;
        public $owner;
        public $name;
        public $description;
        public $timezone;
        public $longitude;
        public $latitude;

        public function load($id) 
        {
                global $mysqli;

                $query = "SELECT `id`, `owner`, `name`, `description`, `timezone`, `longitude`, `latitude` "
                        . "FROM `photostream` WHERE `id`=" . $mysqli->real_escape_string($id);
                //echo $query . "\n";

                $res = $mysqli->query($query);
                if (!$res || ($res->num_rows != 1)) return false;

                $this->import($res->fetch_assoc());

                return true;
        }

        public function import($row) 
        {
                $this->id = $row['id'];
                $this->owner = $row['owner'];
                $this->name = $row['name'];
                $this->description = $row['description'];
                $this->timezone = $row['timezone'];
                $this->longitude = $row['longitude'];
                $this->latitude = $row['latitude'];
        }

        public function loadIdUsingOwnerAndName($owner, $name) 
        {
                global $mysqli;

                $query = ("SELECT `id` FROM `photostream` WHERE `owner`=" . $mysqli->real_escape_string($owner)
                          . " AND `name`='" . $mysqli->real_escape_string($name) . "'");
                //echo $query . "\n";

                $res = $mysqli->query($query);
                if (!$res || ($res->num_rows != 1)) 
                        return false;

                $row = $res->fetch_assoc();

                $this->id = $row['id'];

                return true;
        }

        public function create() 
        {
                global $mysqli;

                if (isset($this->id)) 
                        return false;

                $query = "INSERT INTO `photostream` VALUES (" 
                        . "NULL," 
                        . $mysqli->real_escape_string($this->owner) . ","
                        . "'" . $mysqli->real_escape_string($this->name) . "',"
                        . "'" . $mysqli->real_escape_string($this->description) . "',";
                
                if (isset($this->timezone)) 
                        $query .= $mysqli->real_escape_string($this->timezone) . ",";
                else $query .= "NULL,";
                
                if (isset($this->longitude)) 
                        $query .= $mysqli->real_escape_string($this->longitude) . ",";
                else $query .= "NULL,";
                
                if (isset($this->latitude)) 
                        $query .= $mysqli->real_escape_string($this->latitude) . ");";
                else $query .= "NULL);";

                //echo $query . "\n";
                /* return 1; */


                $res = $mysqli->query($query);
                if ($mysqli->errno) {
                        //echo $mysqli->error . "\n";
                        return false;
                }

                $this->id = $mysqli->insert_id;
                return true;
        }

        public function update() 
        {
                global $mysqli;

                if (!isset($this->id)) 
                        return false;

                $query = "REPLACE INTO `photostream` SET " 
                        . "id=" . $mysqli->real_escape_string($this->id) . ","
                        . "owner=" . $mysqli->real_escape_string($this->owner) . ","
                        . "name='" . $mysqli->real_escape_string($this->name) . "',"
                        . "description='" . $mysqli->real_escape_string($this->description) . "',";
                
                if (isset($this->timezone)) 
                        $query .= "timezone=" . $mysqli->real_escape_string($this->timezone) . ",";
                else $query .= "timezone=NULL,";
                
                if (isset($this->longitude)) 
                        $query .= "longitude=" . $mysqli->real_escape_string($this->longitude) . ",";
                else $query .= "longitude=NULL,";
                
                if (isset($this->latitude)) 
                        $query .= "latitude=" . $mysqli->real_escape_string($this->latitude) . ";";
                else $query .= "latitude=NULL;";

                //echo $query . "\n";
                /* return 1; */



                $res = $mysqli->query($query);

                return ($mysqli->errno == 0);
        }

        public static function row_to_path($dir, $row, $size)
        {
                $size = $size? $size : "orig";
                return sprintf("%s/%d/%04d/%02d/%02d/%02d%02d%02d-%s.jpg",
                               $dir, $row['photostream'], 
                               $row['year'], $row['month'], $row['day'],
                               $row['hour'], $row['minute'], $row['second'],
                               $size);
        }

        public static function selector_to_path($dir, $sel, $size)
        {
                $d = $sel->date;
                $size = $size? $size : $sel->size;
                return sprintf("%s/%d/%04d/%02d/%02d/%02d%02d%02d-%s.jpg",
                               $dir, $sel->id, 
                               $sel->date->year, $sel->date->month, $sel->date->day,
                               $sel->date->hour, $sel->date->minute, $sel->date->second,
                               $size);
        }

        public static function to_query($sel)
        {
                $query = "SELECT * FROM photos WHERE photostream=" . $sel->id;

                if ($sel->date != NULL) {
                        if ($sel->date->has_time()) {
                                $query .= sprintf(" AND `datetime` = '%s'", 
                                                  $sel->date->format("Y-m-d H:M:S"));
                        } else {
                                $query .= sprintf(" AND date(`datetime`) = '%s'", 
                                                  $sel->date->format("Y-m-d"));
                        }
                } else if (($sel->from != NULL) 
                           && ($sel->to != NULL)) {
                        if ($sel->from->has_time()) {
                                $query .= sprintf(" AND `datetime` >= '%s'", 
                                                  $sel->from->format("Y-m-d H:M:S"));
                        } else {
                                $query .= sprintf(" AND date(`datetime`) >= '%s'", 
                                                  $sel->from->format("Y-m-d"));
                        }
                        if ($sel->from->has_time()) {
                                $query .= sprintf(" AND `datetime` <= '%s'", 
                                                  $sel->to->format("Y-m-d H:M:S"));
                        } else {
                                $query .= sprintf(" AND date(`datetime`) <= '%s'", 
                                                  $sel->to->format("Y-m-d"));
                        }
                }

                $query .= " ORDER BY `datetime` DESC";

                return $query;
        }
}

?>
